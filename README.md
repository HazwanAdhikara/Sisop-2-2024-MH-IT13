# PRAKTIKUM SISOP-2-2024-MH-IT13

**KELOMPOK IT13**
| Nama                      | NRP        |
|---------------------------|------------|
|Muhamad Arrayyan              | 5027231014 |
|Hazwan Adhikara Nasution      | 5027231017 |
|Muhammad Andrean Rizq Prasetio     | 5027231052 |

## Pengantar
Laporan resmi ini dibuat terkait dengan praktikum modul 2 sistem operasi yang telah dilaksanakan pada tanggal 22 april 2024 hingga tanggal 27 april 2024. Praktikum modul 2 terdiri dari 4 soal yang dikerjakan oleh kelompok praktikan yang terdiri dari 3 orang selama waktu tertentu.

Kelompok IT13 melakukan pengerjaan modul 2 ini dengan pembagian sebagai berikut:
  + Soal 1 dikerjakan oleh Muhammad Andrean Rizq Prasetio
  + Soal 2 dikerjakan oleh Hazwan Adhikara Nasution
  + Soal 3 dikerjakan oleh Muhammad Andrean Rizq Prasetio
  + Soal 4 dikerjakan oleh Muhamad Arrayyan

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
#### > Penyelesaian
#### > Penjelasan
#### > Dokumentasi
#### > Revisi
---
### **`Soal 2`**
`>Hazwan`
### > Isi Soal
#### > Penyelesaian
#### > Penjelasan
#### > Dokumentasi
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
+ Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut `setup.c`
+ Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan:
  - `./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>`
  - Contoh penggunaannya adalah sebagai berikut:
   - `./setup -o firefox 2 wireshark 2`
     - Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.
+ Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan `./setup -f file.conf`
  - Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
  - Contoh isi file.conf:
    1. Firefox 2
    2. Wireshark 3
  - Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.
+ Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan: 
  - `./setup -k`
+ Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
  - Contohnya: 
    + `./setup -k file.conf `
  - Command ini hanya mematikan aplikasi yang dijalankan dengan 
    + `./setup -f file.conf`


#### > Penyelesaian
### **`setup.c`**
```bash

#### > Penjelasan
#### > Dokumentasi
#### > Revisi




