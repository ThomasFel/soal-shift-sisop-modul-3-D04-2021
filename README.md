# Soal Shift Sisop 2021 Modul 3

Kelompok D-04
- Thomas Felix Brilliant (05111940000062)
- Muhammad Rizky Widodo (05111940000216)
- Fiodhy Ardito Narawangsa (05111940000218)

## SOAL 1 ##

### 1A ###

- <b>SOAL</b>

  Pada saat <i>client</i> tersambung dengan <i>server</i>, terdapat dua pilihan pertama, yaitu <i>register</i> dan <i>login</i>. Jika memilih register, <i>client</i> akan diminta input <b>ID</b> dan <b><i>password</i></b>-nya untuk dikirimkan ke <i>server</i>. User juga dapat melakukan <i>login</i>. <i>Login</i> berhasil jika ID dan <i>password</i> yang dikirim dari aplikasi <i>client</i> sesuai dengan <i>list</i> akun yang ada di dalam aplikasi <i>server</i>. Sistem ini juga dapat menerima <i>multi-connections</i>. Koneksi terhitung ketika aplikasi <i>client</i> tersambung dengan <i>server</i>. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai <i>client</i> pertama keluar untuk bisa melakukan <i>login</i> dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan <b>ID</b> dan <b><i>password</i></b> pada <i>file</i> bernama `akun.txt` dengan format:
  ```
  id:password
  id2:password2
  ```
  
