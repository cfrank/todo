// Copyright 2018 Chris Frank
// Licensed under BSD-3-Clause
// Refer to the license.txt file included in the root of the project

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
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
        struct todo_data *ret = malloc(sizeof(struct todo_data));

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
        ret->is_custom = true;
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
        ret->is_custom = false;
        ret->value = value;

        return ret;
}

void save_todo_data_to_file(const struct todo_data *todo)
{
        print_user_message(todo->id);

        char *data_path = create_file_path(TODO_DIR_NAME, todo->id);

        FILE *data_file = open_file(data_path, "a+");

        fprintf(data_file, "%d\n", todo->state->is_custom);
        fprintf(data_file, "%s;%" PRIu64 ";", todo->id, todo->priority);

        if (todo->state->is_custom) {
                fprintf(data_file, "%s;", todo->state->string);
        } else {
                fprintf(data_file, "%d;", todo->state->value);
        }

        fprintf(data_file, "%s;%s", todo->subject, todo->description);

        fclose(data_file);

        free(data_path);
}

struct todo_data *read_todo_from_file(FILE *todo_file)
{
        ssize_t result = 0;
        size_t buffer_size = 256;
        char *buffer = malloc(buffer_size);
        char *segment;

        // Todo Properties
        struct todo_data *todo = NULL;
        bool is_custom;
        char *id;
        uint64_t priority;
        struct state_data *state = NULL;
        char *subject;
        char *description;

        result = read_line_from_file(&buffer, &buffer_size, todo_file, false);

        if (result <= 0) {
                goto return_err;
        }

        is_custom = (bool)strtol(buffer, NULL, 10);

        result = read_line_from_file(&buffer, &buffer_size, todo_file, false);

        if (result <= 0) {
                goto return_err;
        }

        // Get id
        segment = strtok(buffer, ";");

        if (segment == NULL) {
                goto return_err;
        }

        id = strdup(buffer);

        // Get priority
        segment = strtok(NULL, ";");

        if (segment == NULL) {
                goto return_err;
        }

        priority = (uint64_t)strtol(segment, NULL, 10);

        // Get state data
        segment = strtok(NULL, ";");

        if (segment == NULL) {
                goto return_err;
        }

        if (is_custom) {
                char *state_string = strdup(segment);
                state = create_custom_state_data(true, state_string);

                state->is_custom = is_custom;
        } else {
                size_t value = (size_t)strtol(segment, NULL, 10);
                enum state_value defined_state = num_to_state_value(value);

                state = create_defined_state_data(true, defined_state);

                state->is_custom = is_custom;
        }

        // Get the subject
        segment = strtok(NULL, ";");

        if (segment == NULL) {
                goto return_err;
        }

        subject = strdup(segment);

        // Get the description
        segment = strtok(NULL, ";");

        if (segment == NULL) {
                goto return_err;
        }

        description = strdup(segment);

        todo = create_todo_data(id, priority, state, subject, description);

        free(buffer);

        return todo;

return_err:
        free(buffer);

        die("Failed to read todo data from file");

        return NULL;
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
                print_user_message(
                        "%s: (%zu)\n", defined_state_list[value].name, value);
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
        if (todo->state->is_custom) {
                free(todo->state->string);
        }

        free(todo->id);
        free(todo->state);
        free(todo->subject);
        free(todo->description);
        free(todo);
}

