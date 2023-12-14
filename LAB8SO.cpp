#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 25

void handleError(const char* message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void printChildInfo(int parentPID, int childPID, const char* arg, int exitCode) {
    printf("Dziecko: PID = %d, PID zakończonego dziecka = %d, argument = %s, kod zakończenia = %d\n",
        parentPID, childPID, arg, exitCode);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        handleError("Błąd: Program wymaga jednego argumentu.");
    }

    size_t argLength = strlen(argv[1]);
    if (argLength > MAX_LENGTH || argLength == 0) {
        handleError("Błąd: Argument musi mieć długość od 1 do 25 znaków.");
    }

    for (size_t i = 0; i < argLength; ++i) {
        if (!isdigit(argv[1][i])) {
            handleError("Błąd: Argument powinien zawierać tylko cyfry dziesiętne.");
        }
    }

    if (argLength == 1) {
        int result = argv[1][0] - '0';
        printf("Kod zakończenia: %d\n", result);
        return result;
    }

    int halfLength = (argLength + 1) / 2;
    char firstHalf[MAX_LENGTH], secondHalf[MAX_LENGTH];

    strncpy(firstHalf, argv[1], halfLength);
    firstHalf[halfLength] = '\0';
    strcpy(secondHalf, argv[1] + halfLength);

    // Wyświetlenie informacji o dziecku 1
    int child1PID = fork();
    if (child1PID == -1) {
        handleError("Błąd podczas tworzenia procesu potomnego 1.");
    }
    else if (child1PID == 0) {
        printf("Dziecko 1: PID = %d, argument = %s\n", getpid(), firstHalf);
        int exitCode1 = atoi(argv[0]);
        printChildInfo(getppid(), getpid(), firstHalf, exitCode1);
        exit(exitCode1);
    }

    // Wyświetlenie informacji o dziecku 2
    int child2PID = fork();
    if (child2PID == -1) {
        handleError("Błąd podczas tworzenia procesu potomnego 2.");
    }
    else if (child2PID == 0) {
        printf("Dziecko 2: PID = %d, argument = %s\n", getpid(), secondHalf);
        int exitCode2 = atoi(argv[0]);
        printChildInfo(getppid(), getpid(), secondHalf, exitCode2);
        exit(exitCode2);
    }

    int status1, status2;
    waitpid(child1PID, &status1, 0);
    waitpid(child2PID, &status2, 0);

    return WEXITSTATUS(status1) + WEXITSTATUS(status2);
}
