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
        // struct state_data *state;

        print_user_message("Priority (Ex. 1): ");

        int result = scanf("%" PRIu64, &priority);

        if (!validate_scan_result(result)) {
                die("You entered an invalid priority");
        }

        bool use_defined = input_to_bool("Use defined state?", true);

        if (use_defined) {
                printf("%s", "Hello world!");
        }

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
