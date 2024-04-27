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

** 1. Header Inclusion **
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
```
Header inclusion mengimpor pustaka standar yang diperlukan untuk operasi I/O, manipulasi string, manajemen proses, waktu, dan lain-lain.

** 2. Macro Definitions **
```bash
#define MAX_PATH_LENGTH 256
#define MAX_BUFFER_SIZE 1024
```
Definisi makro yang menentukan panjang maksimum path file dan ukuran maksimum buffer.

** 3. Function replaceString **
```bash
void replaceString(char *str, const char *old, const char *new) {
    // ...
}
```
Fungsi ini digunakan untuk mengganti semua kemunculan string old dalam string str dengan string new.

** 4. Function logReplacement **
```bash
void logReplacement(const char *filePath) {
    // ...
}
```
Fungsi ini mencatat penggantian string yang dilakukan ke dalam file log dengan mencatat timestamp dan path file yang diubah.

** 5. Function runDaemon **
```bash
void runDaemon(const char *folderPath) {
    // ...
}
```
Fungsi utama program yang menjalankan daemon. Proses utama daemon dijalankan di dalam fungsi ini.

** 6. Function main **
```bash
int main(int argc, char *argv[]) {
    // ...
}
```
Fungsi utama program. Menerima argumen berupa path folder tempat file contoh.txt berada. Jika jumlah argumen tidak sesuai, program akan mencetak pesan usage dan keluar. Jika argumen sesuai, maka fungsi runDaemon akan dipanggil dengan path folder yang diberikan.

** 7. Penjelasan Tambahan **
Pada bagian runDaemon, program menggunakan `fork()` untuk membuat proses baru, kemudian menjalankan proses tersebut sebagai sebuah daemon menggunakan `setsid()`, mengubah direktori kerja menjadi direktori yang ditentukan, menutup file descriptor standar (stdin, stdout, stderr), dan terus berjalan di dalam loop while untuk melakukan proses pemantauan dan pemrosesan file `contoh.txt`.
Fungsi `replaceString` digunakan untuk mencari dan mengganti string tertentu dalam buffer. Ini membantu dalam mengganti string yang berkaitan dengan malware dengan string yang lebih jelas terkait dengan jenis malware yang ditemukan.
Pemanggilan `logReplacement` terjadi setiap kali string dalam file `contoh.txt` berhasil diganti. Ini membantu dalam mencatat aktivitas penggantian string yang dilakukan oleh program ke dalam file log `virus.log`.

#### > Revisi

Tidak ada revisi dari program ini karena program dapat berjalan dengan baik dan sesuai dengan permintaan.
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
Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!

A. Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!
B. Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command:
./admin <user>
C. Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 
./admin -m <user>
Dan untuk mematikan fitur tersebut menggunakan: 
./admin -s <user>
Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:
[dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN
D. Program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan: 
./admin -c user
sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:
./admin -a user
E. Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN

#### > Penyelesaian
```bash
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
```

#### > Penjelasan
** 1. Header Inclusion **
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
```
Pustaka-pustaka standar yang digunakan untuk berbagai keperluan, seperti operasi I/O, manipulasi string, manajemen proses, manipulasi sinyal, waktu, dan manipulasi direktori.

** 2. Macro Definitions **
```bash
#define MAX_PROCESS 1024
#define LOG_FILE_PREFIX "kali_andrey.log"
```
Definisi makro untuk jumlah maksimum proses dan awalan nama file log.

** 3. Global Variables **
```bash
pid_t monitor_process_id = 0;
pid_t crack_process_id = 0;
char user[64];
FILE *fp_log;
```
Variabel global untuk menyimpan PID dari proses pemantauan dan proses crack, nama pengguna yang akan dimonitor/crack, dan file pointer untuk file log.

** 4. Function log_process **
```bash
void log_process(char *status, pid_t pid, char *process_name) {
    // ...
}
```
Fungsi untuk mencatat informasi proses ke dalam file log. Memasukkan waktu, PID proses, nama proses, dan status proses ke dalam file log.

