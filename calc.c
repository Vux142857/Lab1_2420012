#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

#define ANS_FILE "calc_ans.txt"
#define HIST_FILE "calc_history.txt"
#define MAX_HISTORY 5
#define MAX_INPUT 100
#define MAX_VALUE 1e9

void save_history(const char *entry) {
    FILE *file = fopen(HIST_FILE, "r");
    char history[MAX_HISTORY][MAX_INPUT];
    int count = 0;
    if (file) {
        while (count < MAX_HISTORY - 1 && fgets(history[count], sizeof(history[count]), file)) {
            history[count][strcspn(history[count], "\n")] = 0;
            count++;
        }
        fclose(file);
    }
    
    file = fopen(HIST_FILE, "w");
    if (!file) return;
    fprintf(file, "%s\n", entry);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", history[i]);
    }
    fclose(file);
}

void show_history() {
    FILE *file = fopen(HIST_FILE, "r");
    if (!file) {
        printf("No history available\n");
        return;
    }
    char line[MAX_INPUT];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

double get_ans() {
    FILE *file = fopen(ANS_FILE, "r");
    double ans = 0;
    if (file) {
        fscanf(file, "%lf", &ans);
        fclose(file);
    }
    return ans;
}

void save_ans(double ans) {
    FILE *file = fopen(ANS_FILE, "w");
    if (file) {
        fprintf(file, "%.2f", ans);
        fclose(file);
    }
}

void calculate(char *input) {
    double num1, num2, res = 0;
    char oper;
    if (strlen(input) >= MAX_INPUT - 1) {
        printf("INPUT ERROR: Input too long!\n");
        return;
    }
    if (sscanf(input, "%lf %c %lf", &num1, &oper, &num2) != 3) {
        printf("SYNTAX ERROR\n");
        return;
    }
    if (num1 > MAX_VALUE || num2 > MAX_VALUE) {
        printf("VALUE ERROR: Number too large!\n");
        return;
    }
    if (oper == '/' && num2 == 0) {
        printf("MATH ERROR\n");
        return;
    }
    switch (oper) {
        case '+': res = num1 + num2; break;
        case '-': res = num1 - num2; break;
        case 'x': res = num1 * num2; break;
        case '/': res = num1 / num2; break;
        case '%': res = (int)num1 % (int)num2; break;
        default: printf("SYNTAX ERROR\n"); return;
    }
    if (res > MAX_VALUE) {
        printf("VALUE ERROR: Result too large!\n");
        return;
    }
    printf("%.2f\n", res);
    save_ans(res);
    char entry[MAX_INPUT];
    snprintf(entry, sizeof(entry), "%.2f %c %.2f = %.2f", num1, oper, num2, res);
    save_history(entry);
}

int main() {
    char input[MAX_INPUT];
    while (1) {
        printf(">> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("INPUT ERROR: Failed to read input!\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) >= MAX_INPUT - 1) {
            printf("INPUT ERROR: Input too long!\n");
            continue;
        }
        if (strcmp(input, "EXIT") == 0) break;
        if (strcmp(input, "HIST") == 0) {
            show_history();
            continue;
        }
        calculate(input);
    }
    return 0;
}
