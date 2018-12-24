// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "util.h"

struct command_tuple {
        enum arg_command command;
        const char *name;
};

static const struct command_tuple command_list[COMMAND_COUNT] = {
        {ADD, "add"},
        {CREATE, "create"},
        {DELETE, "delete"},
        {EDIT, "edit"},
        {INIT, "init"},
        {LIST, "list"},
        {VIEW, "view"},
        {VERSION, "version"},
        {UNKNOWN, "unknown"},
};

enum arg_command get_command(const char *command)
{
        enum arg_command ret = UNKNOWN;
        char *lowercase_command = convert_string_to_lowercase(command);

        for (size_t i = 0; i < COMMAND_COUNT; ++i) {
                if (strcmp(lowercase_command, command_list[i].name) == 0) {
                        ret = command_list[i].command;
                }
        }

        free(lowercase_command);

        return ret;
}

const char *command_to_string(enum arg_command command)
{
        for (size_t i = 0; i < COMMAND_COUNT; ++i) {
                if (command_list[i].command == command) {
                        return command_list[i].name;
                }
        }

        return NULL;
}
