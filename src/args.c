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
        {HELP, "help"},
        {INIT, "init"},
        {LIST, "list"},
        {VERSION, "version"},
        {VIEW, "view"},
        {UNKNOWN, "unknown"},
};

struct argument_list *create_argument_list(int argc, char **argv,
                                           size_t start_index)
{
        if (start_index >= argc) {
                die("Attempted to retrieve nonexistent command line argument");
        }

        struct argument_list *arg_list = malloc(sizeof(struct argument_list));

        if (arg_list == NULL) {
                die("Failed to allocate memory for argument list");
        }

        arg_list->arguments = malloc(sizeof(char *) * (argc - 1) - start_index);
        arg_list->length = 0;

        if (arg_list->arguments == NULL) {
                die("Failed to store arguments in the argument list");
        }

        for (size_t i = start_index; i < argc; ++i) {
                arg_list->arguments[arg_list->length] = argv[i];
                ++arg_list->length;
        }

        return arg_list;
}

void destroy_argument_list(struct argument_list *arg_list)
{
        free(arg_list->arguments);

        free(arg_list);
}

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
