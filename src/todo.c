// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the
// project

#include <stdio.h>
#include <stdlib.h>

#include "args.h"

int main(int argc, char **argv)
{
        if (argc < 1) {
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
                printf("Add...");
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
                printf("Initializing...");
                break;
        case LIST:
                printf("List...");
                break;
        case VERSION:
                printf("Version...");
                break;
        case VIEW:
                printf("View...");
                break;
        default:
                break;
        }

        return EXIT_SUCCESS;
}
