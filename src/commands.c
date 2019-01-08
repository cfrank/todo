// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "commands.h"
#include "constants.h"
#include "data.h"
#include "util.h"
#include "version.h"

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

static struct state_data *get_custom_state_data(bool is_active)
{
        char *custom_state;

        print_user_message("Enter a custom state: (Ex. Late): ");

        custom_state = ingest_user_input(25);

        return create_custom_state_data(is_active, custom_state);
}

int add_command(int argc, char **argv)
{
        uint64_t priority;
        struct state_data *state = NULL;
        char *subject;
        char *description;
        struct todo_data *todo = NULL;

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

        print_user_message("Successfully saved '%s'\n", todo->id);

        destroy_todo_data(todo);

        return EXIT_SUCCESS;
}

void init_command(void)
{
        if (!create_directory(TODO_DIR_NAME)) {
                // Directory already exists
                die("You cannot re-initialize todo");
        }

        print_user_message("Successfully initialized todo in %s\n",
                           TODO_DIR_NAME);
}

void version_command(void)
{
        printf("Todo version %d.%d.%d\n",
               VERSION_MAJOR,
               VERSION_MINOR,
               VERSION_PATCH);
}
