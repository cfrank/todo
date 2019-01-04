// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#pragma once

#include <dirent.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// String utilities
char *duplicate_string(const char *string);
char *convert_string_to_lowercase(const char *string);

// Filesystem utilities
bool create_file(const char *file, const char *mode);
char *create_file_path(const char *directory_name, const char *filename);
bool create_directory(const char *directory_name);
bool directory_exists(const char *directory_name);
DIR *open_directory(const char *directory_name);
FILE *open_file(const char *file, const char *mode);

// Input utilities
char *ingest_user_input(uint64_t initial_size);
bool input_to_bool(const char *message, bool affirmative_default);
bool validate_scan_result(int scan_result);

// General utilities
void die(const char *format, ...);
void print_user_message(const char *format, ...);