** 5. Function kill_process **
```bash
void kill_process(int sig) {
    // ...
}
```
Fungsi yang akan dipanggil ketika sinyal SIGALRM diterima. Ini digunakan untuk memeriksa proses apa saja yang sedang berjalan dan mencatat informasi tersebut ke dalam file log.

** 6. Function main **
```bash
int main(int argc, char *argv[]) {
    // ...
}
```
Fungsi utama program. Menangani argumen yang diberikan saat menjalankan program dan melakukan operasi sesuai dengan argumen tersebut.

** 7. Proses Pemantauan **
>> Program dapat dijalankan dengan opsi -m untuk memonitor proses pengguna tertentu.
Ini dilakukan dengan membuat proses baru yang akan secara terus-menerus memeriksa proses apa >> saja yang sedang berjalan, dan jika ada proses yang berjalan dengan nama pengguna yang ditentukan, informasi tersebut akan dicatat ke dalam file log.

** 8. Proses Penghentian Pemantauan ** 
>> Program dapat dijalankan dengan opsi -s untuk menghentikan pemantauan proses pengguna tertentu.
>> Ini dilakukan dengan mengirim sinyal SIGTERM ke proses pemantauan dan menunggu proses tersebut berhenti.

** 9. Proses Crack **
>> Program dapat dijalankan dengan opsi -c untuk melakukan "crack" terhadap proses pengguna tertentu.
>> Ini dilakukan dengan membuat proses baru yang akan secara terus-menerus memeriksa proses apa saja yang sedang berjalan dan mencatat informasi tersebut ke dalam file log.

** 10. Penghentian Proses Crack **
>> Program dapat dijalankan dengan opsi -a untuk menghentikan proses crack terhadap proses pengguna tertentu.
>> Ini dilakukan dengan mengirim sinyal SIGTERM ke proses crack dan menunggu proses tersebut berhenti.

** 11. Penjelasan Tambahan **
>> Program menggunakan direktori /proc untuk memeriksa proses-proses yang sedang berjalan.
>> File log dibuka pada awal program dan ditutup sebelum program berakhir.
>> Program memberikan pesan kesalahan jika tidak cukup argumen yang diberikan atau jika gagal membuka file log.

