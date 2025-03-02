#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ANS_FILE "/tmp/.calc_ans"
#define HIST_FILE "/tmp/.calc_history"
#define MAX_VALUE 1000000000
#define INPUT_SIZE 100

double ANS = 0;

void save_history(const char *entry) {
    FILE *file = fopen(HIST_FILE, "r");
    char history[5][INPUT_SIZE] = {0};
    int count = 0;

    if (file) {
        while (count < 5 && fgets(history[count], INPUT_SIZE, file)) {
            count++;
        }
        fclose(file);
    }

    file = fopen(HIST_FILE, "w");
    if (file) {
        fprintf(file, "%s\n", entry);
        for (int i = 0; i < count; i++) {
            fprintf(file, "%s", history[i]);
        }
        fclose(file);
    }
}

void show_history() {
    FILE *file = fopen(HIST_FILE, "r");
    if (!file) {
        printf("No history available\n");
        return;
    }
    char line[INPUT_SIZE];
    while (fgets(line, INPUT_SIZE, file)) {
        printf("%s", line);
    }
    fclose(file);
}

int is_number(const char *s) {
    while (*s) {
        if (!isdigit(*s) && *s != '.' && *s != '-') {
            return 0;
        }
        s++;
    }
    return 1;
}

void calculate(const char *input) {
    double num1, num2, result = 0;
    char oper;

    if (sscanf(input, "%lf %c %lf", &num1, &oper, &num2) != 3) {
        printf("SYNTAX ERROR\n");
        return;
    }

    if (num1 > MAX_VALUE || num2 > MAX_VALUE) {
        printf("VALUE ERROR: Number too large!\n");
        return;
    }

    switch (oper) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case 'x': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                printf("MATH ERROR: Division by zero!\n");
                return;
            }
            result = num1 / num2;
            break;
        case '%':
            if ((int)num2 == 0) {
                printf("MATH ERROR: Modulo by zero!\n");
                return;
            }
            result = (int)num1 % (int)num2;
            break;
        default:
            printf("SYNTAX ERROR\n");
            return;
    }

    if (result > MAX_VALUE) {
        printf("VALUE ERROR: Result too large!\n");
        return;
    }

    ANS = result;
    printf("= %.2lf\n", result);

    char history_entry[INPUT_SIZE];
    snprintf(history_entry, sizeof(history_entry), "%.2lf %c %.2lf = %.2lf", num1, oper, num2, result);
    save_history(history_entry);
}

int main() {
    char input[INPUT_SIZE];

    while (1) {
        printf(">> ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "EXIT") == 0) {
            break;
        } else if (strcmp(input, "HIST") == 0) {
            show_history();
        } else {
            calculate(input);
        }
    }

    return 0;
}
