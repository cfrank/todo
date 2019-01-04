// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "constants.h"
#include "data.h"
#include "util.h"
#include "version.h"

int add_command(void)
{
        uint64_t priority;
        struct state_data *state = NULL;
        char *subject;
        char *description;
        struct todo_data *todo = NULL;

        print_user_message("Priority (Ex. 1): ");

        int result = scanf("%" PRIu64, &priority);

        if (!validate_scan_result(result)) {
                die("You entered an invalid priority");
        }

        bool use_defined = input_to_bool("Use defined state?", true);
        bool is_active
                = input_to_bool("Do you want to mark this todo active?", true);

        // TODO: Maybe move these into static functions
        if (use_defined) {
                size_t user_choice;
                enum state_value defined_state;

                print_state_values();

                fputc('\n', stdout);

                print_user_message("State (Ex. 1): ");

                int result = scanf("%zu", &user_choice);

                if (!validate_scan_result(result)) {
                        die("You entered an invalid state state id");
                }

                if (user_choice > RE_OPENED) {
                        die("Please choose from the provided list of state "
                            "ids");
                }

                defined_state = num_to_state_value(user_choice);

                if (defined_state != INVALID) {
                        state = create_defined_state_data(is_active,
                                                          defined_state);
                }
        } else {
                char *custom_state;

                print_user_message("Enter a custom state: (Ex. Late): ");

                custom_state = ingest_user_input(25);

                state = create_custom_state_data(is_active, custom_state);
        }

        print_user_message("Enter a subject (Ex. Add a file): ");

        subject = ingest_user_input(50);

        print_user_message("Enter a description (Ex. Add the make file): ");

        description = ingest_user_input(100);

        todo = create_todo_data(1, priority, state, subject, description);

        die("Successfully saved %s", todo->subject);

        return EXIT_SUCCESS;
}

void init_command(void)
{
        if (!create_directory(TODO_DIR_NAME)) {
                // Directory already exists
                die("You cannot re-initialize todo");
        }

        char *file_path = create_file_path(TODO_DIR_NAME, "todos.data");

        if (!create_file(file_path, "ab+")) {
                die("Failed to create todo data file");
        }

        free(file_path);
}

void version_command(void)
{
        printf("Todo version %d.%d.%d\n",
               VERSION_MAJOR,
               VERSION_MINOR,
               VERSION_PATCH);
}
