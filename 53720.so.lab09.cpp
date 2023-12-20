#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_LENGTH 25

void error(const char* msg) {
    perror(msg);
    ExitProcess(1);
}

void process(const char* arg) {
    if (strlen(arg) == 1) {
        int value = arg[0] - '0';
        ExitProcess(value);
    }
    else {
        size_t len = strlen(arg);
        int half = (int)(len / 2);
        int first_half_len = half;
        int second_half_len = (int)(len - half);

        char first_half[MAX_LENGTH];
        char second_half[MAX_LENGTH];

        strncpy_s(first_half, MAX_LENGTH, arg, first_half_len);
        first_half[first_half_len] = '\0';

        strncpy_s(second_half, MAX_LENGTH, arg + first_half_len, second_half_len);
        second_half[second_half_len] = '\0';

        STARTUPINFOA si1 = { 0 }, si2 = { 0 };
        PROCESS_INFORMATION pi1, pi2;

        if (!CreateProcessA(
            NULL, (LPSTR)"./53720.so.lab09", NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
            error("CreateProcess failed for child 1");
        }
        else {
            if (!CreateProcessA(
                NULL, (LPSTR)"./53720.so.lab09", NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
                error("CreateProcess failed for child 2");
            }
            else {
                WaitForSingleObject(pi1.hProcess, INFINITE);
                DWORD exitCode1;
                GetExitCodeProcess(pi1.hProcess, &exitCode1);
                printf("%d %d %s %d\n", GetCurrentProcessId(), pi1.dwProcessId, first_half, exitCode1);

                WaitForSingleObject(pi2.hProcess, INFINITE);
                DWORD exitCode2;
                GetExitCodeProcess(pi2.hProcess, &exitCode2);
                printf("%d %d %s %d\n", GetCurrentProcessId(), pi2.dwProcessId, second_half, exitCode2);

                ExitProcess((int)(exitCode1 + exitCode2));
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2 || strlen(argv[1]) > MAX_LENGTH) {
        error("Invalid arguments");
    }

    for (size_t i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            error("Argument contains non-digit characters");
        }
    }

    process(argv[1]);
    return 0;
}
