// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "util.h"

struct defined_state_map {
        const char *name;
        enum state_value value;
};

static const struct defined_state_map defined_state_list[STATE_COUNT] = {
        {"Open", OPEN},
        {"In Progress", IN_PROGRESS},
        {"Urgent", URGENT},
        {"Resolved", RESOLVED},
        {"Closed", CLOSED},
        {"Re Opened", RE_OPENED},
        {"Invalid", INVALID},
};

static const size_t VALID_STATE_COUNT = STATE_COUNT - 1;

struct todo_data *create_todo_data(char *id, uint64_t priority,
                                   struct state_data *state, char *subject,
                                   char *description)
{
        struct todo_data *ret = malloc(sizeof(struct state_data));

        if (ret == NULL) {
                return NULL;
        }

        ret->id = id;
        ret->priority = priority;
        ret->state = state;
        ret->subject = subject;
        ret->description = description;

        return ret;
}

struct state_data *create_custom_state_data(bool active, char *string)
{
        struct state_data *ret = malloc(sizeof(struct state_data));

        if (ret == NULL) {
                return NULL;
        }

        ret->active = active;
        ret->custom_state = true;
        ret->string = string;

        return ret;
}

struct state_data *create_defined_state_data(bool active,
                                             enum state_value value)
{
        struct state_data *ret = malloc(sizeof(struct state_data));

        if (ret == NULL) {
                return NULL;
        }

        ret->active = active;
        ret->custom_state = false;
        ret->value = value;

        return ret;
}

FILE *create_todo_data_file(char *id)
{
        return stdin;
}

enum state_value num_to_state_value(size_t num)
{
        if (num > VALID_STATE_COUNT) {
                return INVALID;
        }

        return defined_state_list[num].value;
}

void print_state_values(void)
{
        fputc('\n', stdout);

        // For this function to work properly all the state values must be
        // continuous.
        for (size_t value = 0; value < VALID_STATE_COUNT; ++value) {
                printf("%s: (%zu)\n", defined_state_list[value].name, value);
        }
}

const char *state_value_to_string(enum state_value value)
{
        for (size_t i = 0; i < STATE_COUNT; ++i) {
                if (defined_state_list[i].value == value) {
                        return defined_state_list[i].name;
                }
        }

        // An invalid value was passed
        return state_value_to_string(INVALID);
}

void destroy_todo_data(struct todo_data *todo)
{
        destroy_state_data(todo->state);

        free(todo);
}

void destroy_state_data(struct state_data *state)
{
        if (state->custom_state) {
                free(state->string);
        }

        free(state);
}
