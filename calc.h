#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 256
#define HIST_SIZE 5

void save_ans(double ans);
double get_ans();
void save_history(const char* entry);
void show_history();
int is_valid_input(const char* input, double* num1, char* oper, double* num2);
double calculate_result(double num1, char oper, double num2, char* error_msg);
void clear_screen();

#endif