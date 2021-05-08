# Soal Shift Sisop 2021 Modul 3

Kelompok D-04
- Thomas Felix Brilliant (05111940000062)
- Muhammad Rizky Widodo (05111940000216)
- Fiodhy Ardito Narawangsa (05111940000218)

## SOAL 1 ##

### 1A ###

- <b>SOAL</b>

  Pada saat <i>client</i> tersambung dengan <i>server</i>, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input ID dan <i>password</i>-nya untuk dikirimkan ke <i>server</i>. User juga dapat melakukan login. Login berhasil jika ID dan <i>password</i> yang dikirim dari aplikasi <i>client</i> sesuai dengan <i>list</i> akun yang ada di dalam aplikasi <i>server</i>. Sistem ini juga dapat menerima <i>multi-connections</i>. Koneksi terhitung ketika aplikasi <i>client</i> tersambung dengan <i>server</i>. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai <i>client</i> pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan ID dan <i>password</i> pada <i>file</i> bernama `akun.txt` dengan format:
  ```
  id:password
  id2:password2
  ```

### 1B ###

- <b>SOAL</b>

  Sistem memiliki sebuah <i>database</i> yang bernama `files.tsv`. Isi dari `files.tsv` ini adalah <b><i>path file</i> saat berada di <i>server</i></b>, <b><i>publisher</i></b>, dan <b>tahun publikasi</b>. Setiap penambahan dan penghapusan <i>file</i> pada folder <i>file</i> yang bernama `FILES` pada <i>server</i> akan memengaruhi isi dari `files.tsv`. Folder `FILES` otomatis dibuat saat <i>server</i> dijalankan.


### 1C ###

- <b>SOAL</b>

  Tidak hanya itu, Keverk juga diminta membuat fitur agar <i>client</i> dapat menambah file baru ke dalam <i>server</i>. Direktori `FILES` memiliki struktur direktori di bawah ini:
  ```
  File1.ekstensi
  File2.ekstensi
  ```
  
  Pertama, <i>client</i> mengirimkan input ke <i>server</i> dengan struktur sebagai berikut:
  
  <i>Command client</i>:
  ```
  add
  ```
  
  <i>Client console</i>:
  ```
  Publisher:
  Tahun Publikasi:
  Filepath:
  ```
  
  Kemudian, dari aplikasi <i>client</i> akan dimasukkan data buku tersebut (perlu diingat bahwa <i>Filepath</i> ini merupakan <b><i>path file</i> yang akan dikirim ke <i>server</i></b>). Lalu <i>client</i> nanti akan melakukan pengiriman <i>file</i> ke aplikasi <i>server</i> dengan menggunakan socket. Ketika <i>file</i> diterima di <i>server</i>, maka <i>row</i> dari `files.tsv` akan bertambah sesuai dengan data terbaru yang ditambahkan.
  
### 1D ###

- <b>SOAL</b>

  Dan <i>client</i> dapat men-<i>download file</i> yang telah ada dalam folder `FILES` di <i>server</i>, sehingga sistem harus dapat mengirim <i>file</i> ke <i>client</i>. <i>Server</i> <b>harus</b> melihat dari `files.tsv` untuk melakukan pengecekan apakah <i>file</i> tersebut valid. Jika tidak valid, maka mengirimkan pesan <i>error</i> balik ke <i>client</i>. Jika berhasil, <i>file</i> akan dikirim dan akan diterima ke <i>client</i> di <b>folder <i>client</i></b> tersebut.
  
  <i>Command client</i>:
  ```
  download TEMPfile.pdf
  ```

### 1E ###

- <b>SOAL</b>

  Setelah itu, <i>client</i> juga dapat menghapus <i>file</i> yang tersimpan di <i>server</i>. Akan tetapi, Keverk takut <i>file</i> yang dibuang adalah <i>file</i> yang penting, maka <i>file</i> hanya akan diganti namanya menjadi `old-NamaFile.ekstensi`. Ketika <i>file</i> telah diubah namanya, maka <i>row</i> dari <i>file</i> tersebut di `file.tsv` akan terhapus.

  <i>Command client</i>:
  ```
  delete TEMPfile.pdf
  ```

### 1F ###

- <b>SOAL</b>

  <i>Client</i> dapat melihat semua isi `files.tsv` dengan memanggil suatu perintah yang bernama `see`. Output dari perintah tersebut keluar dengan format:
  
  <i>Command client</i>:
  ```
  see
  ```
  
  Format output pada <i>client</i>:
  ```
  Nama:
  Publisher:
  Tahun publishing:
  Ekstensi File :
  Filepath :
  
  Nama:
  Publisher:
  Tahun publishing:
  Ekstensi File :
  Filepath :
  ```

