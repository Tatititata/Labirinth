#ifndef TEST_ALL_H
#define TEST_ALL_H
#define TEMP_FILENAME "temp_input.txt"

#include <check.h>

#include "../model/model.h"

void write_to_temp_file(const char* content);

Suite* parser_suite();
Suite* maze_suite();
Suite* q_agent_suite();
Suite* cave_suite();
#endif