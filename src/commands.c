// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "args.h"
#include "commands.h"
#include "constants.h"
#include "data.h"
#include "util.h"
#include "version.h"

static struct state_data *get_custom_state_data(bool is_active)
{
        char *custom_state;

        print_user_message("Enter a custom state: (Ex. Late): ");

        custom_state = ingest_user_input(25);

        return create_custom_state_data(is_active, custom_state);
}

static struct state_data *get_defined_state_data(bool is_active)
{
        size_t user_choice;
        enum state_value defined_state;

        print_state_values();

        fputc('\n', stdout);

        print_user_message("State (Ex. 1): ");

        // TODO: Stop using scanf
        int result = scanf("%zu", &user_choice);

        if (!validate_scan_result(result)) {
                die("You entered an invalid state id");
        }

        defined_state = num_to_state_value(user_choice);

        if (defined_state == INVALID) {
                die("Please choose from the provided list of state ids");
        }

        return create_defined_state_data(is_active, defined_state);
}

static bool is_initialized(void)
{
        if (path_exists(TODO_DIR_NAME)) {
                return true;
        }

        return false;
}

int add_command(int argc, char **argv)
{
        uint64_t priority;
        struct state_data *state = NULL;
        char *subject;
        char *description;
        struct todo_data *todo = NULL;

        if (!is_initialized()) {
                die("You must first initialize todo before adding a todo");
        }

        struct argument_list *arg_list = create_argument_list(argc, argv, 2);

        if (arg_list->length != 1) {
                die("Invalid arguments supplied see 'todo help %s'",
                    command_to_string(ADD));
        }

        // arguments points to a item in argv
        char *id = arg_list->arguments[0];

        destroy_argument_list(arg_list);

        print_user_message("Priority (Ex. 1): ");

        int result = scanf("%" PRIu64, &priority);

        if (!validate_scan_result(result)) {
                die("You entered an invalid priority");
        }

        bool use_defined = input_to_bool("Use defined state?", true);
        bool is_active = input_to_bool("Mark this todo active?", true);

        if (use_defined) {
                state = get_defined_state_data(is_active);
        } else {
                state = get_custom_state_data(is_active);
        }

        print_user_message("Enter a subject (Ex. Add a file): ");

        subject = ingest_user_input(50);

        print_user_message("Enter a description (Ex. Add the make file): ");

        description = ingest_user_input(100);

        todo = create_todo_data(id, priority, state, subject, description);

        if (todo == NULL || todo->state == NULL) {
                die("Could not allocate memory for todo data");
        }

        save_todo_data_to_file(todo);

        destroy_todo_data(todo);

        return EXIT_SUCCESS;
}

void init_command(void)
{
        if (is_initialized()) {
                die("You cannot re-initialize todo");
        }

        if (!create_directory(TODO_DIR_NAME)) {
                die("Failed to initialize todo");
        }

        print_user_message("Successfully initialized todo in %s\n",
                           TODO_DIR_NAME);
}

static void file_callback(const struct dirent *entry)
{
        char *path = create_file_path(TODO_DIR_NAME, entry->d_name);
        struct stat sb;

        if (stat(path, &sb) == -1) {
                die("%s: %s", strerror(errno), entry->d_name);
        }

        if (S_ISREG(sb.st_mode)) {
                FILE *todo_file = open_file(path, "r");

                if (todo_file == NULL) {
                        die("Could read todo data file for: %s", entry->d_name);
                }

                struct todo_data *todo = read_todo_from_file(todo_file);

                printf("%-10s%-10" PRIu64, todo->id, todo->priority);

                if (todo->state->is_custom) {
                        printf("%-20s", todo->state->string);
                } else {
                        printf("%-20s",
                               state_value_to_string(todo->state->value));
                }

                printf("%s\n", todo->subject);

                destroy_todo_data(todo);

                fclose(todo_file);
        }

        free(path);
}

void list_command(void)
{
        printf("%-10s%-10s%-20s%s\n", "Id.", "Prior.", "State", "Subject");
        directory_iterator(TODO_DIR_NAME, file_callback);
}

void version_command(void)
{
        printf("Todo version %d.%d.%d\n",
               VERSION_MAJOR,
               VERSION_MINOR,
               VERSION_PATCH);
}
