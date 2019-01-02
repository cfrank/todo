// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "commands.h"

int main(int argc, char **argv)
{
        if (argc < 2) {
                fprintf(stderr,
                        "Invalid arguments see 'todo help' for "
                        "usage info\n");
                return EXIT_FAILURE;
        }

        enum arg_command command = get_command(argv[1]);

        if (command == UNKNOWN) {
                fprintf(stderr,
                        "Invalid command found! Run 'todo help' for usage "
                        "info\n");

                return EXIT_FAILURE;
        }

        switch (command) {
        case ADD:
                add_command();
                break;
        case CREATE:
                printf("Create...");
                break;
        case DELETE:
                printf("Delete...");
                break;
        case EDIT:
                printf("Edit...");
                break;
        case HELP:
                printf("Help...");
                break;
        case INIT:
                init_command();
                break;
        case LIST:
                printf("List...");
                break;
        case VERSION:
                version_command();
                break;
        case VIEW:
                printf("View...");
                break;
        default:
                break;
        }

        return EXIT_SUCCESS;
}
