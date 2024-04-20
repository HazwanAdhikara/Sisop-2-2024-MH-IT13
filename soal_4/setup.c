#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_APPS 100
#define MAX_NAME_LENGTH 50

struct App {
    char name[MAX_NAME_LENGTH];
    int numWindows;
};

void openApps(struct App apps[], int numApps);
void saveDataToFile(const struct App apps[], int numApps);
void readConfigFile(const char *filename, struct App apps[], int *numApps);
void killAllApps(struct App apps[], int numApps);

int main(int argc, char *argv[]) {
    struct App apps[MAX_APPS];
    int numApps = 0;

    if (argc < 2) {
        printf("Usage: %s -o <app1> <num1> <app2> <num2> ... <appN> <numN>\n", argv[0]);
        printf("       %s -f <config_file>\n", argv[0]);
        printf("       %s -k\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0) {
        if (argc < 4 || argc % 2 != 0) {
            printf("Invalid number of arguments\n");
            return 1;
        }
        numApps = (argc - 2) / 2;
        for (int i = 0; i < numApps; i++) {
            strcpy(apps[i].name, argv[i * 2 + 2]);
            apps[i].numWindows = atoi(argv[i * 2 + 3]);
        }
        openApps(apps, numApps);
        saveDataToFile(apps, numApps);
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        readConfigFile(argv[2], apps, &numApps);
    } else if (strcmp(argv[1], "-k") == 0 && argc == 2) {
        killAllApps(apps, numApps);
    } else {
        printf("Invalid option\n");
    }

    return 0;
}

void openApps(struct App apps[], int numApps) {
    for (int i = 0; i < numApps; i++) {
        for (int j = 0; j < apps[i].numWindows; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                execlp(apps[i].name, apps[i].name, NULL);
                exit(0);
            }
        }
    }
}

void saveDataToFile(const struct App apps[], int numApps) {
    FILE *file = fopen("app_data.txt", "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    fprintf(file, "%d\n", numApps);
    for (int i = 0; i < numApps; i++) {
        fprintf(file, "%s %d\n", apps[i].name, apps[i].numWindows);
    }
    fclose(file);
}

void readConfigFile(const char *filename, struct App apps[], int *numApps) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }

    while (fscanf(file, "%s %d", apps[*numApps].name, &apps[*numApps].numWindows) != EOF) {
        (*numApps)++;
    }

    fclose(file);

    openApps(apps, *numApps);
}

void killAllApps(struct App apps[], int numApps) {
    for (int i = 0; i < numApps; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp("pkill", "pkill", "-f", apps[i].name, NULL);
            exit(0);
        } else if (pid < 0) {
            printf("Failed to fork\n");
        } else {
            waitpid(pid, NULL, 0); // Menunggu proses anak selesai
        }
    }
}

