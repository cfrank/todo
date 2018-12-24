// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#pragma once

enum arg_command {
    ADD,
    CREATE,
    DELETE,
    EDIT,
    INIT,
    LIST,
    VIEW,
    VERSION,
    UNKNOWN,
    COMMAND_COUNT, // Needs to be last - used to count number of commands
};

enum arg_command get_command(const char *command);
const char *command_to_string(enum arg_command command);
