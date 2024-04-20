#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH_LENGTH 256
#define MAX_BUFFER_SIZE 1024

// Fungsi untuk mereplace string dalam sebuah string
void replaceString(char *str, const char *old, const char *new) {
    char *pos, temp[MAX_BUFFER_SIZE];
    int index = 0;
    int oldLen = strlen(old);
    int newLen = strlen(new);

    // Loop untuk mencari dan mereplace string lama dengan string baru
    while ((pos = strstr(str, old)) != NULL) {
        strcpy(temp, str);
        index = pos - str;
        str[index] = '\0';
        strcat(str, new);
        strcat(str, temp + index + oldLen);
    }
}

// Fungsi untuk mencatat penggantian string dalam sebuah file
void logReplacement(const char *filePath) {
    FILE *logFile;
    time_t now;
    struct tm *timestamp;
    char logEntry[MAX_BUFFER_SIZE];

    // Mendapatkan waktu saat ini
    time(&now);
    timestamp = localtime(&now);

    // Membuat log entry berdasarkan waktu dan path file
    sprintf(logEntry, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n",
            timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_year + 1900,
            timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, filePath);

    // Menulis log entry ke dalam file virus.log
    logFile = fopen("virus.log", "a");
    if (logFile != NULL) {
        fputs(logEntry, logFile);
        fclose(logFile);
    }
}

// Fungsi untuk menjalankan program sebagai daemon
void runDaemon(const char *folderPath) {
    pid_t pid, sid;

    // Membuat proses baru menggunakan fork()
    pid = fork();

    // Handle jika gagal membuat proses baru
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // Exit parent process jika berhasil membuat child process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Mengubah file permission dengan umask
    umask(0);

    // Membuat session baru dengan setsid()
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // Mengubah working directory ke folder yang diberikan
    if ((chdir(folderPath)) < 0) {
        exit(EXIT_FAILURE);
    }

    // Menutup file descriptor standar
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Loop utama untuk menjalankan program sebagai daemon
    while (1) {
        FILE *file;
        char filePath[MAX_PATH_LENGTH];
        char buffer[MAX_BUFFER_SIZE];

        // Menggabungkan path folder dan nama file contoh.txt
        sprintf(filePath, "%s/contoh.txt", folderPath);

        // Membuka file contoh.txt untuk dibaca dan ditulis
        file = fopen(filePath, "r+");
        if (file != NULL) {
            // Loop untuk membaca dan mereplace string dalam file
            while (fgets(buffer, MAX_BUFFER_SIZE, file) != NULL) {
                replaceString(buffer, "m4LwAr3", "[MALWARE]");
                replaceString(buffer, "5pYw4R3", "[SPYWARE]");
                replaceString(buffer, "R4nS0mWaR3", "[RANSOMWARE]");
                fseek(file, -strlen(buffer), SEEK_CUR);
                fputs(buffer, file);
            }
            fclose(file);
            // Mencatat penggantian string ke dalam file virus.log
            logReplacement(filePath);
        }

        // Delay selama 15 detik sebelum melakukan iterasi selanjutnya
        sleep(15);
    }
}

// Fungsi utama
int main(int argc, char *argv[]) {
    // Memeriksa apakah jumlah argumen sesuai
    if (argc != 2) {
        printf("Usage: %s <folder_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Menjalankan program sebagai daemon dengan folder path yang diberikan
    runDaemon(argv[1]);

    return 0;
}
