#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_PROCESS 1024
#define LOG_FILE_PREFIX "kali_andrey.log"

pid_t monitor_process_id = 0;
pid_t crack_process_id = 0;
char user[64];
FILE *fp_log;

void log_process(char *status, pid_t pid, char *process_name) {
    time_t current_time;
    char *c_time_string;

    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    c_time_string[strlen(c_time_string) - 1] = '\0';

    fprintf(fp_log, "[%s]-[%d]-[%s]-%s\n", c_time_string, pid, process_name, status);
    fflush(fp_log);
}

void kill_process(int sig) {
    DIR *dir;
    struct dirent *entry;
    char path[256];
    pid_t pid;
    char command[256];

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        pid = atoi(entry->d_name);
        sprintf(path, "/proc/%s/cmdline", entry->d_name);
        FILE *fp = fopen(path, "r");
        if (fp) {
            fgets(command, sizeof(command), fp);
            log_process("JALAN", pid, command); // Log semua proses
            fclose(fp);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <user>\n", argv[0]);
        printf("Options:\n");
        printf("  -m: Monitor user processes\n");
        printf("  -s: Stop monitoring user processes\n");
        printf("  -c: Crack user processes\n");
        printf("  -a: Stop cracking user processes\n");
        return 1;
    }

    strcpy(user, argv[2]);

    char log_file[256];
    sprintf(log_file, "%s", LOG_FILE_PREFIX);
    fp_log = fopen(log_file, "a");
    if (fp_log == NULL) {
        printf("Gagal membuka file log: %s\n", log_file);
        return 1;
    }

    if (strcmp(argv[1], "-m") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            while (1) {
                DIR *dir;
                struct dirent *entry;
                char path[256];
                pid_t pid;
                char command[256];

                dir = opendir("/proc");
                if (dir == NULL) {
                    perror("opendir");
                    return 1;
                }

                while ((entry = readdir(dir)) != NULL) {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                        continue;

                    pid = atoi(entry->d_name);
                    sprintf(path, "/proc/%s/cmdline", entry->d_name);
                    FILE *fp = fopen(path, "r");
                    if (fp) {
                        fgets(command, sizeof(command), fp);
                        if (strstr(command, user) != NULL) {
                            log_process("JALAN", pid, command);
                        }
                        fclose(fp);
                    }
                }

                closedir(dir);
                sleep(1);
            }
        } else {
            monitor_process_id = pid;
            printf("Monitoring user %s processes\n", user);
        }
    } else if (strcmp(argv[1], "-s") == 0) {
        if (monitor_process_id != 0) {
            kill(monitor_process_id, SIGTERM);
            int status;
            waitpid(monitor_process_id, &status, 0);
            monitor_process_id = 0;
            printf("Stopped monitoring user %s processes\n", user);
        } else {
            printf("No monitoring process running for user %s\n", user);
        }
    } else if (strcmp(argv[1], "-c") == 0) {
        crack_process_id = fork();
        if (crack_process_id == 0) {
            struct sigaction sa;
            sa.sa_handler = kill_process;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGALRM, &sa, NULL);

            while (1) {
                alarm(1);
                pause();
            }
        } else {
            printf("Cracking user %s processes\n", user);
        }
    } else if (strcmp(argv[1], "-a") == 0) {
        if (crack_process_id != 0) {
            kill(crack_process_id, SIGTERM);
            int status;
            waitpid(crack_process_id, &status, 0);
            crack_process_id = 0;
            printf("Stopped cracking user %s processes\n", user);
        } else {
            printf("No cracking process running for user %s\n", user);
        }
    } else {
        printf("Invalid option: %s\n", argv[1]);
        return 1;
    }

    fclose(fp_log);
    return 0;
}
