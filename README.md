# PRAKTIKUM SISOP-2-2024-MH-IT13

**KELOMPOK IT13**
| Nama | NRP |
|---------------------------|------------|
|Muhamad Arrayyan | 5027231014 |
|Hazwan Adhikara Nasution | 5027231017 |
|Muhammad Andrean Rizq Prasetio | 5027231052 |

## Pengantar

Laporan resmi ini dibuat terkait dengan praktikum modul 2 sistem operasi yang telah dilaksanakan pada tanggal 22 april 2024 hingga tanggal 27 april 2024. Praktikum modul 2 terdiri dari 4 soal yang dikerjakan oleh kelompok praktikan yang terdiri dari 3 orang selama waktu tertentu.

Kelompok IT13 melakukan pengerjaan modul 2 ini dengan pembagian sebagai berikut:

- Soal 1 dikerjakan oleh Muhammad Andrean Rizq Prasetio
- Soal 2 dikerjakan oleh Hazwan Adhikara Nasution
- Soal 3 dikerjakan oleh Muhammad Andrean Rizq Prasetio
- Soal 4 dikerjakan oleh Muhamad Arrayyan

Sehingga dengan demikian, Pembagian bobot pengerjaan soal menjadi (Rayyan 30%, Hazwan 30%, Andre 40%).

Kelompok IT13 juga telah menyelesaikan tugas praktikum modul 1 yang telah diberikan dan telah melakukan demonstrasi kepada Asisten lab. Dari hasil praktikum yang telah dilakukan sebelumnya, maka diperoleh hasil sebagaimana yang dituliskan pada setiap bab di bawah ini.

## Ketentuan

Struktur Repository Seperti Berikut:

```bash
—soal_1:
  	— virus.sh

—soal_2:
  	— management.c

—soal_3:
  	— admin.c

—soal_4:
	— setup.c
```

---

### **`Soal 1`**

`>Andre`

### > Isi Soal

Gavriel adalah seorang cyber security enthusiast. Suatu hari, ia dikontrak oleh sebuah perusahaan ternama untuk membuat sebuah program yang cukup rumit dengan bayaran jutaan rupiah. Karena tergoda dengan nominal bayaran tersebut, Gavriel langsung menerima tawaran tersebut. Setelah mencoba membuat dan mengembangkan program tersebut selama seminggu, laptop yang digunakan Gavriel mengalami overheat dan mati total sehingga harus dilarikan ke tukang servis terdekat. Karena keterbatasan waktu dalam pembuatan program dan tidak ingin mengecewakan perusahaan, Gavriel meminta bantuan kalian untuk membuat program tersebut dengan ketentuan sebagai berikut:

A. Program dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi

B. Program tersebut berfungsi untuk mereplace string dengan ketentuan sebagai berikut:
I. String m4LwAr3 direplace dengan string [MALWARE]
II. String 5pYw4R3 direplace dengan string [SPYWARE]
III. String R4nS0mWaR3 direplace dengan string [RANSOMWARE]

C. Program harus berjalan secara daemon, dan tidak diperbolehkan menggunakan command system() dalam pembuatan program

D. Program akan secara terus menerus berjalan di background dengan jeda 15 detik
Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file 

E. Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama virus.log dengan format: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!

Contoh penggunaan: ./virus /home/user/virus

