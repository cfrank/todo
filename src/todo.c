// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdlib.h>

#include "args.h"
#include "commands.h"
#include "util.h"

int main(int argc, char **argv)
{
        if (argc < 2) {
                die("Invalid arguments see 'todo help' for usage info");
        }

        enum arg_command command = get_command(argv[1]);

        if (command == UNKNOWN) {
                die("Invalid command found! Run 'todo help' for usage info");
        }

        switch (command) {
        case ADD:
                add_command(argc, argv);
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
                list_command();
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
