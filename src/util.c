// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

bool create_directory(const char *directory_name)
{
        if (mkdir(directory_name, 0777) < 0) {
                if (errno != EEXIST) {
                        perror(directory_name);
                        exit(EXIT_FAILURE);
                }

                return false;
        }

        return true;
}

void create_file(const char *file_path, const char *mode)
{
        FILE *file = open_file(file_path, mode);
        fclose(file);
}

char *create_file_path(const char *directory_name, const char *filename)
{
        size_t directory_length = strlen(directory_name);
        size_t file_length = strlen(filename);
        char *file_path = malloc((directory_length + file_length) + 1);

        if (file_path == NULL) {
                die("Could not create file path for %s", filename);
        }

        strncpy(file_path, directory_name, directory_length);
        strncpy(file_path + directory_length, filename, file_length);
        file_path[strlen(file_path)] = '\0';

        return file_path;
}

bool directory_exists(const char *directory_name)
{
        DIR *directory = open_directory(directory_name);

        if (!directory) {
                return false;
        }

        closedir(directory);
        return true;
}

DIR *open_directory(const char *directory_name)
{
        DIR *directory = opendir(directory_name);

        if (directory) {
                return directory;
        }

        return NULL;
}

FILE *open_file(const char *file_path, const char *mode)
{
        for (;;) {
                FILE *file = fopen(file_path, mode);

                if (file) {
                        return file;
                }

                if (errno == EINTR) {
                        continue;
                }

                die("Could not create %s", file_path);
        }
}

static char get_default_input(bool affirmative_default)
{
        if (affirmative_default) {
                return 'y';
        }

        return 'n';
}

bool input_to_bool(const char *message, bool affirmative_default)
{
        char input;

        fputs(message, stdout);

        if (affirmative_default) {
                fputs(" [Y,n]", stdout);
        } else {
                fputs(" [y,N]", stdout);
        }

        input = tolower(fgetc(stdin));

        if (input == '\n' || input == get_default_input(affirmative_default)) {
                return affirmative_default;
        }

        return !affirmative_default;
}

void die(const char *format, ...)
{
        va_list vargs;

        va_start(vargs, format);
        vfprintf(stderr, format, vargs);
        putchar('\n');
        va_end(vargs);

        exit(EXIT_FAILURE);
}
