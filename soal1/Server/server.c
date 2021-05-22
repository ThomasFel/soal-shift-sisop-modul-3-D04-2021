#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PORT 8080

pthread_t tid[1000], id_client_login;

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

void *operation(void *arg) {
    int *new_socket = (int *) arg; 
    int flag = 0;
    
    while (1) {
        char buffer[1024] = {0};
        char message[1024] = {0};
        char parameter[10][1024] = {0};
        int index = 0;
        int valread;

        //SOAL 1A
        if (flag == 0) { //LANDING PAGE
            strcpy(message, "1.Register\n2.Login\n\nInput: ");
            send(*new_socket, message, strlen(message), 0);
            valread = recv(*new_socket, buffer, 1024, 0); //RECEIVE MESSAGES FROM SOCKET
            
            if (checkClose(valread, new_socket)) {
                break;
            }

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
                        
                        strcpy(id_login, id);
                        strcpy(password_login, password);
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
        }
        
        if (flag == 1) { //COMMAND INPUT
            strcpy(message, "Command: ");
            send(*new_socket, message, strlen(message), 0);
            
            valread = recv(*new_socket, buffer, 100, 0);

            if (checkClose(valread, new_socket)) {
                break;
            }

            //SOAL 1B & 1C
            if (strcmp("add", buffer) == 0) {
                flag = 2;
            }
            
            //SOAL 1F
            else if (strcmp("see", buffer) == 0) {
                flag = 5;    
            }
            
            else {
                //SOAL 1D
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

                //SOAL 1E
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

                //SOAL 1G
                ret = strstr(buffer, "find");

                if (ret) {
                    char *token = strtok(buffer, " ");

                    while (token != NULL) {
                        strcpy(parameter[index], token);
                        index++;
                        token = strtok(NULL, " ");
                    }

                    flag = 6;
                }
            }
        }
        
        //SOAL 1B & 1C
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
            
            if (checkClose(valread,new_socket)) {
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
            fprintf(fileout,"%s\t%s\t%s\n", filePath, publisher, year);
            fclose(fileout);

            fileout = fopen("running.log", "a");
            fprintf(fileout,"Tambah : %s (%s:%s)\n", fileName, id_login, password_login);
            fclose(fileout);
        }
        
        //SOAL 1D
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

            //SEND FILE
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
            }
            
            else {
                strcpy(message, "ERROR! File not found.\n\n");
                send(*new_socket, message, strlen(message), 0);
            }

            flag = 1;
        }
        
        //SOAL 1E
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

                fileout = fopen("running.log", "a");
                fprintf(fileout, "Hapus : %s (%s:%s)\n", parameter[1], id_login, password_login);
                fclose(fileout);

                printf("DELETION DONE.\n");
                strcpy(message, "DELETE SUCCESS.\n\n");
                send(*new_socket, message, strlen(message), 0);
            }
            
            else {
                strcpy(message, "ERROR! File not found.\n\n");
                send(*new_socket, message, strlen(message), 0);
            }

            flag = 1;
        }

        //SOAL 1F
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

        //SOAL 1G
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
    }
}

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
