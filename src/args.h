// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#pragma once

enum arg_command {
        ADD,
        CREATE,
        DELETE,
        EDIT,
        HELP,
        INIT,
        LIST,
        VERSION,
        VIEW,
        UNKNOWN,
        COMMAND_COUNT, // Needs to be last - used to count number of commands
};

struct argument_list {
        char **arguments;
        size_t length;
};

struct argument_list *create_argument_list(int argc, char **argv,
                                           size_t start_index);
void destroy_argument_list(struct argument_list *arg_list);
enum arg_command get_command(const char *command);
const char *command_to_string(enum arg_command command);
