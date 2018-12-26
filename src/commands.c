// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "constants.h"
#include "util.h"

void init_command(void)
{
        if (!create_directory(TODO_DIR_NAME)) {
                // Directory already exists
                die("You cannot re-initialize todo");
        }

        char *file_path = create_file_path(TODO_DIR_NAME, "todos.data");

        puts(file_path);

        free(file_path);
}
