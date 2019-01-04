// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdlib.h>

#include "data.h"
#include "util.h"

struct todo_data *create_todo_data(uint64_t id, uint64_t priority,
                                   struct state_data *state,
                                   const char *subject, const char *description)
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

enum state_value num_to_state_value(size_t num)
{
        switch (num) {
        case 0:
                return OPEN;
        case 1:
                return IN_PROGRESS;
        case 2:
                return URGENT;
        case 3:
                return RESOLVED;
        case 4:
                return CLOSED;
        case 5:
                return RE_OPENED;
        default:
                return INVALID;
        }
}

void print_state_values(void)
{
        fputc('\n', stdout);

        // TODO: Might not be best solution since it assumes state
        // values will always be continuous, but that seems like a sane
        // idea right now
        //
        // Also if a value is added after RE_OPENED then this breaks...

        for (size_t value = OPEN; value <= RE_OPENED; ++value) {
                printf("%s: (%zu)\n", state_value_to_string(value), value);
        }
}

const char *state_value_to_string(enum state_value value)
{
        switch (value) {
        case OPEN:
                return "Open";
        case IN_PROGRESS:
                return "In Progress";
        case URGENT:
                return "Urgent";
        case RESOLVED:
                return "Resolved";
        case CLOSED:
                return "Closed";
        case RE_OPENED:
                return "Reopened";
        default:
                return "";
        }
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
