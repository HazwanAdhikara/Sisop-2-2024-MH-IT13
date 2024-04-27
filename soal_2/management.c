#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#define URL "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup"
#define OUTFILE "/home/zwaneee/sisop/modul2/soal2/library.zip"
#define LIBRARY_DIR "/home/zwaneee/sisop/modul2/soal2"
#define DECRYPT_DIR "/home/zwaneee/sisop/modul2/soal2/library"
#define BACKUP_DIR "/home/zwaneee/sisop/modul2/soal2/library/backup"
#define LOG_DIR "/home/zwaneee/sisop/modul2/soal2/history.log"

volatile sig_atomic_t mode = 0;

FILE *log_file;

time_t runtime;
struct tm *digitime;

void handle_default(int sig)
{
    mode = 0;
}

void handle_backup(int sig)
{
    mode = 1;
}

void handle_restore(int sig)
{
    mode = 2;
}

void handle_kill(int sig)
{
    exit(EXIT_SUCCESS);
}

void download_file()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        execl("/usr/bin/wget", "wget", "--content-disposition", URL, "-P", LIBRARY_DIR, NULL);
        syslog(LOG_ERR, "execl failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            syslog(LOG_ERR, "Download failed");
            exit(EXIT_FAILURE);
        }
    }
}

void unzip_file()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "Fork failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        execl("/usr/bin/unzip", "unzip", "-o", OUTFILE, "-d", LIBRARY_DIR, NULL);
        syslog(LOG_ERR, "execl failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            syslog(LOG_ERR, "Unzip failed");
            exit(EXIT_FAILURE);
        }
    }
}

void rot19(char *filename)
{
    for (int i = 0; filename[i]; i++)
    {
        char c = filename[i];
        if ((c >= 'a' && c <= 'z'))
        {
            c = ((c - 'a' + 7) % 26) + 'a';
        }
        else if ((c >= 'A' && c <= 'Z'))
        {
            c = ((c - 'A' + 7) % 26) + 'A';
        }
        filename[i] = c;
    }
}

void decrypt_names()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(DECRYPT_DIR);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && !isdigit(entry->d_name[0]))
        {
            char new_name[256];

            strcpy(new_name, entry->d_name);
            rot19(new_name);

            char old_path[256], new_path[256];

            sprintf(old_path, "%s/%s", DECRYPT_DIR, entry->d_name);
            sprintf(new_path, "%s/%s", DECRYPT_DIR, new_name);

            if (rename(old_path, new_path) != 0)
            {
                syslog(LOG_ERR, "Failed to rename file: %s", strerror(errno));
            }
            else
            {
                syslog(LOG_INFO, "Renamed %s to %s", entry->d_name, new_name);
            }
        }
    }
    closedir(dir);
}

void delete_files()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(DECRYPT_DIR);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strstr(entry->d_name, "d3Let3") != NULL)
        {
            char file_path[256];
            sprintf(file_path, "%s/%s", DECRYPT_DIR, entry->d_name);

            if (remove(file_path) != 0)
            {
                syslog(LOG_ERR, "Failed to delete file: %s", strerror(errno));
            }
            else
            {
                log_file = fopen(LOG_DIR, "a");
                if (log_file != NULL)
                {
                    fprintf(log_file, "zwaneee [%02d:%02d:%02d] - %s - Successfully deleted.\n", digitime->tm_hour, digitime->tm_min, digitime->tm_sec, entry->d_name);
                    fclose(log_file);
                }
                syslog(LOG_INFO, "Deleted %s", entry->d_name);
            }
        }
    }
    closedir(dir);
}

void rename_files()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(DECRYPT_DIR);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            char *filename = entry->d_name;
            char *extension = strrchr(filename, '.');
            if (extension != NULL)
            {
                if (strstr(filename, "m0V3") == NULL)
                {
                    char new_name[256];
                    if (strcmp(extension, ".ts") == 0)
                    {
                        strcpy(new_name, "helper.ts");
                    }
                    else if (strcmp(extension, ".py") == 0)
                    {
                        strcpy(new_name, "calculator.py");
                    }
                    else if (strcmp(extension, ".go") == 0)
                    {
                        strcpy(new_name, "server.go");
                    }
                    else if (strstr(filename, "r3N4mE") != NULL && strcmp(extension, ".ts") != 0 && strcmp(extension, ".py") != 0 && strcmp(extension, ".go") != 0)
                    {
                        strcpy(new_name, "renamed.file");
                    }
                    else
                    {
                        continue;
                    }

                    char old_path[256], new_path[256];
                    sprintf(old_path, "%s/%s", DECRYPT_DIR, filename);
                    sprintf(new_path, "%s/%s", DECRYPT_DIR, new_name);

                    if (rename(old_path, new_path) != 0)
                    {
                        syslog(LOG_ERR, "Failed to rename file: %s", strerror(errno));
                    }
                    else
                    {
                        log_file = fopen(LOG_DIR, "a");
                        if (log_file != NULL)
                        {
                            fprintf(log_file, "zwaneee [%02d:%02d:%02d] - %s - Successfully renamed.\n", digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename);
                            fclose(log_file);
                        }
                        syslog(LOG_INFO, "Renamed %s to %s", filename, new_name);
                    }
                }
            }
        }
    }
    closedir(dir);
}

