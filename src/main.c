/**
 * @file main.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Entry point of a program
 */

#include "main.h"

int main(int argc, char **argv) {
    argv_p = malloc(sizeof(t_argv));

    signal(SIGINT, sigintHandler);
    if (argvParse(argc, argv, argv_p)) {
        stop(errno);
    }

    if (client(argv_p)) {
        stop(errno);
    }

    free(argv_p);    
    return 0;
}

void sigintHandler(int signal) {
    printf("\nProgram ended with %d signal\n", signal);
    free(argv_p);
    exit(0);
}

void stop(int exit_code) {
    free(argv_p);
    exit(exit_code);
}