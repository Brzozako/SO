#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LENGTH 25

void error(char *msg) {
    perror(msg);
    exit(1);
}

void process(char *arg) {
    if (strlen(arg) == 1) {
        int value = arg[0] - '0';
        exit(value);
    } else {
        int len = strlen(arg);
        int half = len / 2;
        int first_half_len = half;
        int second_half_len = len - half;

        char first_half[MAX_LENGTH];
        char second_half[MAX_LENGTH];

        strncpy(first_half, arg, first_half_len);
        first_half[first_half_len] = '\0';

        strncpy(second_half, arg + first_half_len, second_half_len);
        second_half[second_half_len] = '\0';

        pid_t child1, child2;
        child1 = fork();

        if (child1 < 0) {
            error("Fork failed");
        } else if (child1 == 0) {
            execl("./lab8", "./lab8", first_half, NULL);
            error("Exec failed for child 1");
        } else {
            child2 = fork();
            if (child2 < 0) {
                error("Fork failed");
            } else if (child2 == 0) {
                execl("./lab8", "./lab8", second_half, NULL);
                error("Exec failed for child 2");
            } else {
                int status;
                waitpid(child1, &status, 0);
                printf("%d %d %s %d\n", getpid(), child1, first_half, WEXITSTATUS(status));

                waitpid(child2, &status, 0);
                printf("%d %d %s %d\n", getpid(), child2, second_half, WEXITSTATUS(status));

                exit(WEXITSTATUS(status) + WEXITSTATUS(status));
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 || strlen(argv[1]) > MAX_LENGTH) {
        error("Invalid arguments");
    }

    for (int i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            error("Argument contains non-digit characters");
        }
    }

    process(argv[1]);
    return 0;
}

