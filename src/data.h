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
        INVALID,
};

struct state_data {
        bool active;
        bool custom_state;
        union {
                enum state_value value;
                char *string;
        };
};

struct todo_data {
        uint64_t id;
        uint64_t priority;
        struct state_data *state;
        char *subject;
        char *description;
};

struct todo_data *create_todo_data(uint64_t id, uint64_t priority,
                                   struct state_data *state, char *subject,
                                   char *description);
struct state_data *create_custom_state_data(bool active, char *string);
struct state_data *create_defined_state_data(bool active,
                                             enum state_value value);
enum state_value num_to_state_value(size_t num);
void print_state_values(void);
const char *state_value_to_string(enum state_value value);

void destroy_todo_data(struct todo_data *todo);
void destroy_state_data(struct state_data *state);