### 1G ###

- <b>SOAL</b>

  Aplikasi <i>client</i> juga dapat melakukan pencarian dengan memberikan suatu <i>string</i>. Hasilnya adalah semua nama <i>file</i> yang mengandung <i>string</i> tersebut. Format output seperti nomor (f).
  
  <i>Command client</i>:
  ```
  find TEMP
  ```

### 1H ###

- <b>SOAL</b>

  Dikarenakan Keverk waspada dengan pertambahan dan penghapusan <i>file</i> di <i>server</i>, maka Keverk membuat suatu log untuk <i>server</i> yang bernama `running.log`. 
  
  Isi dari log:
  ```
  Tambah : File1.ektensi (id:pass)
  Hapus : File2.ektensi (id:pass)
  ```

Struktur direktori:
```
├── Client
│  ├── client.c
│  ├── File2.ekstensi
│  ├── File1.ekstensi
│ 
└── Server
   ├── akun.txt
   ├── files.tsv
   ├── server.c
   ├── running.log
   └── FILES
      ├── File2.ekstensi
      ├── File1.ekstensi
```

## SOAL 2 ##

### 2A ###

- <b>SOAL</b>

  Membuat program perkalian matriks (4 × 3 dengan 3 × 6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1 - 20 (tidak perlu dibuat filter angka).

### 2B ###

- <b>SOAL</b>

  Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (<b>Catatan!</b>: gunakan <i>shared memory</i>). Kemudian matriks tersebut akan dilakukan perhitungan dengan matriks baru sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap <i>cell</i> yang berasal dari matriks A menjadi angka untuk faktorial, lalu <i>cell</i> dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (<b>Catatan!</b>: gunakan <i>thread</i> untuk perhitungan di setiap <i>cell</i>).
  
  Ketentuan:
  ```
  If a >= b -> a! / (a - b)!
  If b > a -> a!
  If 0 -> 0
  ```
  
### 2C ###

- <b>SOAL</b>

  Karena takut <i>lag</i> dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan <i>resource</i> komputernya dengan command `ps aux | sort -nrk 3,3 | head -5` (<b>Catatan!</b>: Harus menggunakan <i>IPC Pipes</i>).
  
## SOAL 3 ##

### 3A ###

- <b>SOAL</b>

  Program menerima opsi `-f` seperti contoh, jadi pengguna bisa menambahkan argumen <i>file</i> yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna.

  Output yang dikeluarkan:
  ```
  File 1 : Berhasil Dikategorikan (jika berhasil)
  File 2 : Sad, gagal :( (jika gagal)
  File 3 : Berhasil Dikategorikan
  ```
  
### 3B ###

- <b>SOAL</b>

  Program juga dapat menerima opsi `-d` untuk melakukan pengkategorian pada suatu <i>directory</i>. Namun pada opsi `-d` ini, user hanya bisa memasukkan input 1 <i>directory</i> saja, tidak seperti <i>file</i> yang bebas menginput <i>file</i> sebanyak mungkin. Contohnya adalah seperti ini:
  ```
  $ ./soal3 -d /path/to/directory/
  ```
  
  Perintah di atas akan mengkategorikan file di `/path/to/directory`, lalu hasilnya akan disimpan di <i>working directory</i>, tempat program C tersebut berjalan (hasil kategori filenya bukan di `/path/to/directory`).

  Output yang dikeluarkan:
  ```
  Jika berhasil, print "Direktori sukses disimpan!"
  Jika gagal, print "Yah, gagal disimpan :("
  ```

### 3C ###

- <b>SOAL</b>

  Selain menerima opsi-opsi di atas, program ini menerima opsi `\*`, contohnya ada di bawah ini:
  ```
  $ ./soal3 \*
  ```
  
  Opsi ini akan mengkategorikan seluruh <i>file</i> yang ada di <i>working directory</i> ketika menjalankan program C tersebut.

### 3D ###

- <b>SOAL</b>

  Semua <i>file</i> harus berada di dalam folder, jika terdapat <i>file</i> yang tidak memiliki ekstensi, <i>file</i> disimpan dalam folder `Unknown`. Jika file <i>hidden</i>, masuk folder `Hidden`.

### 3E ###

- <b>SOAL</b>

  Setiap 1 <i>file</i> yang dikategorikan dioperasikan oleh 1 <i>thread</i> agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.