- <b>JAWABAN</b>

  Menggunakan <i>template</i> `socket-client.c` dari modul 3.
  
  `client.c`
  ```C
  #define PORT 8080
  
  int main(int argc, char const *argv[]) {
       struct sockaddr_in address;
       int sock = 0, valread;
       struct sockaddr_in serv_addr;
       char *hello = "Hello from client";
    
       if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
           printf("\n Socket creation error \n");
           return -1;
       }
  
       memset(&serv_addr, '0', sizeof(serv_addr));
  
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_port = htons(PORT);
      
       if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
           printf("\nInvalid address/ Address not supported \n");
           return -1;
       }
  
       if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
           printf("\nConnection Failed \n");
           return -1;
       }
  
  . . .
  
  close(sock);
  
  return 0;
  ```
  Memodifikasi sedikit <i>template</i> di atas dengan menambahkan `close(sock)` untuk menutup <i>socket</i> setelah menjalankan proses <i>client</i>.
  
  Menggunakan <i>template</i> `socket-server.c` dari modul 3.
  
  `server.c`
  ```C
  #define PORT 8080
  
  . . .
  
  int main(int argc, char const *argv[]) {
       mkdir("FILES", 0777);

       int server_fd, new_socket[1000];
       struct sockaddr_in address;
       int opt = 1;
       int addrlen = sizeof(address);

       if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
       }
      
       if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
       }

       address.sin_family = AF_INET;
       address.sin_addr.s_addr = INADDR_ANY;
       address.sin_port = htons(PORT);
      
       if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
       }

       if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
       }
       
  . . .
  
  return 0;
  ```
  Memodifikasi sedikit <i>template</i> di atas dengan menambahkan `mkdir("FILES", 0777)` untuk membuat direktori baru, yaitu <b>FILES</b> dengan <i>set permission</i> 0777 yang akan digunakan pada [soal.1b](#1b "Goto 1b").
  
  Fungsi <i>main thread</i>.
  
  `server.c`
  ```C
  pthread_t tid[1000], id_client_login;
  
  . . .
  
  int main(int argc, char const *argv[]) {
  
  . . .
  
  int ctr = 0;
  
       while (1) {
            if ((new_socket[ctr] = accept(server_fd, (struct sockaddr *) &address, (socklen_t*)&addrlen)) < 0) {
                 perror("accept");
                 exit(EXIT_FAILURE);
            }

            pthread_create(&(tid[ctr]), NULL, operation, &new_socket[ctr]);
            ctr++;
            printf("Client %d is connected\n", ctr);
       }

       for (int i = 0; i < ctr; i++) {
            pthread_join(tid[i], NULL);
       }
  
       return 0;
  
  }
  ```
  - Mendefinisikan <i>thread</i> `tid` dan `id_client_login` sebagai variabel global.
  - Mendeklarasikan variabel `ctr` untuk <i>counter</i> di dalam <i>main thread</i>.
  - <i>Thread</i> akan dibuat dengan `pthread_create(&(tid[ctr]), NULL, operation, &new_socket[ctr])`. <i>Thread</i> akan menjalankan fungsi `operation` sebagai <i>routine</i> dengan atribut `&new_socket[ctr]` sebagai variabel.
  - <i>Join</i>-kan setiap <i>thread</i> yang telah dibuat dengan `pthread_join(tid[i], NULL)`.
  
  Membuat fungsi `operation`, kemudian <i>looping</i> untuk menjalankan <i>client</i>.
  
  `server.c`
  ```C
  void *operation(void *arg) {
       int *new_socket = (int *) arg; 
       int flag = 0;
       
       while (1) {
            char buffer[1024] = {0};
            char message[1024] = {0};
            char parameter[10][1024] = {0};
            int index = 0;
            int valread;
  
       . . .
  
       }
  }
  ```
  - Mendeklarasikan variabel <i>pointer</i> `*new_socket` untuk membuat <i>socket</i> baru.
  - Mendeklarasikan variabel `flag` untuk penanda dalam pengondisian <i>command</i> yang nanti diinput.
  - Mendeklarasikan variabel `buffer` untuk menerima <i>buffer</i> (memori yang menyimpan data ketika data ditransfer).
  - Mendeklarasikan variabel `message` untuk mengirim pesan ke <i>client</i> maupun sebaliknya.
  - Mendeklarasikan variabel `parameter` untuk menyimpan nama <i>file download</i> atau di-<i>delete</i>.
  - Mendeklarasikan variabel `index` untuk membantu <i>indexing</i> proses penyimpanan nama <i>file</i>.
  - Mendeklarasikan variabel `valread` untuk menerima pesan dari <i>socket</i>.
  
  Membuat pengondisian untuk halaman awal ketika program dijalankan.
  
  `server.c`
  ```C
  if (flag == 0) { //LANDING PAGE
       strcpy(message, "1.Register\n2.Login\n\nInput: ");
       send(*new_socket, message, strlen(message), 0);
       valread = recv(*new_socket, buffer, 1024, 0); //RECEIVE MESSAGES FROM SOCKET
            
       if (checkClose(valread, new_socket)) 
            break;
       }
  
  . . .
  
  ```
  Jika `flag` mempunyai <i>value</i> 0, maka akan menampilkan halaman awal. Menggunakan `strcpy` untuk menyalin <i>string</i> yang akan dimunculkan di terminal ke variabel `message`. Lalu, mengirimkan pesan ke <i>client</i> dan nantinya `valread` akan menerima pesan balasan dari <i>socket</i>. Setelah itu, jika fungsi `checkClose` bernilai <b>TRUE</b> akan menghentikan <i>looping</i>.
  
  Membuat fungsi untuk pengecekan <i>client</i> apakah sedang <i>login</i> atau tidak.
  
  `server.c`
  ```C
  int login = 0, ctr = 1;
  char id_login[100] = {0};
  char password_login[100] = {0};
  
  bool checkClose(int valread, int *new_socket) {
       if (valread == 0) {
            if (pthread_equal(pthread_self(), id_client_login)) { //COMPARE 2 THREAD IDENTIFIERS, RETURNS THE ID OF THE CALLING THREAD
                 login = 0;
                 bzero(id_login, 100); //ERASE DATA IN THE 100 BYTES OF THE MEMORY STARTING AT THE LOCATION POINTED BY ID_LOGIN
                 bzero(password_login, 100);
            }

            close(*new_socket);
            return 1;
      }

      return 0;
  }
  ```
  - Mendeklarasikan variabel `login` dengan <i>value</i> 0 dan `ctr` dengan <i>value</i> 1 sebagai variabel global.
  - Mendeklarasikan variabel `id_login` dan `password` untuk proses <i>login</i> sebagai variabel global.
  - Saat <i>client</i> menghentikan aplikasi, mengecek apakah <i>client</i> merupakan <i>client</i> yang sedang <i>login</i> dengan membandingkan <b>ID <i>Thread Client</i></b> dan variabel `id_client_login` menggunakan `pthread_equal`.
  - Jika sama, maka hapus semua variabel yang berkaitan dengan <i>client</i> yang sedang <i>login</i> dengan `bzero`.
  - Setelah itu tutup <i>socket</i> dengan `close(*new_socket)`.

  Ketika pengguna menginputkan 1, maka akan masuk ke halaman <i>register</i>.
  
  `server.c`
  ```C
  if (strcmp(buffer, "1") == 0) { //REGISTER
       strcpy(message, "Registration\n\nID: ");
       send(*new_socket, message, strlen(message), 0);
                
       char id[100] = {0};
       char password[100] = {0};
       valread = recv(*new_socket, id, 100, 0);
                
       if (checkClose(valread, new_socket)) {
            break;
       }

       strcpy(message, "Password: ");
       send(*new_socket, message, strlen(message), 0);
       valread = recv(*new_socket, password, 100, 0);
                
       if (checkClose(valread, new_socket)) {
            break;
       }

       FILE *fileout;
       fileout = fopen("akun.txt", "a");
       char strconcate[200] = {0};
                
       strcpy(strconcate, id);
       strcat(strconcate, ":");
       strcat(strconcate, password);
       fputs(strconcate, fileout);
       fputs("\n", fileout);
       fclose(fileout);

       printf("Data saved successfully.\n");

       strcpy(message, "Registration success!\n\n");
       send(*new_socket, message, strlen(message), 0);
  }
  ```
  - <i>Server</i> akan meminta <i>client</i> untuk menginputkan <b>ID</b> dan <b><i>Password</i></b> ke dalam variabel `id` dan `password`. Nantinya <b>ID</b> dan <b><i>Password</i></b> dikirim menggunakan `send` dan akan mendapat pesan balasan yang disimpan di `valread`.
  - `fopen` digunakan untuk membuka `akun.txt`, menggunakan <i>command</i> `a` agar data yang diinput ditambahkan di bagian akhir teks.
  - Memasukkan output dengan mengkombinasi `strcpy`, `strcat`, dan `fputs` ke dalam `akun.txt`. Lalu menutup <i>file</i>-nya.
  - Memunculkan pesan <b>sukses</b> baik di terminal <i>server</i> maupun <i>client</i>.

  Ketika pengguna menginputkan 2, maka akan masuk ke halaman <i>login</i>.
  
  `server.c`
  ```C
  else if (strcmp(buffer, "2") == 0) { //LOGIN
       strcpy(message,"Login\n\nID: ");
       send(*new_socket, message, strlen(message), 0);
                
       char id[100] = {0};
       char password[100] = {0};
       valread = recv(*new_socket, id, 100, 0);
                
       if (checkClose(valread, new_socket)) {
            break;
       }

       strcpy(message, "Password: ");
       send(*new_socket, message, strlen(message), 0);
       valread = recv(*new_socket, password, 100, 0);
                
       if (checkClose(valread, new_socket)) {
            break;
       }

       FILE *filein;
       filein = fopen("akun.txt", "r");

       char temp[100] = {0};
       char tempId[100] = {0};
       char tempPassword[100] = {0};
       bool found = false;

       while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) { //READS FORMATTED INPUT FROM STREAM (END OF FILE)
            char *token = strtok(temp, ":"); //BREAKS STRING INTO A SERIES OF TOKENS USING THE DELIMITER ":"

            if (token != NULL) {
                 strcpy(tempId, token);
                 token = strtok(NULL, ":");
            }

            if (token != NULL) {
                 strcpy(tempPassword, token);
            }

            if (strcmp(id, tempId) == 0 && strcmp(password, tempPassword) == 0) {
                 found = true;
                 break;
            } 
       }

       if (login == 0) {
            if (found) {
                 strcpy(message, "Login success!\n\n");
                 send(*new_socket, message, strlen(message), 0);
                        
                 id_client_login = pthread_self(); //ID THREAD CLIENT
                 login = 1;
                 flag = 1;
                        
                 . . .
		 
            }
                    
            else {
                 strcpy(message, "Login failed!\n\n");
                 send(*new_socket, message, strlen(message), 0);
            }
       }
                
       else {
            strcpy(message, "Another client has logged in!\n\n");
            send(*new_socket, message, strlen(message), 0);
       }
  }
  ```
  - <i>Server</i> akan meminta <i>client</i> untuk menginputkan <b>ID</b> dan <b><i>Password</i></b> ke dalam variabel `id` dan `password`. Nantinya <b>ID</b> dan <b><i>Password</i></b> dikirim menggunakan `send` dan akan mendapat pesan balasan yang disimpan di `valread`.
  - `fopen` digunakan untuk membuka `akun.txt`, menggunakan <i>command</i> `r` untuk melakukan proses penginputan.
  - Mendeklarasikan variabel `temp`, `tempId`, `tempPassword` untuk pencocokan input dengan database yang ada di `akun.txt`.
  - Mendeklarasikan variabel `found` untuk <i>handling login</i>.
  - Melakukan `fscanf` sampai dengan <b><i>END OF FILE</i></b> (EOF). Menggunakan `strtok` agar <i>string</i> dipisah menjadi <i>token</i> dengan <i>delimiter</i> `:` untuk  mengambil <b>ID</b> dan <b><i>Password</i></b> yang ada di database. Kemudian, dibandingkan antara input dan database. Jika <b>TRUE</b>, maka <i>value</i> `found` menjadi <b>TRUE</b>.
  - Varibel `login` digunakan untuk melihat apakah <i>client</i> sudah <i>login</i> atau belum. Jika sudah, maka akan memunculkan pesan bahwa <i>client</i> lain ada yang sedang <i>login</i>, jika belum dan verifikasi berhasil, maka mengubah <i>value</i> `login` menjadi 1 dan variabel `id_client_login` diisi dengan <b>ID <i>Thread Client</i></b> menggunakan fungsi `pthread_self()`. Setelah itu menyalin <b>ID</b> dan <b><i>Password</i></b> ke dalam `id_login` dan `password_login`.
  - Memunculkan pesan <b>sukses</b> dan <b>gagal</b> baik di terminal <i>server</i> maupun <i>client</i>.

- <b>OUTPUT</b>

  <b><i>Register</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262651-274b3d00-bc06-11eb-86d7-6bcda8cc734c.png" width="640" height="480">

  <b><i>Login</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262632-14386d00-bc06-11eb-9ef2-e16cd35f6ad6.png" width="640" height="480">
  
  <b><i>akun.txt</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262847-e7d12080-bc06-11eb-87d3-967dfbf9aafc.png" width="640" height="480">
  
### 1B ###

- <b>SOAL</b>

  Sistem memiliki sebuah <i>database</i> yang bernama `files.tsv`. Isi dari `files.tsv` ini adalah <b><i>path file</i> saat berada di <i>server</i></b>, <b><i>publisher</i></b>, dan <b>tahun publikasi</b>. Setiap penambahan dan penghapusan <i>file</i> pada folder <i>file</i> yang bernama `FILES` pada <i>server</i> akan memengaruhi isi dari `files.tsv`. Folder `FILES` otomatis dibuat saat <i>server</i> dijalankan.
  
- <b>JAWABAN</b>

  Seperti yang telah dijelaskan pada sub-soal sebelumnya, folder <b>FILES</b> telah otomatis dibuat sewaktu program dijalankan. Dan pembuatan `files.tsv`-nya akan sekalian masuk ke sub-soal selanjutnya.
  
- <b>OUTPUT</b>

  <b><i>Folder FILES otomatis dibuat setelah program jalan</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262717-67122480-bc06-11eb-9098-f20351b1ed34.png" width="640" height="480">


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

- <b>JAWABAN</b>
  
  Membuat pengondisian untuk halaman <i>command</i> ketika program dijalankan.
  
  `server.c`
  ```C
  if (flag == 1) { //COMMAND INPUT
       strcpy(message, "Command: ");
       send(*new_socket, message, strlen(message), 0);
            
       valread = recv(*new_socket, buffer, 100, 0);

       if (checkClose(valread, new_socket)) {
            break;
       }

       if (strcmp("add", buffer) == 0) {
            flag = 2;
       }
  
  . . .
  
  }
  ```
  - Jika `flag` mempunyai <i>value</i> 1, maka akan menampilkan halaman <i>command</i>. Menggunakan `strcpy` untuk menyalin <i>string</i> yang akan dimunculkan di terminal ke variabel `message`. Lalu, mengirimkan pesan ke <i>client</i> dan nantinya `valread` akan menerima pesan balasan dari <i>socket</i>. Setelah itu, jika fungsi `checkClose` bernilai <b>TRUE</b> akan menghentikan <i>looping</i>.
  - Jika <i>command</i> yang diinputkan adalah `add`, maka `flag` bernilai 2 untuk masuk ke pengondisian `add`.

  Pengondisian <i>command</i> `add`.
  
  `server.c`
  ```C
  if (flag == 2) { //COMMAND "ADD"
       strcpy(message, "Publisher: ");
       send(*new_socket, message, strlen(message), 0);
       char publisher[100] = {0};
       valread = recv(*new_socket, publisher, 100, 0);
            
       if (checkClose(valread, new_socket)) {
            break;
       }

       strcpy(message, "Tahun Publikasi: ");
       send(*new_socket, message , strlen(message) , 0);
       char year[100] = {0};
       valread = recv(*new_socket, year, 100, 0);
            
       if (checkClose(valread, new_socket)) {
            break;
       }

       strcpy(message, "Filepath: ");
       send(*new_socket, message, strlen(message), 0);
            
       char temp[1000] = {0}, fileName[1024] = {0}, filePath[1024] = {0};
       valread = recv(*new_socket, fileName, 1024, 0);
       strcpy(message, "OK");
       send(*new_socket, message, strlen(message), 0);

       strcpy(filePath, "FILES/");
       strcat(filePath, fileName);

  . . .
  
  }
  ```
  <i>Server</i> akan meminta <i>client</i> untuk menginputkan <b>publisher</b>, <b>tahun publikasi</b>, dan <b><i>filepath</i></b> ke dalam variabel `publisher`, `year` dan `fileName`. Nantinya <b>publisher</b>, <b>tahun publikasi</b>, dan <b><i>filepath</i></b> dikirim menggunakan `send` dan akan mendapat pesan balasan yang disimpan di `valread`. Untuk <b><i>filepath</i></b> menggunakan `strcpy` dan `strcat` untuk memunculkan output agar sesuai yang diharapkan soal.
  
  Proses <i>client</i> ketika soal [soal.1c](#1c "Goto 1c") dijalankan.
  
  `client.c`
  ```C
  if (strcmp(buffer, "Filepath: ") == 0) {
       char filePath[1024] = {0};
       scanf("%[^\n]%*c", filePath);

       //SEND FILENAME
       char tempfilePath[1024] = {0};
       strcpy(tempfilePath, filePath);
       char *token = strtok(tempfilePath, "/");
       char namaFile[1024] = {0};
            
       while (token) {
            strcpy(namaFile, token);
            token = strtok(NULL, "/");
       }

       send(sock, namaFile, strlen(namaFile), 0);
       valread = read(sock, buffer, 1024);

       FILE *filein;
       filein = fopen(filePath, "r");
       char temp[1000] = {0};

       while (fgets(temp, 1000, filein)) {
            send(sock, temp, strlen(temp), 0);
            valread = read(sock, buffer, 1024);
       }
            
       printf("SENDING DONE.\n\n");
       strcpy(message, "DONE");
       send(sock, message, strlen(message), 0);
  }
  ```
  <i>Client</i> akan memecah <b><i>filepath</i></b> yang diinputkan untuk memperoleh nama <i>file</i> yang akan dikirim ke server menggunakan `strtok`. Setelah itu, <i>client</i> akan mengirimkan <i>file</i> baris per baris menggunakan `fgets`. Setiap baris yang dikirimkan server akan menunggu respon dari server yang akan menjawab `OK`. Proses ini berakhir ketika <i>client</i> mengirimkan pesan `DONE`.
  
  Lanjutan proses <i>server</i> untuk soal [soal.1c](#1c "Goto 1c").
  
  `server.c`
  ```C
  if (flag == 2) { //COMMAND "ADD"
       do {   
            bzero(temp, 1000); //ERASE DATA IN THE 1000 BYTES OF THE MEMORY STARTING AT THE LOCATION POINTED BY TEMP
            valread = recv(*new_socket, temp, 1000, 0);
                
            if (strcmp(temp, "DONE") != 0) {
                 FILE *fileout;
                 fileout = fopen(filePath, "a");
                 fputs(temp, fileout);
                 fclose(fileout);
                 strcpy(message, "OK");
                 send(*new_socket, message, strlen(message), 0);
            }
       } while (strcmp(temp, "DONE") != 0);
            
       printf("SAVING DONE.\n");
       flag = 1;

       FILE *fileout;
       fileout = fopen("files.tsv", "a");
       fprintf(fileout, "%s\t%s\t%s\n", filePath, publisher, year);
       fclose(fileout);
  }
  ```
  <i>Server</i> akan menulis tiap baris ke suatu <i>file</i> menggunakan `fputs` dan merespon dengan pesan `OK` untuk tiap barisnya hingga server menerima pesan `DONE`. Setelah itu server menambahkan <i>record</i> untuk <i>file</i> yang diinputkan pada `files.tsv` dan dengan `fprintf(fileout,"%s\t%s\t%s\n", filePath, publisher, year)` untuk menyimpan output sesuai dengan permintaan soal. Tidak lupa mengubah <i>value</i> `flag` agar kembali ke proses <i>command</i>. Terakhir, memunculkan pesan <b>sukses</b> di terminal <i>server</i> maupun <i>client</i>.

- <b>OUTPUT</b>

  <b><i>Command ADD dijalankan</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262757-9a54b380-bc06-11eb-8b3b-a3281f5c8922.png" width="640" height="480">
  
  <b><i>files.tsv</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262825-cf610600-bc06-11eb-8cba-bdd51b66c6a7.png" width="640" height="480">

  
### 1D ###

- <b>SOAL</b>

  Dan <i>client</i> dapat men-<i>download file</i> yang telah ada dalam folder `FILES` di <i>server</i>, sehingga sistem harus dapat mengirim <i>file</i> ke <i>client</i>. <i>Server</i> <b>harus</b> melihat dari `files.tsv` untuk melakukan pengecekan apakah <i>file</i> tersebut valid. Jika tidak valid, maka mengirimkan pesan <i>error</i> balik ke <i>client</i>. Jika berhasil, <i>file</i> akan dikirim dan akan diterima ke <i>client</i> di <b>folder <i>client</i></b> tersebut.
  
  <i>Command client</i>:
  ```
  download TEMPfile.pdf
  ```

- <b>JAWABAN</b>

  Proses untuk menerima inputan dari terminal.
  
  `server.c`
  ```C
  char *ret;
  ret = strstr(buffer, "download"); //FIND THE FIRST OCURRENCE OF THE SUBSTRING "DOWNLOAD" IN BUFFER
                
  if (ret) {
       char *token = strtok(buffer, " ");

       while (token != NULL) {
            strcpy(parameter[index], token);
            index++;
            token = strtok(NULL, " ");
       }

       flag = 3;
  }
  ```
  Karena <i>command</i> terdiri dari dua kata, maka harus dipecah menggunakan `strtok` dengan delimiter ` ` sehingga bisa diperoleh nama <i>file</i> yang ingin diunduh, kemudian disimpan ke variabel `parameter`. Lalu <i>assign</i> variabel `flag` dengan 3 untuk masuk ke pengondisian <i>command</i> `download`.
  
  Proses <i>client</i> ketika [soal.1d](#1d "Goto 1d") dijalankan.
  
  `client.c`
  ```C
  else if (strcmp(buffer, "DOWNLOAD PREPARING...") == 0) {
       printf("\n");
       bzero(message, 1024);
       strcpy(message, "OK1");
       send(sock, message, strlen(message), 0);

       char temp[1000] = {0}, fileName[1024] = {0};
       valread = recv(sock, fileName, 1024, 0);
       
       bzero(message, 1024);
       strcpy(message, "OK2");
       send(sock, message, strlen(message), 0);

       do {   
            bzero(temp, 1000);
            valread = recv(sock, temp, 1000, 0);
                
            if (strcmp(temp, "DONE") != 0) {
                 FILE *fileout;
                 fileout = fopen(fileName, "a");
                 fputs(temp, fileout);
                 fclose(fileout);
                 strcpy(message, "OK");
                 send(sock, message, strlen(message), 0);
            }
       } while(strcmp(temp, "DONE") != 0);
            
       strcpy(message, "OK");
       send(sock, message, strlen(message), 0);
       printf("DOWNLOAD SUCCESS.\n\n");
  }
  ```
  Dengan proses yang sama pada [soal.1c](#1c "Goto 1c"), <i>client</i> akan melakukan pengecekan nama <i>file</i>. Setiap nama <i>file</i> yang dikirimkan server akan menunggu respon dari server yang akan menjawab `OK`. Proses ini berakhir ketika <i>client</i> mengirimkan pesan `DONE`.
  
  Pengondisian <i>command</i> `download`.
  
  `server.c`
  ```C
  if (flag == 3) { //COMMAND "DOWNLOAD"
       //DB CHECK
       FILE *filein;
       filein = fopen("files.tsv", "r");
       char search[1000] = {0};
       strcpy(search, "FILES/");
       strcat(search, parameter[1]);

       char temp[1000] = {0};
       char filePath[100] = {0};
       bool found = false;
            
       while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) {
            char *token = strtok(temp, "\t");
                
            if (token != NULL) {
                 strcpy(filePath, token);
            }
                
            if (strcmp(search, filePath) == 0) {
                 found = true;
                 break;
            }
       }
       
       fclose(filein);
  
  . . .
  
  else {
       strcpy(message, "ERROR! File not found.\n\n");
       send(*new_socket, message, strlen(message), 0);
  }

       flag = 1;
  }
  ```
  <i>Server</i> melakukan pengecekan apakah <i>file</i> yang akan diunduh terdapat pada database `files.tsv`. Menggunakan `strtok` untuk mengambil <b><i>filepath</i></b>-nya. Jika <i>file</i> ditemukan maka akan langsung masuk kondisi `found` bernilai <b><i>TRUE</i></b>. Jika tidak ditemukan, maka akan memunculkan pesan <b><i>ERROR</i></b>. Tidak lupa mengubah <i>value</i> `flag` agar kembali ke proses <i>command</i>.
 
  Proses ketika `found` berjalan.
  
  `server.c`
  ```C
  if (flag == 3) { //COMMAND "DOWNLOAD"
     
  . . .
     
       if (found) {
            bzero(message, 1024); //ERASE DATA IN THE 1000 BYTES OF THE MEMORY STARTING AT THE LOCATION POINTED BY MEMORY
            bzero(buffer, 1024);
            strcpy(message, "DOWNLOAD PREPARING...");
            send(*new_socket, message, strlen(message), 0);
            valread = recv(*new_socket, buffer, 1024, 0);

            bzero(message, 1024);
            bzero(buffer, 1024);

            //SEND FILENAME
            strcpy(message, parameter[1]);
            send(*new_socket, message, strlen(message), 0);
            valread = recv(*new_socket, buffer, 1024, 0);

            FILE *filein;
            filein = fopen(filePath, "r");

            while (fgets(temp, 1000, filein)) {
                 send(*new_socket, temp, strlen(temp), 0);
                 valread = recv(*new_socket, buffer, 1024, 0);
            }

            printf("SENDING DONE.\n");
            strcpy(message, "DONE");
            send(*new_socket, message, strlen(message), 0);
            valread = recv(*new_socket, buffer, 1024, 0);
  
  . . .
  
  }
  ```
  <i>Server</i> akan mengirimkan pesan ke <i>client</i> ketika <i>file</i> ditemukan, lalu juga akan mengirimkan <i>filename</i>. Setelah itu, <i>client</i> akan mengirimkan <i>file</i> baris per baris menggunakan `fgets`. Proses ini berakhir ketika <i>client</i> mengirimkan pesan `DONE`.

- <b>OUTPUT</b>

  <b><i>Folder client dikosongkan</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119262985-6ded6700-bc07-11eb-8926-0401370e7238.png" width="640" height="480">
  
  <b><i>Command DOWNLOAD dijalankan</i></b>
  
  <img src="https://user-images.githubusercontent.com/37539546/119262963-531af280-bc07-11eb-962f-6a72faa3735f.png" width="640" height="480">
  
### 1E ###

- <b>SOAL</b>

  Setelah itu, <i>client</i> juga dapat menghapus <i>file</i> yang tersimpan di <i>server</i>. Akan tetapi, Keverk takut <i>file</i> yang dibuang adalah <i>file</i> yang penting, maka <i>file</i> hanya akan diganti namanya menjadi `old-NamaFile.ekstensi`. Ketika <i>file</i> telah diubah namanya, maka <i>row</i> dari <i>file</i> tersebut di `file.tsv` akan terhapus.

  <i>Command client</i>:
  ```
  delete TEMPfile.pdf
  ```

- <b>JAWABAN</b>

  Proses untuk menerima inputan dari terminal.
  
  `server.c`
  ```C
  ret = strstr(buffer, "delete");
                
  if (ret) {
       char *token = strtok(buffer, " ");

       while (token != NULL) {
            strcpy(parameter[index], token);
            index++;
            token = strtok(NULL, " ");
       }

       flag = 4;
  }
  ```
  Karena <i>command</i> terdiri dari dua kata, maka harus dipecah menggunakan `strtok` dengan delimiter ` ` sehingga bisa diperoleh nama <i>file</i> yang ingin diunduh, kemudian disimpan ke variabel `parameter`. Lalu <i>assign</i> variabel `flag` dengan 4 untuk masuk ke pengondisian <i>command</i> `delete`.
  
  Pengondisian <i>command</i> `delete`.
  
  `server.c`
  ```C
  if (flag == 4) { //COMMAND "DELETE"
       //DB CHECK
       FILE *filein;
       filein = fopen("files.tsv", "r");
       char search[1000] = {0};
       strcpy(search, "FILES/");
       strcat(search, parameter[1]);

       char temp[1000] = {0};
       char filePath[100]= {0};
       bool found = false;
            
       while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) {
            char *token = strtok(temp, "\t");
            if (token != NULL) {
                 strcpy(filePath, token);
            }
                
            if (strcmp(search, filePath) == 0) {
                 found = true;
                 break;
            }
       }

       fclose(filein);
  
  . . .
  
       else {
            strcpy(message, "ERROR! File not found.\n\n");
            send(*new_socket, message, strlen(message), 0);
       }
       
       flag = 1;
  }
  ```
  <i>Server</i> melakukan pengecekan apakah <i>file</i> yang akan dihapus terdapat pada database `files.tsv`. Menggunakan `strtok` untuk mengambil <b><i>filepath</i></b>-nya. Jika <i>file</i> ditemukan maka akan langsung masuk kondisi `found` bernilai <b><i>TRUE</i></b>. Jika tidak ditemukan, maka akan memunculkan pesan <b><i>ERROR</i></b>. Tidak lupa mengubah <i>value</i> `flag` agar kembali ke proses <i>command</i>.
  
  Proses ketika `found` berjalan.
  
  `server.c`
  ```C
  if (flag == 4) { //COMMAND "DELETE"
  
  . . .
  
       if (found) {
            char fileDelete[100] = {0}, newName[100]= {0};
            strcpy(fileDelete, "FILES/");
            strcat(fileDelete, parameter[1]);
            strcpy(newName, "FILES/old-");
            strcat(newName, parameter[1]);
            rename(fileDelete, newName);

            FILE *fileout;
            filein = fopen("files.tsv", "r");

            //DUMMY FILE
            fileout = fopen("tempFiles.tsv", "w");
            fclose(fileout);

            while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) {
                 char temp2[100] = {0};
                 strcpy(temp2, temp);
                 char *token = strtok(temp,"\t");
                    
                 if (token != NULL) {
                      strcpy(filePath, token);
                 }

                 if (strcmp(search, filePath) != 0) {
                      fileout = fopen("tempFiles.tsv", "a");
                      fputs(temp2, fileout);
                      fputs("\n", fileout);
                      fclose(fileout);
                 }
            }

            remove("files.tsv");
            rename("tempFiles.tsv", "files.tsv");

            . . .

            printf("DELETION DONE.\n");
            strcpy(message, "DELETE SUCCESS.\n\n");
            send(*new_socket, message, strlen(message), 0);
       }
  
  . . .
  
  }
  ```
  Ketika <i>file</i> ditemukan, ubah nama <i>file</i> tersebut dengan mendapat awalan `old-` menggunakan fungsi `rename` yang didapat dari kombinasi `strcpy` dan `strcat`. Lalu, membuat semacam <i>file dummy</i>, `tempFiles.tsv` untuk seluruh baris kecuali baris yang dihapus. Setelah itu `files.tsv` yang lama dihapus dengan fungsi `remove` dan <i>file dummy</i>, `tempFiles.tsv` di-<i>rename</i> menjadi `files.tsv`. Terakhir, memunculkan pesan <b>sukses</b> di terminal <i>server</i> maupun <i>client</i>.

- <b>OUTPUT</b>

  <b><i>Folder FILES sebelum menjalankan DELETE</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119263015-91181680-bc07-11eb-9fe9-96ec4440ec3a.png" width="640" height="480">
  
  <b><i>Command DELETE dijalankan</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119263030-9e350580-bc07-11eb-810f-cf548ee09b5e.png" width="640" height="480">
  
  <b><i>files.tsv</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119263056-b573f300-bc07-11eb-9f6b-16111e58eb80.png" width="640" height="480">
  
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

- <b>JAWABAN</b>
  
  Pengondisian <i>command</i> `see`.
  
  `server.c`
  ```C
  if (flag == 5) { //COMMAND "SEE"
       FILE *filein;
       filein = fopen("files.tsv", "r");

       char temp[1000] = {0};
       char filePath[100] = {0}, publisher[100] = {0}, year[100] = {0}, name[100] = {0}, ext[100] = {0};

       while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) {
            char *token = strtok(temp, "\t");

            if (token != NULL) {
                 strcpy(filePath, token);
                 token = strtok(NULL, "\t");
            }

            if (token != NULL) {
                 strcpy(publisher, token);
                 token = strtok(NULL, "\t");
            }

            if (token != NULL){
                 strcpy(year, token);
            }

            token = strtok(temp, "/");
            token = strtok(NULL, ".");
            strcpy(name, token);
            token = strtok(NULL, ".");
            strcpy(ext,token);

            strcpy(message, "Nama: ");
            strcat(message, name);
            strcat(message, "\n");
            strcat(message, "Publisher: ");
            strcat(message, publisher);
            strcat(message, "\n");
            strcat(message, "Tahun publishing: ");
            strcat(message, year);
            strcat(message, "\n");
            strcat(message, "Ekstensi: ");
            strcat(message, ext);
            strcat(message, "\n");
            strcat(message, "Filepath: ");
            strcat(message, filePath);
            strcat(message, "\n");
            send(*new_socket, message, strlen(message), 0 );

            valread = recv(*new_socket, buffer, 1000, 0);
       }

       strcpy(message, "DONE");
       send(*new_socket, message, strlen(message), 0);
       valread = recv(*new_socket, buffer, 1000, 0);
            
       fclose(filein);

       flag = 1;
  }
  ```
  <i>Server</i> melakukan pembacaan terhadap database `files.tsv` baris per baris dengan `fscanf(filein, "%[^\n]%*c", temp)) != EOF`. Menggunakan `strtok` dengan delimiter `\t` (tab) untuk mengambil <b><i>filepath</i></b>, <b>publisher</b>, dan <b>tahun publikasi</b>. Lalu dipecah lagi dengan delimiter `/` dan `.` sehingga bisa diperoleh ekstensi <i>file</i>-nya. Mengkombinasi `strcpy` dan `strcat` untuk mengeluarkan output sesuai dengan permintaan soal. Setelah selesai, server akan mengirimkan pesan `DONE`. Tidak lupa mengubah <i>value</i> `flag` agar kembali ke proses <i>command</i>.
  
  Proses <i>client</i> ketika soal [soal.1f](#1f "Goto 1f") dijalankan.
  
  `client.c`
  ```C
  else {
       scanf("%[^\n]%*c", message);
       send(sock, message, strlen(message), 0);
  
       if (strcmp(message, "see") == 0) {
            do {
                 bzero(buffer,1024);
                 valread = read(sock, buffer, 1024);
                
                 if (strcmp(buffer, "DONE") != 0) {
                      printf("%s\n", buffer);
                      strcpy(message, "OK");
                      send(sock, message, strlen(message) , 0);
                 }
            } while(strcmp(buffer, "DONE") != 0);

            strcpy(message, "OK");
            send(sock, message, strlen(message), 0);
       }
  
  . . .
  
  }
  ```
  Saat menerima input `see`, <i>client</i> akan menerima pesan terus menerus dari <i>server</i>. Proses ini berakhir ketika <i>client</i> mengirimkan pesan `DONE`.

- <b>OUTPUT</b>
  
  <b><i>Command SEE dijalankan</i></b>
  
  <img src="https://user-images.githubusercontent.com/37539546/119263090-dccac000-bc07-11eb-9ac4-c747dcfb0c42.png" width="640" height="480">
  
### 1G ###

- <b>SOAL</b>

  Aplikasi <i>client</i> juga dapat melakukan pencarian dengan memberikan suatu <i>string</i>. Hasilnya adalah semua nama <i>file</i> yang mengandung <i>string</i> tersebut. Format output seperti nomor (f).
  
  <i>Command client</i>:
  ```
  find TEMP
  ```

- <b>JAWABAN</b>
  
  Pengondisian <i>command</i> `see`.
  
  `server.c`
  ```C
  if (flag == 6) { //COMMAND "FIND"
       FILE *filein;
       filein = fopen("files.tsv", "r");

       char temp[1000] = {0};
       char filePath[100] = {0}, publisher[100] = {0}, year[100] = {0}, name[100] = {0}, ext[100] = {0};
            
       while ((fscanf(filein, "%[^\n]%*c", temp)) != EOF) {
            char *token = strtok(temp, "\t");

            if (token != NULL) {
                 strcpy(filePath, token);
                 token = strtok(NULL, "\t");
            }

            if (token != NULL) {
                 strcpy(publisher, token);
                 token = strtok(NULL, "\t");
            }

            if (token != NULL) {
                 strcpy(year, token);
            }

            token = strtok(temp, "/");
            token = strtok(NULL, ".");
            strcpy(name, token);
            token = strtok(NULL, ".");
            strcpy(ext, token);

            char *ret =strstr(filePath, parameter[1]);

            if (ret) {
                 strcpy(message, "Nama: ");
                 strcat(message, name);
                 strcat(message, "\n");
                 strcat(message, "Publisher: ");
                 strcat(message, publisher);
                 strcat(message, "\n");
                 strcat(message, "Tahun publishing: ");
                 strcat(message, year);
                 strcat(message, "\n");
                 strcat(message, "Ekstensi: ");
                 strcat(message, ext);
                 strcat(message, "\n");
                 strcat(message, "Filepath: ");
                 strcat(message, filePath);
                 strcat(message, "\n");
                 send(*new_socket, message, strlen(message), 0 );

                 valread = recv(*new_socket, buffer, 1000, 0);
            }
       }

       strcpy(message, "DONE");
       send(*new_socket, message, strlen(message), 0 );
       valread = recv(*new_socket, buffer, 1000, 0);
            
       fclose(filein);

       flag = 1;
  }
  ```
  <i>Server</i> melakukan pembacaan terhadap database `files.tsv` baris per baris dengan `fscanf(filein, "%[^\n]%*c", temp)) != EOF`. Menggunakan `strtok` dengan delimiter `\t` (tab) untuk mengambil <b><i>filepath</i></b>, <b>publisher</b>, dan <b>tahun publikasi</b>. Lalu dipecah lagi dengan delimiter `/` dan `.` sehingga bisa diperoleh ekstensi <i>file</i>-nya. Mengkombinasi `strcpy` dan `strcat` untuk mengeluarkan output sesuai dengan permintaan soal. Setelah selesai, server akan mengirimkan pesan `DONE`. Tidak lupa mengubah <i>value</i> `flag` agar kembali ke proses <i>command</i>.
  
   Proses <i>client</i> ketika [soal.1g](#1g "Goto 1g") dijalankan.
  
  `client.c`
  ```C
  else {
  
       . . .
  
       char *ret = strstr(message, "find");
       
       if (ret) {
            do {
                 bzero(buffer, 1024);
                 valread = read(sock, buffer, 1024);
                
                 if (strcmp(buffer, "DONE") != 0) {
                      printf("%s\n", buffer);
                      strcpy(message, "OK");
                      send(sock, message, strlen(message), 0);
                 }
            } while(strcmp(buffer, "DONE") != 0);

            strcpy(message, "OK");
            send(sock, message, strlen(message), 0);
       }
  }	    
  ```
  Saat menerima input `find` (dapat dicek menggunakan `strstr`), <i>client</i> akan menerima pesan terus menerus dari <i>server</i>. Proses ini berakhir ketika <i>client</i> mengirimkan pesan `DONE`.

- <b>OUTPUT</b>
  
  <b><i>Command FIND dijalankan</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119263118-eeac6300-bc07-11eb-92c6-11dd869df187.png" width="640" height="480">
  
### 1H ###

- <b>SOAL</b>

  Dikarenakan Keverk waspada dengan pertambahan dan penghapusan <i>file</i> di <i>server</i>, maka Keverk membuat suatu log untuk <i>server</i> yang bernama `running.log`. 
  
  Isi dari log:
  ```
  Tambah : File1.ektensi (id:pass)
  Hapus : File2.ektensi (id:pass)
  ```
- <b>JAWABAN</b>

  Pendeklarasian variabel global untuk menyimpan <b>ID</b> dan <i><b>Password</b> Login</i>.
  
  `server.c`
  ```C
  if (login == 0) {
       if (found) {
       
       . . .     
       
            strcpy(id_login, id);
            strcpy(password_login, password);
       }
       
       . . .
  }
  ```
  Server memiliki variabel global `id_login` dan `password_login` untuk menyimpan <b>ID</b> dan <i><b>Password</b></i> dari <i>client</i> yang berhasil melakukan <i>login</i>.
  
  Proses pembuatan log jika melakukan penambahan <i>file</i>.
  
  `server.c`
  ```C
  if (flag == 2) { //COMMAND "ADD"
  
  . . .
  
       if (found) {
            
	    . . .
	    
            FILE *fileout;
            fileout = fopen("files.tsv", "a");
       
            . . .
       
            fileout = fopen("running.log", "a");
            fprintf(fileout, "Tambah : %s (%s:%s)\n", fileName, id_login, password_login);
            fclose(fileout);
  }
  ```
  Saat <i>server</i> sudah selesai menerima <i>file</i> dari <i>client</i>, maka akan membuka `running.log` dan menggunakan <i>command</i> `a` agar data yang diinput ditambahkan di bagian akhir teks. Kemudian, menulis log dengan menggunakan `fprintf` sesuai format yang diminta soal. <b>ID</b> dan <i><b>Password</b> Client</i> diperoleh dari variabel global `id_login` dan `password_login`
  
  Proses pembuatan log jika melakukan penghapusan <i>file</i>.
  
  `server.c`
  ```C
  if (flag == 4) { //COMMAND "DELETE"
  
  . . .
  
       FILE *fileout;
       fileout = fopen("files.tsv", "a");
       
       . . .
       
       fileout = fopen("running.log", "a");
       fprintf(fileout, "Hapus : %s (%s:%s)\n", fileName, id_login, password_login);
       fclose(fileout);
  }
  ```
  Saat <i>server</i> sudah selesai menghapus <i>file</i>, maka akan membuka `running.log` dan menggunakan <i>command</i> `a` agar data yang diinput ditambahkan di bagian akhir teks. Kemudian, menulis log dengan menggunakan `fprintf` sesuai format yang diminta soal. <b>ID</b> dan <i><b>Password</b> Client</i> diperoleh dari variabel global `id_login` dan `password_login`.

- <b>OUTPUT</b>
  
  <b><i>running.log</i></b>

  <img src="https://user-images.githubusercontent.com/37539546/119263137-008e0600-bc08-11eb-93d6-3470d7638a69.png" width="640" height="480">

### KENDALA ###
 
- Kesulitan sewaktu <i>debug</i> `server.c` maupun `client.c`.
- Tingkat kesulitan soal seperti membuat CRUD.
- Kehabisan waktu saat praktikum karena mengerjakan 2 nomor dan saat itu juga pekan ETS.
- <i>Error segmentation fault</i> :(

## SOAL 2 ##

### 2A ###

- <b>SOAL</b>

  Membuat program perkalian matriks (4 × 3 dengan 3 × 6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1 - 20 (tidak perlu dibuat filter angka).

- <b>JAWABAN</b>

  Perkalian matriks ini akan ada variabel yang mengindikasikan baris pertama dan kolom pertama pada pengulangan pertama yang dijadikan sebagai <i>counter</i> sesuai dengan syarat perkalian matriks.
  
  ```C
  int matriksX[4][3] = {
       {1, 1, 1},
       {1, 2, 3},
       {1, 1, 1},
       {2, 2, 2}
  };

  int matriksY[3][6] = {
       {1, 2, 3, 4, 5, 6},
       {1, 0, 1, 0, 1, 0},
       {1, 2, 4, 4, 1, 2}
  };
  
  int matriksZ[4][6];
  ```
  Pendefinisian tiga matriks yaitu `matriksX` (4 × 3), `matriksY` (3 × 6), dan `matriksZ` (4 × 6) sebagai matriks hasil perkalian. <i>Value</i> di-<i>set</i> dengan interval 1 - 20 sesuai dengan permintaan soal.
  
  Kemudian, mendefinisikan <i>struct</i>. 
  ```C
  struct args {
       int i;
       int j;
  };
  ```
  Di sini `i` sebagai baris dan `j` sebagai kolom.
  
  Membuat fungsi perkalian untuk menghitung matriks.
  ```C
  void *perkalian(void *arg) {
       int i = ((struct args*) arg) -> i;
       int j = ((struct args*) arg) -> j;

       for (int k = 0; k < 3; k++) {
            matriksZ[i][j] += matriksX[i][k] * matriksY[k][j];
       }
  }
  ```
  - Fungsi perkalian menggunakan `arg` sebagai argumennya. `((struct args*) arg) -> i` untuk memasukkan baris dari tiap matriks ke dalam variabel `i` dan `((struct args*) arg) -> j` untuk memasukkan kolom dari tiap matriks ke dalam variabel `j`.
  - Pada <i>looping</i>, membuat sebuah variabel `k` yang menjadi nilai kesamaan ordo matriks (3 kolom di matriks pertama dan 3 baris di matriks kedua) yang akan mengulang sebanyak 3 kali.
  - Perkalian dilakukan dengan mengalikan setiap baris di `matriksX` dengan setiap kolom di `matriksY`, jadi `k` mengindikasikan baris pertama dan kolom pertama pada pengulangan pertama dan seterusnya.
  - Hasil perkalian akan ditambahkan dan dimasukkan ke dalam `matriksZ`. Pada <i>case</i> ini ordo matriks hasil adalah (4 × 6), karena ordo matriks hasil perkalian dua buah matriks adalah jumlah baris pertama dikali jumlah kolom ke dua.
  
  Fungsi main untuk penyelesaian soal menggunakan <i>thread</i>.
  ```C
  int main() {
       pthread_t tid[4][6];

       for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                 struct args *index = (struct args*) malloc(sizeof(struct args));
                 index -> i = i;
                 index -> j = j;
                 pthread_create(&tid[i][j], NULL, &perkalian, (void *)index);
            }
       }

       for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 6; j++) {
                 pthread_join(tid[i][j], NULL);
            }
       }
  
  . . .
  
  ```
  - Mendefinisikan `tid` dan `pthread` dengan jumlah ordo matriks hasil dan sebuah <i>struct</i> yang berisi atribut `index`.
  - <i>Looping</i> pertama sebagai indikasi baris dan <i>looping</i> kedua sebagai indikasi kolom yang setiap indikasi baris dan kolom tersebut di-<i>set</i> ke `i` dan `j`.
  - <i>Thread</i> akan dibuat dengan `pthread_create(&tid[i][j], NULL, &perkalian, (void *)index)` dan berjalan dengan `tid` `i` dan `j` yang di-<i>increment</i> tiap <i>looping</i>-nya. <i>Thread</i> akan menjalankan fungsi `perkalian` sebagai <i>routine</i> dengan atribut `index` sebagai variabel.
  - <i>Join</i>-kan setiap <i>thread</i> yang telah dibuat dengan `pthread_join(tid[i][j], NULL)`.

  Mencetak matriks hasil.
  ```C
  printf("Hasil Matriks: \n");

  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            printf("%4d", matriksZ[i][j]);
       }
       printf("\n");
  }
  ```
  <i>Print</i> dengan `printf("%4d", matriksZ[i][j])` untuk tiap baris dan kolom menggunakan <i>counter</i> `i` dan `j` dan <i>increment</i> sebanyak jumlah baris, yaitu 4 dan jumlah kolom, yaitu 6. `%4d` di sini adalah untuk banyak karakter <b>integer</b> yang akan dicetak dari matriks `matriksZ[i][j]`.
  
  Membuat <i>shared memory</i> untuk `matriksC` sesuai dengan <i>template</i> yang ada di modul 3.
  ```C
  key_t key = 1234;
  int *value;

  int shmid = shmget(key, sizeof(matriksZ), IPC_CREAT | 0666);
  value = shmat(shmid, NULL, 0);

  int *p = (int *) value;

  memcpy(p, matriksZ, 95);

  shmdt(value);
  ```
  Membuat terlebih dahulu <i>key</i>-nya. <i>Pointer</i> `p` menunjuk tiap isi array matriks dan dilakukan penyalinan data dengan `memcpy` melalui variabel <i>pointer</i> `p` terhadap variabel matriks hasil agar variabel matriks hasil yang memorinya di-<i>share</i> berisi hasil matriks yang telah dikalikan. Dan untuk mengakhiri <i>shared memory</i> yang sedang berlangsung digunakan `shmctl`.

- <b>OUTPUT</b>

  <img src="https://user-images.githubusercontent.com/37539546/119262248-b6efec00-bc04-11eb-8a77-c89e05a7f159.png" width="640" height="480">
  
### 2B ###

- <b>SOAL</b>

  Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (<b>Catatan!</b>: gunakan <i>shared memory</i>). Kemudian matriks tersebut akan dilakukan perhitungan dengan matriks baru sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap <i>cell</i> yang berasal dari matriks A menjadi angka untuk faktorial, lalu <i>cell</i> dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) (<b>Catatan!</b>: gunakan <i>thread</i> untuk perhitungan di setiap <i>cell</i>).
  
  Ketentuan:
  ```
  If a >= b -> a! / (a - b)!
  If b > a -> a!
  If 0 -> 0
  ```
  
- <b>JAWABAN</b>
  
  Membuat matriks yang akan digunakan sebagai <i>shared memory</i> dan thread untuk setiap nilai dari matriks hasil yang akan berjalan dengan <i>routine</i> faktorial.
  ```C
  int matriks[4][6];

  unsigned long long result[4][6];
  
  int matriksInput[4][6] = {
       {1, 2, 3, 4, 5, 6},
       {1, 1, 1, 1, 1, 1},
       {2, 2, 2, 2, 2, 2},
       {1, 0, 1, 0, 1, 0}
  };
  ```
  Pendefinisian tiga matriks yaitu `matriks` (4 × 6) sebagai matriks hasil perkalian soal sebelumnya, `matriksInput` (4 × 6) sebagai matriks yang digunakan untuk proses penghitungan faktorial, dan `result` (4 × 6) sebagai matriks hasil faktorial. <i>Value</i> di-<i>set</i> dengan interval 1 - 20 sesuai dengan permintaan soal.
  
  Kemudian, mendefinisikan <i>struct</i>. 
  ```C
  struct args {
       int i;
       int j;
       int matriksA;
       int matriksB;
  };
  ```
  Di sini `i` sebagai baris, `j` sebagai kolom, `matriksA` sebagai matriks yang menyimpan matriks hasil perkalian [soal.2a](#2a "Goto 2a"), dan `matriksB` sebagai matriks yang digunakan untuk proses penghitungan faktorial.
  
  Membuat fungsi faktorial.
  ```C
  unsigned long long faktorial(int n) {
       if (n == 0) {
            return 1;
       }

       else {
            return n * faktorial(n - 1);
       }
  }
  ```
  Menerima inputan dari parameter `n`, jika `n` bernilai 0, maka hasil faktorialnya 1, jika selain itu akan melakukan proses penghitungan secara iteratif sampai `n` bernilai 0.
  
  Membuat fungsi faktorial dengan pengondisian seperti yang diminta pada contoh di soal.
  ```C
  void *faktorial2(void *arg) {
       struct args *index = (struct args *) arg;
       int a = index -> matriksA;
       int b = index -> matriksB;

       int i = index -> i;
       int j = index -> j;

       if (a == 0 | b == 0) {
            result[i][j] = 0;
       }

       else if (a >= b) {
            result[i][j] = faktorial(a) / faktorial(a - b);
       }

       else if (b > a) {
            result[i][j] = faktorial(a);
       }
  }
  ```
  - `i` dan `j` di-<i>set</i> dengan baris dan kolom matriks. Lalu `matriksA` dan `matriksB` masing-masing di-<i>set</i> ke `a` dan `b`.
  - Jika `a` dan `b` bernilai 0, <i>value</i> matriks `result[i][j]` akan bernilai 0.
  - Jika `a` lebih besar sama dengan `b`, matriks `result[i][j]` akan bernilai hasil pembagian `faktorial(a)` dan `faktorial(a - b)`.
  - Jika `b` lebih besar dari `a`, <i>value</i> matriks `result[i][j]` akan bernilai `faktorial(a)`.
  
  Membuat <i>shared memory</i> untuk `matriks` sesuai dengan <i>template</i> yang ada di modul 3.
  ```C
  int main() {
       key_t key = 1234;
       int (*value)[6];

       int shmid = shmget(key, 95, IPC_CREAT | 0666);
       value = shmat(shmid, NULL, 0);

       int *p = (int *) value;

       memcpy(matriks, p, 95);
  
  . . .
  
  ```
  Membuat terlebih dahulu <i>key</i>-nya. <i>Pointer</i> `p` menunjuk tiap isi array matriks dan dilakukan penyalinan data dengan `memcpy` melalui variabel <i>pointer</i> `p` terhadap variabel matriks agar variabel matriks berisi hasil matriks yang telah dikalikan pada soal sebelumnya.
  
  Fungsi main untuk penyelesaian soal menggunakan <i>thread</i>.
  ```C
  pthread_t tid[4][6];

  printf("Matriks: \n");
  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            printf("%4d", matriks[i][j]);
       }

       printf("\n");
  }

  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            struct args *index = (struct args *) malloc(sizeof(struct args));
            index -> i = i;
            index -> j = j;
            index -> matriksA = matriks[i][j];
            index -> matriksB = matriksInput[i][j];

  pthread_create(&tid[i][j], NULL, &faktorial2, (void *)index);
       }
  }

  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            pthread_join(tid[i][j], NULL);
       }
  }
  ```
  - Mendefinisikan `tid` dan `pthread` dengan jumlah ordo matriks hasil dan sebuah <i>struct</i> yang berisi atribut `index`.
  - Me-<i>loop</i> matriks yang menyimpan matriks hasil perkalian dari [soal.2a](#2a "Goto 2a") untuk mengecek apakah matriks ini mempunyai <i>value</i> yang sama dengan matriks hasil soal sebelumnya (<i>debugging</i>).
  - <i>Looping</i> pertama sebagai indikasi baris dan <i>looping</i> kedua sebagai indikasi kolom yang setiap indikasi baris dan kolom tersebut di-<i>set</i> ke `i` dan `j`. Lalu `matriks[i][j]` dan `matriksInput[i][j]` masing-masing di-<i>set</i> ke `matriksA` dan `matriksB`.
  - <i>Thread</i> akan dibuat dengan `pthread_create(&tid[i][j], NULL, &perkalian, (void *)index)` dan berjalan dengan `tid` `i` dan `j` yang di-<i>increment</i> tiap <i>looping</i>-nya. <i>Thread</i> akan menjalankan fungsi `faktorial2` sebagai <i>routine</i> dengan atribut `index` sebagai variabel.
  - <i>Join</i>-kan setiap <i>thread</i> yang telah dibuat dengan `pthread_join(tid[i][j], NULL)`.

  Mencetak matriks hasil.
  ```C
  printf("Hasil Matriks: \n");
  
  for (int i = 0; i < 4; i++) {
       for (int j = 0; j < 6; j++) {
            printf("%4llu ", result[i][j]);
       }
       printf("\n");
  }
  
  printf("\n");
  
  shmdt(value);
  shmctl(shmid, IPC_RMID, NULL);
  ```
  <i>Print</i> dengan `printf("%4llu ", result[i][j])` untuk tiap baris dan kolom menggunakan <i>counter</i> `i` dan `j` dan <i>increment</i> sebanyak jumlah baris, yaitu 4 dan jumlah kolom, yaitu 6. `%4llu` di sini adalah untuk banyak karakter <b>unsigned long long</b> yang akan dicetak dari matriks `result[i][j]`. Dan untuk mengakhiri <i>shared memory</i> yang sedang berlangsung digunakan `shmctl`.

- <b>OUTPUT</b>

  <img src="https://user-images.githubusercontent.com/37539546/119262434-704ec180-bc05-11eb-81d6-0170652173a9.png" width="640" height="480">
  
### 2C ###

- <b>SOAL</b>

  Karena takut <i>lag</i> dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan <i>resource</i> komputernya dengan command `ps aux | sort -nrk 3,3 | head -5` (<b>Catatan!</b>: Harus menggunakan <i>IPC Pipes</i>).

- <b>JAWABAN</b>
  
  Melakukan <i>fork</i> yang mana <i>parent process</i> akan menjadi `write end` dari <i>pipe</i> dan <i>child</i> akan menjadi `read end` dari output <i>parent process</i>.
  ```C
  int main(int argc, char **argv) {
       pid_t child_id;
    
       int fd[4];
       int status;
       char *ps_aux_arg[] = {"ps", "aux", NULL};
       char *sort_arg[] = {"sort", "-nrk", "3,3", NULL};
       char *head_arg[] = {"head", "-5", NULL};

       pipe(fd);
       pipe(fd + 2);
  
  . . .
  
  ```
  - Men-<i>set</i> `child_id` untuk <i>thread</i> dengan <b>pid_t</b> dan dua <i>file descriptor</i> yaitu `fd` sebagai `read end` dan `fd + 2` sebagai `write end`.
  - Mendefinisikan <i>array of pointer</i> `*ps_aux_arg[]` untuk menyimpan <i>list string</i> perintah `ps aux`.
  - Mendefinisikan <i>array of pointer</i> `*sort_arg[]` untuk menyimpan <i>list string</i> perintah `sort -nrk 3,3`.
  - Mendefinisikan <i>array of pointer</i> `*head_arg[]` untuk menyimpan <i>list string</i> perintah `head -5`.
  
  Membuat <i>process</i> yang digunakan untuk `ps aux`. Di sini menggunakan <b>STDOUT_FILENO</b> (<i>standard output value</i>) yang mempunyai <i>value</i> 1. 
  ```C
  child_id = fork();
        
  if (child_id == 0) {
       dup2(fd[1], STDOUT_FILENO);

       close(fd[0]);
       close(fd[1]);
       close(fd[2]);
       close(fd[3]);

       execvp(*ps_aux_arg, ps_aux_arg);
  }
  ```
  - Melakukan <i>fork</i> untuk <i>parent process</i>-nya sehingga membuat <i>copy</i>-an `fd[1]` yang berfungsi sebagai `write end` dari <i>pipe</i> dengan <b>STDOUT</b>.
  - <i>Close</i> semua <i>file descriptor</i>.
  - `ps aux` akan dijalankan untuk menampilkan semua proses yang dijalankan oleh pengguna dengan menggunakan `execvp` yang argumen pertamanya menerima <i>array of pointer</i> `*ps_aux_arg` dan argumen kedua menerima <i>pointer to array of pointer</i> `ps_aux_arg` yang berisi <i>list string</i> perintah. <i>Parent process</i> hanya akan berjalan sekali.
  
  Membuat <i>process</i> yang digunakan untuk `sort -nrk 3,3`. Di sini menggunakan <b>STDIN_FILENO</b> (<i>standard input value</i>) yang mempunyai <i>value</i> 0 dan <b>STDOUT_FILENO</b> (<i>standard output value</i>) yang mempunyai <i>value</i> 1. 
  ```C
  else {
       child_id = fork();
        
       if (child_id == 0) {
           dup2(fd[0], STDIN_FILENO);
           dup2(fd[3], STDOUT_FILENO);

           close(fd[0]);
           close(fd[1]);
           close(fd[2]);
           close(fd[3]);

           execvp(*sort_arg, sort_arg);
       }
  
  . . .
  
  ```
  - Melakukan <i>fork</i> untuk <i>child process</i>-nya sehingga membuat <i>copy</i>-an `fd[0]` yang berfungsi sebagai input atau `read end` hasil `ps aux` dari <i>pipe</i> dengan <b>STDIN</b> dan `fd[3]` yang berfungsi sebagai `write end` dari <i>pipe</i> dengan <b>STDOUT</b>.
  - <i>Close</i> semua <i>file descriptor</i>.
  - `sort -nrk 3,3` akan dijalankan untuk menampilkan semua proses yang dijalankan oleh pengguna dengan menggunakan `execvp` yang argumen pertamanya menerima <i>array of pointer</i> `*sort_arg` dan argumen kedua menerima <i>pointer to array of pointer</i> `sort_arg` yang berisi <i>list string</i> perintah. <i>Child process</i> hanya akan berjalan sekali.
  
  Membuat <i>process</i> yang digunakan untuk `head -5`. Di sini menggunakan <b>STDIN_FILENO</b> (<i>standard input value</i>) yang mempunyai <i>value</i> 0.
  ```C
  else {
       child_id = fork();
            
       if (child_id == 0) {
            dup2(fd[2], STDIN_FILENO);

            close(fd[0]);
            close(fd[1]);
            close(fd[2]) 
            close(fd[3]);

            execvp(*head_arg, head_arg);
       }
  }
  ```
  - Melakukan <i>fork</i> untuk <i>child process</i>-nya sehingga membuat <i>copy</i>-an `fd[2]` yang berfungsi sebagai input atau `read end` hasil `sort -nrk 3,3` dari <i>pipe</i> dengan <b>STDIN</b>.
  - <i>Close</i> semua <i>file descriptor</i>.
  - `head -5` akan dijalankan untuk menampilkan 5 proses teratas dengan menggunakan `execvp` yang argumen pertamanya menerima <i>array of pointer</i> `*head_arg` dan argumen kedua menerima <i>pointer to array of pointer</i> `head_arg` yang berisi <i>list string</i> perintah. <i>Child process</i> hanya akan berjalan sekali.

  Setelah semua <i>process</i> dilakukan, <i>close</i> semua <i>file descriptor</i>.
  ```C
  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]);
  
  for (int i = 0; i < 3; i++) {
       wait(&status);
  }
  ```
  Terakhir, melakukan looping dari 0 sampai 2 (sesuai jumlah <i>process</i>) untuk melakukan proses `wait` untuk menunda eksekusi <i>process</i> yang berjalan hingga salah satu <i>child</i>-nya <i>terminate</i>.

- <b>OUTPUT</b>

  <img src="https://user-images.githubusercontent.com/37539546/119262474-92e0da80-bc05-11eb-86a4-6e87deae1b91.png" width="640" height="480">

### KENDALA ###
 
- Deskripsi pada [soal.2b](#2b "Goto 2b") agak membingungkan, ternyata menggunakan matriks baru untuk menghitung faktorial matriks hasil perkalian dari [soal.2a](#2a "Goto 2a").
- Program [soal.2c](#2c "Goto 2c") tidak jalan ketika memakai <i>template pipes</i>, entah kenapa. Terpaksa memakai <i>template</i> yang ada di internet.
- Bingung untuk <i>shared memory</i> pada <i>array</i> 2D. 

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
  
- <b>JAWABAN</b>
  
  Untuk melakukan perintah seperti di atas bisa dilakukan dengan cara seperti berikut:
  ```C
  if (argc > 2 && strcmp(argv[1], "-f") == 0)
  {
       pthread_t tid[argc - 2];
       int count = 0;
       
       for (int i = 2; i < argc; i++)
       {
       
            if (access(argv[i], F_OK) == 0)
            {
	             pthread_create(&tid[count], NULL, kategori, (void *)argv[i]);
	             count++;
		       printf("File %d : Berhasil Dikategorikan\n", i-1);
            }

            else 
            {
                 printf("File %d : Sad, gagal :(\n", i-1);
            }
       }
       
       for (int i = 0; i < count; i++) 
       {
            pthread_join(tid[i], NULL);
       }
       
       return 0;
  }
  ```
  
  Di sini pertama kali yang harus dilakukan yaitu menentukan kondisinya dulu yang mana kondisinya dilakukan minimal tiga argumen yaitu `./soal3` untuk eksekusi <i>file</i> .c, kemudian `-f` untuk menentukan perintah yang akan dilakukan dan terakhir <i>path</i> dari <i>file</i> yang ingin dikategorikan. Kemudian dengan menggunakan fungsi `access`, program akan mengakses <i>path file</i> yang akan dikategorikan dengan fungsi `kategori` dan dengan menggunakan perintah `pthread_create` untuk membuat <i>thread</i> yang akan menjalankan perintah tersebut.
  ```C
  void* kategori(void *arg)
  {
       char *asal = (char *)arg;
       char asalpath[150];
       memcpy(asalpath, (char*)arg, 400);
       char *asalTipe = cekTipe(asal);
       char tipe[400];
       strcpy(tipe, asalTipe);

       DIR *dir = opendir(asalTipe);
       if (dir)
       {
            closedir(dir);
       }

       else if (ENOENT == errno) 
       {
            mkdir(asalTipe, 0755);
       }

       char *srcName = cekNama(asalpath);
       char *curr = getenv("PWD");

       char pathTujuan[400];
       sprintf(pathTujuan, "%s/%s/%s", curr, tipe, srcName);
       rename(asalpath, pathTujuan);
  }
  ```
  
  Dalam fungsi kategori ini yang pertama dilakukan adalah melakukan pengecekan tipe <i>file</i>-nya berdasarkan ekstensinya dengan menggunakan fungsi `cekTipe`.
  ```C
  char *cekTipe(char *dir)
  {
       char *unknown = {"Unknown"};
       char *hidden = {"Hidden"};
       char *tmp = strrchr(dir, '/'); 
       
       if (tmp[1] == '.')
       {
            return hidden;
       }

       int i = 0;
       
       while (i < strlen(tmp) && tmp[i] != '.')
       {
            i++;
       }
       
       if (i == strlen(tmp))
       {
            return unknown;
       }

       char tipe[400];
       int j = i;
       
       while (i < strlen(tmp))
       {
            tipe[i-j] = tmp[i], i++;
       }
       
       return hrfKecil(tipe + 1);
  }
  ```
  
  Fungsi ini akan mengecek nama <i>file</i> terakhir di <i>path</i> dengan menggunakan perintah `strrchr`. Perintah tersebut akan mengembalikan <i>string</i> dengan `/` terakhir. Kemudian dari situ akan dicek apabila setelah `/` terdapat `.`, maka <i>file</i> tersebut merupakan <i>file hidden</i> dan fungsi akan mengembalikan string `hidden`. Apabila setelah tulisan tidak terdapat `.` maka akan mengembalikan <i>string</i> `unknown`. Dan terakhir apabila menemukan titik setelah <i>string</i> maka akan memanggil fungsi `hrfkecil` untuk mengecilkan huruf tipe <i>file</i>-nya yang kemudian akan dikembalikan ke fungsi `cekTipe`.
  
  Kemudian setelah fungsi `kategori` mendapatkan <i>string</i> dari fungsi `cekTipe `, selanjutnya akan mengecek apabila folder tersebut sudah ada maka dengan perintah `sprintf` akan mengubah <i>path</i> dari <i>file</i> tersebut ke dalam <i>path</i> yang baru yaitu <i>path</i> ke folder dengan tipe data yang sudah ditentukan.
  
- <b>OUTPUT</b>
  
  <b><i>Kondisi sebelum di-<i>run</i></i></b>
  
  <img src="https://i.postimg.cc/0ykK5BMq/3asebelum.png)" width="640" height="480">
  
  <b><i>Pada saat di-<i>run</i> di terminal akan muncul seperti ini</i></b>
  
  <img src="https://i.postimg.cc/jdQJbbjX/3adirun.png)">
  
  <b><i>Kondisi setelah di-<i>run</i>, <i>file</i> `bs.hex` dan `Pledge.pdf` sudah ada di dalam folder masing-masing</i></b>
  
  <img src="https://i.postimg.cc/1zYvr6hf/3asesudah.png)">
  
  <b><i>Isi folder .hex</i></b>
  
  <img src="https://i.postimg.cc/VLcDgmTB/3asesudah1.png)">
  
  <b><i>Isi folder .pdf</i></b>
  
  <img src="https://i.postimg.cc/nrfY3Gdc/3asesudah2.png)">
  
### 3B ###

- <b>SOAL</b>

  Program juga dapat menerima opsi `-d` untuk melakukan pengkategorian pada suatu <i>directory</i>. Namun pada opsi `-d` ini, user hanya bisa memasukkan input satu <i>directory</i> saja, tidak seperti <i>file</i> yang bebas menginput <i>file</i> sebanyak mungkin. Contohnya adalah seperti ini:
  ```
  $ ./soal3 -d /path/to/directory/
  ```
  
  Perintah di atas akan mengkategorikan file di `/path/to/directory`, lalu hasilnya akan disimpan di <i>working directory</i>, tempat program C tersebut berjalan (hasil kategori filenya bukan di `/path/to/directory`).

  Output yang dikeluarkan:
  ```
  Jika berhasil, print "Direktori sukses disimpan!"
  Jika gagal, print "Yah, gagal disimpan :("
  ```
- <b>JAWABAN</b>
  
  Untuk [soal.3b](#3b "Goto 3b") langkahnya sama seperti [soal.3a](#3a "Goto 3a"), namun memiliki fungsi yang berbeda yaitu fungsi `kategoriFolder`.
  ```C
  void kategoriFolder(char *folderPath, int threadSize)
  {
       DIR *fd = opendir(folderPath);
       struct dirent *dp;
       pthread_t tid[threadSize];
       int count = 0;
       char fileName[400][400];

       while ((dp = readdir(fd)) != NULL)
       {
            if (dp->d_type == DT_REG)
            {
                 sprintf(fileName[count], "%s/%s", folderPath, dp->d_name);
                 pthread_create(&tid[count], NULL, kategori, (void *)fileName[count]);
                 count++;
            }
            
	    else if ((dp->d_type == DT_DIR) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
            {
                 char folderPath2[400];
                 sprintf(folderPath2, "%s/%s", folderPath, dp->d_name);
                 DIR *fd2 = opendir(folderPath2);
                 struct dirent *dp2;
            
	         int threadSize2 = 0;
            
	         while ((dp2 = readdir(fd2)) != NULL)
                 {
                      if (dp2->d_type == DT_REG)
                      {
                           threadSize2++;
                      }
                 }
            
	         kategoriFolder(folderPath2, threadSize2);
            
	         closedir(fd2);
            }
       }

       for (int i = 0; i < threadSize; i++)
       {
            pthread_join(tid[i], NULL);
       }
      
      closedir(fd);
  }
  ```
  
  [soal.3b](#3b "Goto 3b") diminta untuk mengkategorikan <i>file</i>-<i>file</i> yang ada di dalam folder. Untuk mengkategorikannya menggunakan fungsi `kategori` seperti [soal.3a](#3a "Goto 3a"), namun untuk mengubah <i>path</i>-nya. Menggunakan rekursi untuk mengeluarkan <i>file</i>-<i>file</i> yang ada di dalam folder tersebut dan mengkategorikannya berdasarkan ekstensi <i>file</i> dengan cara yang sama dengan [soal.3b](#3b "Goto 3b") yaitu mengubah <i>path</i> dari <i>file</i> tersebut.
  
- <b>OUTPUT</b>
  
  <b><i>Folder sebelum dieksekusi</i></b>
  
  <img src="https://i.postimg.cc/V6CdHyTX/3bsebelum.png)">
  
  <b><i>Saat dieksekusi, maka di terminal akan muncul seperti ini</i></b>
  
  <img src="https://i.postimg.cc/CxcBRskn/3bdirun.png)">
  
  <b><i>Sesudah dieksekusi, maka akan muncul folder .jpg baru yang berasal dari folder sebelumnya</i></b>
  
  <img src="https://i.postimg.cc/nLMsB9GR/3bsesudah.png)">
  
  <b><i>Isi dari folder .jpg</i></b>
  
  <img src="https://i.postimg.cc/s28McfnV/3bsesudah2.png)">

### 3C ###

- <b>SOAL</b>

  Selain menerima opsi-opsi di atas, program ini menerima opsi `\*`, contohnya ada di bawah ini:
  ```
  $ ./soal3 \*
  ```
  
  Opsi ini akan mengkategorikan seluruh <i>file</i> yang ada di <i>working directory</i> ketika menjalankan program C tersebut.
  
- <b>JAWABAN</b>
  
  Untuk menjalankan perintah tersebut, dilakukan sebagai berikut:
  ```C
  else if (argc == 2 && strcmp(argv[1], "*") == 0)
  {
       char *curr = getenv("PWD");
       DIR *dir = opendir(curr);
       struct dirent *dp;
       int threadSize = 0;
 
       while ((dp = readdir(dir)) != NULL)
       {
            if (dp->d_type == DT_REG)
            {
	         threadSize++;
	    }
        }
        
	kategoriFolder(curr, threadSize);
	
	closedir(dir);
  }
  ```
  
  Dengan cara yang sama dengan [soal.3b](#3b "Goto 3b"), namun di sini <i>path</i> yang digunakan adalah <i>path</i> folder yang ada saat ini sehingga folder yang ada di dalam <i>working directory</i> akan mengkategorikan seluruh <i>file</i> dan folder yang ada di dalam <i>working directory</i>.
  
- <b>OUTPUT</b>

  <b><i>Sebelum program dieksekusi</i></b>
  
  <img src="https://i.postimg.cc/wxD1XpDp/3csebelum.png))">
  
  <b><i>Sesudah program dieksekusi</i></b>
  
  <img src="https://i.postimg.cc/T1jhfVTw/3csesudah.png)">

### 3D ###

- <b>SOAL</b>

  Semua <i>file</i> harus berada di dalam folder, jika terdapat <i>file</i> yang tidak memiliki ekstensi, <i>file</i> disimpan dalam folder `Unknown`. Jika file <i>hidden</i>, masuk folder `Hidden`.
  
- <b>JAWABAN</b>
  
  Untuk menjalankan perintah tersebut menggunakan fungsi `cekTipe` yang sudah dijelaskan di [soal.3a](#3a "Goto 3a"), yang mana fungsi tersebut akan mengembalikan <i>string</i> `hidden` apabila memebuhi syarat dan akan mengembalikan <i>string</i> `unknown` apabila memenuhi syarat pula. Kemudian akan masuk ke fungsi `kategori` dengan membawa <i>string</i> `hidden` atau `unknown` yang akan diubah <i>path</i>-nya ke <i>path</i> folder yang sesuai.
  
- <b>OUTPUT</b>
  
  <b><i>Isi dari folder</i></b> `hidden`
  
  <img src="https://i.postimg.cc/BbftrY9k/3dhidden.png)">
  
  <b><i>Isi dari folder</i></b> `unknown`
  
  <img src="https://i.postimg.cc/wTH7FGks/3dunknown.png)">

### 3E ###

- <b>SOAL</b>

  Setiap 1 <i>file</i> yang dikategorikan dioperasikan oleh 1 <i>thread</i> agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

- <b>JAWABAN</b>
  
  Untuk menjalankan perintah tersebut digunakan <i>looping</i>.
  ```C
  while ((dp = readdir(fd)) != NULL)
  {
       if (dp->d_type == DT_REG)
       {
            threadSize++;
       }
  }
  ```
  <i>Looping</i> tersebut berfungsi untuk terus menambah <i>thread</i> pada setiap eksekusi <i>file</i> sehingga setiap <i>file</i> berjalan di satu <i>thread</i> dan program bisa berjalan dengan lebih efisien.

### KENDALA ###
 
- Kesulitan saat meng-<i>update path</i> baru.
- Kesulitan saat menjalankan 1 <i>file</i> 1 <i>thread</i>.
