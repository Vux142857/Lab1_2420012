#include "calc.h"

#define ANS_FILE getenv("HOME") ? strcat(getenv("HOME"), "/.calc_ans") : ".calc_ans"
#define HIST_FILE getenv("HOME") ? strcat(getenv("HOME"), "/.calc_history") : ".calc_history"

void save_ans(long double ans) {
    FILE* file = fopen(ANS_FILE, "w");
    if (file) {
        fprintf(file, "%.2Lf", ans);
        fclose(file);
    }
}

long double get_ans() {
    long double ans = 0.0;
    FILE* file = fopen(ANS_FILE, "r");
    if (file) {
        fscanf(file, "%Lf", &ans);
        fclose(file);
    } else {
        save_ans(ans);
    }
    return ans;
}

void save_history(const char* entry) {
    FILE* file = fopen(HIST_FILE, "r");
    char history[HIST_SIZE][MAX_LINE];
    int count = 0;

    if (file) {
        while (fgets(history[count], MAX_LINE, file) && count < HIST_SIZE) {
            history[count][strcspn(history[count], "\n")] = 0; // Loại bỏ newline
            count++;
        }
        fclose(file);
    }

    file = fopen(HIST_FILE, "w");
    if (file) {
        fprintf(file, "%s\n", entry);
        for (int i = 0; i < count && i < HIST_SIZE - 1; i++) {
            fprintf(file, "%s\n", history[i]);
        }
        fclose(file);
    }
}

void show_history() {
    FILE* file = fopen(HIST_FILE, "r");
    if (file) {
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("No history available\n");
    }
}

int is_valid_input(const char* input, long double* num1, char* oper, long double* num2) {
    char temp[MAX_LINE];
    strcpy(temp, input);
    char* token = strtok(temp, " ");
    if (!token) return 0;

    if (!strtok(NULL, " ")) {
        if (sscanf(token, "%Lf", num1) == 1) {
            *oper = '\0';
            *num2 = 0;
            return 1;
        } else if (strcmp(token, "ANS") == 0) {
            *num1 = get_ans();
            *oper = '\0';
            *num2 = 0;
            return 1;
        } else {
            strcpy(oper, "SYNTAX ERROR");
            return 0;
        }
    }

    if (strcmp(token, "ANS") == 0) {
        *num1 = get_ans();
    } else if (sscanf(token, "%Lf", num1) != 1) {
        return 0;
    }

    token = strtok(NULL, " ");
    if (!token || strlen(token) != 1 || strchr("+-x/%", *token) == NULL) {
        return 0;
    }
    *oper = *token;

    token = strtok(NULL, " ");
    if (!token || sscanf(token, "%Lf", num2) != 1) {
        return 0;
    }

    token = strtok(NULL, " ");
    if (token) return 0;

    return 1;
}

long double calculate_result(long double num1, char oper, long double num2, char* error_msg) {
    switch (oper) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case 'x':
            return num1 * num2;
        case '/':
            if (num2 == 0) {
                strcpy(error_msg, "MATH ERROR");
                return 0;
            }
            return num1 / num2;
        case '%':
            if ((long long)num2 == 0) {
                strcpy(error_msg, "MATH ERROR");
                return 0;
            }
            return (long long)num1 % (long long)num2;
        default:
            strcpy(error_msg, "SYNTAX ERROR");
            return 0;
    }
}

void clear_screen() {
    printf("\033[2J\033[H");
}