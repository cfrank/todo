// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <stdlib.h>

#include "data.h"

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

struct state_data *create_custom_state_data(bool active, const char *string)
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