void move_to_backup()
{
    DIR *dir;
    struct dirent *entry;
    char src_path[256], dest_path[256];

    struct stat st = {0};
    if (stat(BACKUP_DIR, &st) == -1)
    {
        if (mkdir(BACKUP_DIR, 0755) != 0)
        {
            syslog(LOG_ERR, "Failed to create backup directory: %s", strerror(errno));
            return;
        }
    }

    dir = opendir(DECRYPT_DIR);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open directory %s: %s", DECRYPT_DIR, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strstr(entry->d_name, "m0V3") != NULL)
        {
            snprintf(src_path, sizeof(src_path), "%s/%s", DECRYPT_DIR, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", BACKUP_DIR, entry->d_name);

            if (rename(src_path, dest_path) != 0)
            {
                syslog(LOG_ERR, "Failed to move file %s to backup: %s", entry->d_name, strerror(errno));
            }
            else
            {
                log_file = fopen(LOG_DIR, "a");
                if (log_file != NULL)
                {
                    fprintf(log_file, "zwaneee [%02d:%02d:%02d] - %s - Successfully moved to backup.\n", digitime->tm_hour, digitime->tm_min, digitime->tm_sec, entry->d_name);
                    fclose(log_file);
                }
                syslog(LOG_INFO, "Moved %s to backup directory", entry->d_name);
            }
        }
    }
    closedir(dir);
}

void restore_from_backup()
{
    DIR *dir;
    struct dirent *entry;
    char src_path[256], dest_path[256];

    dir = opendir(BACKUP_DIR);
    if (dir == NULL)
    {
        syslog(LOG_ERR, "Failed to open backup directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strstr(entry->d_name, "m0V3") != NULL)
        {
            snprintf(src_path, sizeof(src_path), "%s/%s", BACKUP_DIR, entry->d_name);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", DECRYPT_DIR, entry->d_name);

            if (rename(src_path, dest_path) != 0)
            {
                syslog(LOG_ERR, "Failed to restore file %s: %s", entry->d_name, strerror(errno));
            }
            else
            {
                log_file = fopen(LOG_DIR, "a");
                if (log_file != NULL)
                {
                    fprintf(log_file, "zwaneee [%02d:%02d:%02d] - %s - Successfully restored from backup.\n", digitime->tm_hour, digitime->tm_min, digitime->tm_sec, entry->d_name);
                    fclose(log_file);
                }
                syslog(LOG_INFO, "Restored %s to library directory", entry->d_name);
            }
        }
    }
    closedir(dir);
}

void default_mode()
{
    download_file();
    unzip_file();
    decrypt_names();
    delete_files();
    rename_files();
}

int main(int argc, char *argv[])
{
    runtime = time(NULL);
    digitime = localtime(&runtime);
    signal(34, handle_default);
    signal(SIGUSR1, handle_backup);
    signal(SIGUSR2, handle_restore);
    signal(SIGTERM, handle_kill);
    if (argc > 2 && strcmp(argv[1], "-m") == 0)
    {
        if (strcmp(argv[2], "backup") == 0)
        {
            // move_to_backup();
            mode = 1;
        }
        else if (strcmp(argv[2], "restore") == 0)
        {
            // restore_from_backup();
            mode = 2;
        }
    }
    else
    {
        // Usual daemon functionality
        pid_t pid, sid;

        pid = fork();

        if (pid < 0)
        {
            exit(EXIT_FAILURE);
        }

        if (pid > 0)
        {
            exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();
        if (sid < 0)
        {
            exit(EXIT_FAILURE);
        }

        if ((chdir("/")) < 0)
        {
            exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        openlog("daemon_log", LOG_PID, LOG_DAEMON);

        while (1)
        {
            switch (mode)
            {
            case 0:
                default_mode();
                break;
            case 1:
                move_to_backup();
                break;
            case 2:
                restore_from_backup();
                break;
            }
            sleep(3600); // Sleep for one hour before repeating the cycle
        }

        closelog();
        return 0;
    }
}
