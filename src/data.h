// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum state_value {
        OPEN,
        IN_PROGRESS,
        URGENT,
        RESOLVED,
        CLOSED,
        RE_OPENED,
        INVALID, // Any new state values should be added above this
        STATE_COUNT, // Needs to be last - used to count number of state values
};

struct state_data {
        bool active;
        bool is_custom;
        union {
                enum state_value value;
                char *string;
        };
};

struct todo_data {
        char *id;
        uint64_t priority;
        struct state_data *state;
        char *subject;
        char *description;
};

struct todo_data *create_todo_data(char *id, uint64_t priority,
                                   struct state_data *state, char *subject,
                                   char *description);
struct state_data *create_custom_state_data(bool active, char *string);
struct state_data *create_defined_state_data(bool active,
                                             enum state_value value);
void save_todo_data_to_file(const struct todo_data *todo);
struct todo_data *read_todo_from_file(FILE *file_file);

enum state_value num_to_state_value(size_t num);
void print_state_values(void);
const char *get_state(const struct todo_data *todo);
const char *state_value_to_string(enum state_value value);

void destroy_todo_data(struct todo_data *todo);
void destroy_state_data(struct state_data *state);
