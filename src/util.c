// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

char *duplicate_string(const char *string)
{
        size_t string_length = strlen(string) + 1;
        char *lowercase_string = malloc(string_length);

        if (lowercase_string != NULL) {
                memcpy(lowercase_string, string, string_length);
        }

        return lowercase_string;
}

char *convert_string_to_lowercase(const char *string)
{
        char *lowercase_string = duplicate_string(string);

        if (lowercase_string == NULL) {
                return NULL;
        }

        size_t char_length = strlen(string);

        for (size_t i = 0; i < char_length; ++i) {
                lowercase_string[i] = tolower(lowercase_string[i]);
        }

        return lowercase_string;
}
