#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_APPS 100

pid_t process_ids[MAX_APPS];
int process_ids_index = 0;

void start_applications(int argc, char *argv[]) {
    FILE *pid_file = fopen("running_processes.txt", "w");
    if (pid_file == NULL) {
        printf("Failed to open the PID file\n");
        return;
    }

    for (int i = 2; i < argc; i += 2) {
        char *app_name = argv[i];
        int num_instances = atoi(argv[i + 1]);

        for (int j = 0; j < num_instances; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[] = {app_name, NULL};
                execvp(args[0], args);
                exit(0);
            } else {
                fprintf(pid_file, "%d\n", pid);
            }
        }
    }

    fclose(pid_file);
}

void stop_applications() {
    FILE *pid_file = fopen("running_processes.txt", "r");
    if (pid_file == NULL) {
        printf("Failed to open the PID file\n");
        return;
    }

    pid_t pid;
    while (fscanf(pid_file, "%d", &pid) != EOF) {
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
    }

    fclose(pid_file);
}

void start_applications_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file %s\n", filename);
        return;
    }

    FILE *pid_file = fopen("running_processes.txt", "w");
    if (pid_file == NULL) {
        printf("Failed to open the PID file\n");
        return;
    }

    char app_name[100];
    int num_instances;
    while (fscanf(file, "%s %d", app_name, &num_instances) != EOF) {
        for (int j = 0; j < num_instances; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[] = {app_name, NULL};
                execvp(args[0], args);
                exit(0);
            } else {
                fprintf(pid_file, "%d\n", pid);
            }
        }
    }

    fclose(file);
    fclose(pid_file);
}

void stop_applications_from_file(char *filename) {
    FILE *pid_file = fopen("running_processes.txt", "r");
    if (pid_file == NULL) {
        printf("Failed to open the PID file\n");
        return;
    }

    pid_t pid;
    while (fscanf(pid_file, "%d", &pid) != EOF) {
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
    }

    fclose(pid_file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s -s <app1> <num1> <app2> <num2> ... <appN> <numN>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-o") == 0) {
    	start_applications(argc, argv);
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
    	start_applications_from_file(argv[2]);
    } else if (strcmp(argv[1], "-k") == 0) {
    if (argc == 3) {
        stop_applications_from_file(argv[2]);
    } else {
        stop_applications();
    }
} else {
    printf("Invalid option\n");
}


    return 0;
}
