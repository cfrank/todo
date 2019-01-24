// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#pragma once

#include <dirent.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef void (*file_callback_t)(const struct dirent *entry);

// String utilities
char *duplicate_string(const char *string);
char *convert_string_to_lowercase(const char *string);

// Filesystem utilities
bool create_file(const char *file, const char *mode);
char *create_file_path(const char *directory_path, const char *filename);
bool create_directory(const char *directory_path);
size_t directory_iterator(const char *directory_path, file_callback_t callback);
bool path_exists(const char *path);
ssize_t read_file_until_delimiter(char **buffer, size_t *size, char delimiter,
                                  FILE *file, bool consume);
ssize_t read_line_from_file(char **buffer, size_t *size, FILE *file,
                            bool consume);
DIR *open_directory(const char *directory_path);
FILE *open_file(const char *file_path, const char *mode);
void remove_file(const char *file_path);

// Input utilities
char *ingest_user_input(size_t initial_size);
bool input_to_bool(const char *message, bool affirmative_default);
bool validate_scan_result(int scan_result);

// General utilities
void die(const char *format, ...);
void print_user_message(const char *format, ...);
