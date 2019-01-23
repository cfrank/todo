// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util.h"

static void flush_input_buffer(void);

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

bool create_directory(const char *directory_path)
{
        if (mkdir(directory_path, 0777) < 0) {
                if (errno != EEXIST) {
                        die("%s: %s", directory_path, strerror(errno));
                }

                return false;
        }

        return true;
}

void directory_iterator(const char *directory_path, file_callback_t callback)
{
        DIR *stream = opendir(directory_path);

        if (stream == NULL) {
                die("Failed to list %s: %s", directory_path, strerror(errno));
        }

        struct dirent *directory;

        while ((directory = readdir(stream)) != NULL) {
                callback(directory);
        }

        if (errno == EBADF) {
                die("Failed to read todos from directory stream");
        }

        closedir(stream);
}

bool create_file(const char *file_path, const char *mode)
{
        FILE *file = open_file(file_path, mode);

        if (file != NULL) {
                fclose(file);

                return true;
        }

        return false;
}

char *create_file_path(const char *directory_path, const char *filename)
{
        size_t directory_length = strlen(directory_path);
        size_t file_length = strlen(filename);
        char *file_path = malloc((directory_length + file_length) + 1);

        if (file_path == NULL) {
                die("Could not create file path for %s", filename);
        }

        strncpy(file_path, directory_path, directory_length);
        strncpy(file_path + directory_length, filename, file_length);
        file_path[directory_length + file_length] = '\0';

        return file_path;
}

bool path_exists(const char *path)
{
        if (access(path, F_OK) != 0) {
                return false;
        }

        return true;
}

ssize_t read_line_from_file(char **buffer, size_t *size, FILE *file,
                            bool consume)
{
        return read_file_until_delimiter(buffer, size, '\n', file, consume);
}

ssize_t read_file_until_delimiter(char **buffer, size_t *size, char delimiter,
                                  FILE *file, bool consume)
{
        char ch;
        char *buffer_pos;

        if (buffer == NULL || size == NULL || ferror(file)) {
                errno = EINVAL;
                return -1;
        }

        if (buffer == NULL || *size == 0) {
                // Empty buffer supplied
                *size = 128;
                *buffer = malloc(*size);

                if (*buffer == NULL) {
                        errno = ENOMEM;
                        return -1;
                }
        }

        buffer_pos = *buffer;

        for (;;) {
                ch = getc(file);

                if (ch == EOF) {
                        break;
                }

                if ((buffer_pos + 1) == (*buffer + *size)) {
                        // No more room in buffer
                        size_t new_size = *size * 2;
                        char *realloc_buffer = realloc(*buffer, new_size);

                        if (realloc_buffer == NULL) {
                                errno = ENOMEM;
                                return -1;
                        }

                        buffer_pos = realloc_buffer + (buffer_pos - *buffer);
                        *buffer = realloc_buffer;
                        *size = new_size;
                }

                *buffer_pos++ = ch;

                if (ch == delimiter) {
                        if (!consume) {
                                // If not consuming delimiter roll back buffer
                                buffer_pos--;
                        }

                        break;
                }
        }

        if (ch == EOF && buffer_pos == *buffer) {
                // Nothing was read
                return -1;
        }

        *buffer_pos = '\0';

        return buffer_pos - *buffer;
}

DIR *open_directory(const char *directory_path)
{
        DIR *directory = opendir(directory_path);

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

                die("Could not create %s - %s", file_path, strerror(errno));
        }
}

static void flush_input_buffer(void)
{
        int ch;

        while ((ch = getchar()) != EOF && ch != '\n') {
                continue;
        };

        if (ch == EOF) {
                die("Failed to flush the input buffer");
        }
}

char *ingest_user_input(size_t initial_size)
{
        const size_t max_reallocs = 10;
        size_t reallocs = 0;

        char ch;
        size_t size = 0;
        size_t buffer_size = initial_size;
        char *input = malloc(buffer_size);

        if (input == NULL) {
                die("Could allocate space for user input");
        }

        while ((ch = getchar()) != EOF && ch != '\n') {
                input[size++] = ch;

                if (size == buffer_size && reallocs <= max_reallocs) {
                        // User input is larger than current buffer size
                        // double it
                        buffer_size = size + initial_size;

                        input = realloc(input, buffer_size);

                        if (input == NULL) {
                                die("Could not save user input to memory");
                        }

                        reallocs++;
                }

                if (reallocs == max_reallocs) {
                        die("Invalid user input (Exceeded maximum length)");
                }
        }

        if (ch == EOF) {
                die("Could not read in user input");
        }

        input[size++] = '\0';

        return input;
}

static char get_affirmative_input(bool affirmative_default)
{
        if (affirmative_default) {
                return 'y';
        }

        return 'n';
}

bool input_to_bool(const char *message, bool affirmative_default)
{
        int input;
        char affirmative_input = get_affirmative_input(affirmative_default);

        print_user_message(message);

        if (affirmative_default) {
                fputs(" [Y,n] ", stdout);
        } else {
                fputs(" [y,N] ", stdout);
        }

        input = tolower(fgetc(stdin));

        if (input != '\n') {
                // Either 'y' or 'n' was entered manually so we need to
                // get rid of the dangling '\n'
                flush_input_buffer();
        }

        if (input == '\n' || input == affirmative_input) {
                return affirmative_default;
        }

        return !affirmative_default;
}

bool validate_scan_result(int scan_result)
{
        // Clear any data left in the buffer from the call to scanf
        flush_input_buffer();

        if (scan_result != 1) {
                return false;
        }

        return true;
}

void die(const char *format, ...)
{
        va_list vargs;

        va_start(vargs, format);
        fputs("ERROR: ", stderr);
        vfprintf(stderr, format, vargs);
        fputc('\n', stderr);
        va_end(vargs);

        exit(EXIT_FAILURE);
}

void print_user_message(const char *format, ...)
{
        va_list vargs;

        va_start(vargs, format);
        fputs(":: ", stdout);
        vfprintf(stdout, format, vargs);
        va_end(vargs);
}