#### > Revisi
Dikarenakan program yang saya buat sebelumnya tidak dapat menyelesaikan permasalahan pada poin c dan d maka saya membuat program baru seperti ini
```bash
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define PID_FILE "daemon.pid"
#define STATUS_FILE "status.txt"

int daemon_pid = -1;

void log_activity(char *user, char *activity, int success) {
    FILE *fp;
    char filename[50];
    time_t current_time;
    struct tm *local_time;
    char timestamp[50];

    time(&current_time);
    local_time = localtime(&current_time);
    strftime(timestamp, sizeof(timestamp), "%d:%m:%Y-%H:%M:%S", local_time);

    snprintf(filename, sizeof(filename), "%s.log", user);
    fp = fopen(filename, "a");

    if (fp != NULL) {
        fprintf(fp, "[%s]-%d-%s_%s\n", timestamp, getpid(), activity, success ? "JALAN" : "GAGAL");
        fclose(fp);
    } else {
        printf("Failed to open log file for user %s.\n", user);
    }
}

int read_status() {
    FILE *status_file = fopen(STATUS_FILE, "r");
    int blocked = 0;
    if (status_file != NULL) {
        fscanf(status_file, "%d", &blocked);
        fclose(status_file);
    }
    return blocked;
}

void monitor_activity(char *user) {
    int blocked = read_status();
    if (blocked) {
        log_activity(user, "Monitoring activity", 0);
        printf("Monitoring activity failed for user %s.\n", user);
        return;
    }

    pid_t pid;
    FILE *pid_file;

    pid = fork();

    if (pid == 0) {
        setsid();
        while (1) {
            blocked = read_status();
            if (blocked) {
                break;
            }
            log_activity(user, "Monitoring activity", 1);
            sleep(1);
        }
        exit(0);
    } else if (pid > 0) {
        daemon_pid = pid;
        pid_file = fopen(PID_FILE, "w");
        if (pid_file != NULL) {
            fprintf(pid_file, "%d", daemon_pid);
            fclose(pid_file);
        }
        printf("Monitoring started for user %s.\n", user);
    } else {
        printf("Fork failed.\n");
    }
}

void stop_monitoring(char *user) {
    int blocked = read_status();
    if (blocked) {
        log_activity(user, "Stopping monitoring", 0);
        printf("Stopping monitoring failed for user %s.\n", user);
        return;
    }

    FILE *pid_file;
    int pid;

    pid_file = fopen(PID_FILE, "r");
    if (pid_file != NULL) {
        fscanf(pid_file, "%d", &pid);
        fclose(pid_file);
        if (pid > 0) {
            kill(pid, SIGTERM);
            remove(PID_FILE);
            daemon_pid = -1;
            log_activity(user, "Stopping monitoring", 1);
            printf("Monitoring stopped for user %s.\n", user);
        } else {
            printf("No running monitoring process found for user %s.\n", user);
        }
    } else {
        printf("No running monitoring process found for user %s.\n", user);
    }
}

void block_activity(char *user) {
    FILE *status_file = fopen(STATUS_FILE, "w");
    if (status_file != NULL) {
        fprintf(status_file, "1");
        fclose(status_file);
        log_activity(user, "Blocking activity", 1);
        printf("Activity blocked for user %s.\n", user);
    } else {
        printf("Failed to open status file.\n");
    }
}

void allow_activity(char *user) {
    FILE *status_file = fopen(STATUS_FILE, "w");
    if (status_file != NULL) {
        fprintf(status_file, "0");
        fclose(status_file);
        log_activity(user, "Allowing activity", 1);
        printf("Activity allowed for user %s.\n", user);
    } else {
        printf("Failed to open status file.\n");
    }
}

void sigterm_handler(int signum) {
    printf("Received SIGTERM signal. Exiting.\n");
    exit(signum);
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, sigterm_handler);

    if (argc < 3) {
        printf("-m membuat program berjalan secara daemon dan berjalan terus menerus\n-s mematikan program yang berjalan secara daemon dan berjalan terus menerus\n-c menggagalkan proses yang dijalankan user setiap detik secara terus menerus\n-a mengembalikan akses untuk menjalankan user setiap detik secara terus menerus\n", argv[0]);
        return 1;
    }

    char *option = argv[1];
    char *user = argv[2];

    if (strcmp(option, "-m") == 0) {
        monitor_activity(user);
    } else if (strcmp(option, "-s") == 0) {
        stop_monitoring(user);
    } else if (strcmp(option, "-c") == 0) {
        block_activity(user);
    } else if (strcmp(option, "-a") == 0) {
        allow_activity(user);
    } else {
        printf("Invalid option.\n");
        return 1;
    }

    return 0;
}
```
1. program baru menggunakan setsid untuk membuat proses menjadi sesi pemimpin, menghindari proses terkait dengan terminal.

2. Penggunaan fscanf: Program baru menggunakan fscanf untuk membaca status pemblokiran dari file status.txt, memastikan pembacaan yang lebih aman dan terkontrol.

3. Penggunaan snprintf: Program baru menggunakan snprintf untuk menghindari buffer overflow dalam pembuatan nama file log.

4. Penanganan Sinyal SIGTERM: Program baru menambahkan penanganan sinyal SIGTERM dengan fungsi sigterm_handler. Ini memungkinkan program untuk melakukan proses pembersihan atau penanganan tertentu saat menerima sinyal SIGTERM, yang memungkinkan program untuk berhenti dengan benar ketika diminta.
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