Contoh isi file sebelum program dijalankan:
pU=-JWQ$5$)D-[??%AVh]$cB6bm4LwAr3jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj]5pYw4R3{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1PR4nS0mWaR3{]S/{w?*

Contoh isi file setelah setelah program dijalankan:
pU=-JWQ$5$)D-[??%AVh]$cB6b[MALWARE]jEQC2p3R{HV]=-AUaxj:Qe+h
!aNX,i:!z3W=2;.tHc3&S+}6F)CFf%tfZLP1*w5m1PAzZJUux(
Pd&f8$F5=E?@#[6jd{TJKj][SPYWARE]{KK1?hz384$ge@iba5GAj$gqB41
#C&&a}M9C#f64Eb.?%c)dGbCvJXtU[?SE4h]BY4e1P[RANSOMWARE]{]S/{w?*

#### > Penyelesaian
```bash
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

void replaceString(char *str, const char *old, const char *new) {
    char *pos, temp[MAX_BUFFER_SIZE];
    int index = 0;
    int oldLen = strlen(old);
    int newLen = strlen(new);

    while ((pos = strstr(str, old)) != NULL) {
        strcpy(temp, str);
        index = pos - str;
        str[index] = '\0';
        strcat(str, new);
        strcat(str, temp + index + oldLen);
    }
}

void logReplacement(const char *filePath) {
    FILE *logFile;
    time_t now;
    struct tm *timestamp;
    char logEntry[MAX_BUFFER_SIZE];

    time(&now);
    timestamp = localtime(&now);

    sprintf(logEntry, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n",
            timestamp->tm_mday, timestamp->tm_mon + 1, timestamp->tm_year + 1900,
            timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, filePath);

    logFile = fopen("virus.log", "a");
    if (logFile != NULL) {
        fputs(logEntry, logFile);
        fclose(logFile);
    }
}

void runDaemon(const char *folderPath) {
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir(folderPath)) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        FILE *file;
        char filePath[MAX_PATH_LENGTH];
        char buffer[MAX_BUFFER_SIZE];

        // Menggunakan sprintf untuk menggabungkan path folder dan nama file contoh.txt
        sprintf(filePath, "%s/contoh.txt", folderPath);

        file = fopen(filePath, "r+");
        if (file != NULL) {
            while (fgets(buffer, MAX_BUFFER_SIZE, file) != NULL) {
                replaceString(buffer, "m4LwAr3", "[MALWARE]");
                replaceString(buffer, "5pYw4R3", "[SPYWARE]");
                replaceString(buffer, "R4nS0mWaR3", "[RANSOMWARE]");
                fseek(file, -strlen(buffer), SEEK_CUR);
                fputs(buffer, file);
            }
            fclose(file);
            logReplacement(filePath);
        }

        sleep(15); // Jeda selama 15 detik
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <folder_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    runDaemon(argv[1]);

    return 0;
}

```

#### > Penjelasan

#### > Dokumentasi

#### > Revisi

---

### **`Soal 2`**

`>Hazwan`

### > Isi Soal

Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.

A. Atasannya meminta agar program tersebut dapat berjalan secara daemon dan dapat mengunduh serta melakukan unzip terhadap file `berikut.` Atasannya juga meminta program ini dibuat **tanpa menggunakan command system()**.

B. Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma `ROT19`.

C. Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: **r3N4mE**, **d3Let3**, dan **m0V3**. Untuk setiap file dengan nama yang memuat kode **d3Let3**, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode **r3N4mE**, lakukan hal berikut:
	i. Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
	ii. Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
	iii. Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
	iv. Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

D. Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
	i. _default_: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
	ii. _backup_: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
	iii. _restore_: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan
	iv. Contoh penggunaan: ./management -m backup

E. Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
	i. SIGRTMIN untuk mode _default_
	ii. SIGUSR1 untuk mode _backup_
	iii. SIGUSR2 untuk mode _restore_
	iv. Contoh penggunaan: kill -SIGUSR2 <pid_program>

F. Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien

G. Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
	i. Format: [nama_user][HH:MM:SS] - <nama*file> - <action>
	ii. nama_user adalah username yang melakukan action terhadap file
	iii. Format action untuk setiap kode: 1. kode \_r3N4mE*: Successfully renamed. 2. kode _d3Let3_: Successfully deleted. 3. mode _backup_: Successfully moved to backup. 4. mode _restore_: Successfully restored from backup.
	iv. Contoh pesan log: 1. [paul][00:00:00] - r3N4mE.ts - Successfully renamed. 2. [paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.

H. Berikut adalah struktur folder untuk pengerjaan nomor 2:
soal_2/
├── history.log
├── management.c
└── library/
└── backup/

#### > Penyelesaian

```bash
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
            sleep(3600); // agar program run tiap 1 jam
        }

        closelog();
        return 0;
    }
}
```

#### > Penjelasan

1. **Fungsi `handle_default(int sig)`**:
    - Fungsi ini menangani sinyal default (SIGRTMIN+4) dan mengubah mode menjadi 0 (default mode).
    - Sinyal ini digunakan untuk mengaktifkan operasi default pada daemon.

2. **Fungsi `handle_backup(int sig)`**:
    - Fungsi ini menangani sinyal SIGUSR1 dan mengubah mode menjadi 1 (backup mode).
    - Sinyal ini digunakan untuk memindahkan file ke direktori backup.

3. **Fungsi `handle_restore(int sig)`**:
    - Fungsi ini menangani sinyal SIGUSR2 dan mengubah mode menjadi 2 (restore mode).
    - Sinyal ini digunakan untuk mengembalikan file dari direktori backup ke direktori library.

4. **Fungsi `handle_kill(int sig)`**:
    - Fungsi ini menangani sinyal SIGTERM dan mengakhiri proses daemon.

5. **Fungsi `download_file()`**:
    - Fungsi ini melakukan download file dari URL yang telah ditentukan menggunakan perintah `wget`.

6. **Fungsi `unzip_file()`**:
    - Fungsi ini melakukan ekstraksi file ZIP yang telah diunduh ke direktori library menggunakan perintah `unzip`.

7. **Fungsi `rot19(char *filename)`**:
    - Fungsi ini mengenkripsi nama file dengan algoritma ROT19 (geser karakter sebanyak 7 posisi).

8. **Fungsi `decrypt_names()`**:
    - Fungsi ini mengenkripsi ulang nama file dalam direktori library menggunakan algoritma ROT19.

9. **Fungsi `delete_files()`**:
    - Fungsi ini menghapus file yang memiliki substring "d3Let3" dalam nama file.

10. **Fungsi `rename_files()`**:
    - Fungsi ini mengganti nama file sesuai dengan ekstensi file yang telah ditentukan.

11. **Fungsi `move_to_backup()`**:
    - Fungsi ini memindahkan file yang memiliki substring "m0V3" dalam nama file ke direktori backup.

12. **Fungsi `restore_from_backup()`**:
    - Fungsi ini mengembalikan file dari direktori backup ke direktori library.

13. **Fungsi `main(int argc, char *argv[])`**:
    - Fungsi utama yang mengatur mode kerja daemon.
    - Jika argumen `-m backup` diberikan, mode akan diubah menjadi backup mode.
    - Jika argumen `-m restore` diberikan, mode akan diubah menjadi restore mode.
    - Jika tidak ada argumen, daemon akan berjalan dalam mode default.
    - Daemon akan berjalan dalam loop dan melakukan operasi sesuai dengan mode yang aktif.

#### > Dokumentasi
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-2-2024-MH-IT13/assets/151142830/06afd83e-db6e-44aa-9b31-d912471626ef">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-2-2024-MH-IT13/assets/151142830/f10a5084-bdcb-48ec-80c6-3f4457ddafc9">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-2-2024-MH-IT13/assets/151142830/96518379-b7eb-4077-94aa-ec4122ce4278">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-2-2024-MH-IT13/assets/151142830/328cb7e8-a5af-40f9-af49-086fd50d61bf">

#### > Revisi

---

### **`Soal 3`**

`>Andre`

### > Isi Soal

#### > Penyelesaian

#### > Penjelasan

#### > Dokumentasi

#### > Revisi

---

### **`Soal 4`**

`>Rayyan`

### > Isi Soal

Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!
(NB: Soal di uji coba dan berhasil di sistem yang menggunakan MacOS dan linux yang menggunakan dual boot dan VM. Untuk teman-teman yang menggunakan WSL bisa mengerjakan soal yang lain ya)

- Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut `setup.c`
- Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan:
  - `./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>`
  - Contoh penggunaannya adalah sebagai berikut:
  - `./setup -o firefox 2 wireshark 2`
    - Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.
- Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan `./setup -f file.conf`
  - Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
  - Contoh isi file.conf:
    1. Firefox 2
    2. Wireshark 3
  - Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.
- Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan:
  - `./setup -k`
- Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi.
  - Contohnya:
    - `./setup -k file.conf `
  - Command ini hanya mematikan aplikasi yang dijalankan dengan
    - `./setup -f file.conf`

#### > Penyelesaian

### **`setup.c`**

```bash
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
```

#### > Penjelasan

### setup.c

1. Fungsi untuk memulai aplikasi berdasarkan argumen yang diberikan saat program dijalankan

```bash
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
```

2. Fungsi untuk menghentikan semua aplikasi yang sedang berjalan

```bash
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
```

3. Fungsi untuk memulai aplikasi berdasarkan informasi yang terdapat dalam file configurasi yang diberikan

```bash
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
```

4. Fungsi untuk menghentikan aplikasi berdasarkan informasi yang terdapat dalam file configurasi yang diberikan

```bash
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
```

5. Fungsi ini melakukan pengecekan terhadap argumen yang diberikan saat menjalankan program.

```bash
int main(int argc, char *argv[]) {
    // Memeriksa jumlah argumen yang diberikan
    if (argc < 2) {
        printf("Usage: %s -s <app1> <num1> <app2> <num2> ... <appN> <numN>\n", argv[0]);
        return 1;
    }

    // Memeriksa opsi yang diberikan saat menjalankan program
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
```
