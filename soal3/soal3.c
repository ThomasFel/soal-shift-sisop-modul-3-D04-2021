#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
#include<ctype.h>

char *cekNama(char *dir);
char *hrfKecil(char *str);
char *cekTipe(char *dir);
void* kategori(void *arg);
void kategoriFolder(char *folderPath, int threadSize);

int main(int argc, char *argv[])
{
	if(argc == 0) 
		exit(EXIT_FAILURE);

	if(argc > 2 && strcmp(argv[1], "-f") == 0)
	{ //soal 3a
		pthread_t tid[argc-2];
		int count = 0;
		for(int i=2; i<argc; i++)
		{
			if(access(argv[i], F_OK) == 0)
			{
				pthread_create(&tid[count], NULL, kategori, (void *)argv[i]);
				count++;
				printf("File %d : Berhasil Dikategorikan\n", i-1);
			}
			else 
				printf("File %d : Sad, gagal :(\n", i-1);
		}
		for(int i=0; i<count; i++) 
			pthread_join(tid[i], NULL);
		return 0;
	}

	else if(argc == 3 && strcmp(argv[1], "-d") == 0)
	{
		DIR *fd = opendir(argv[2]);
		if(fd)
		{
			struct dirent *dp;
			int threadSize = 0;
			while((dp = readdir(fd)) != NULL)
			{
				if(dp->d_type == DT_REG)
				{
					threadSize++;
				}
			}
			kategoriFolder(argv[2], threadSize);
			closedir(fd);
			printf("Direktori sukses disimpan!\n");
		}
		else if(ENOENT == errno) 
			printf("Yah, gagal disimpan :(\n");
	}

	else if(argc == 2 && strcmp(argv[1], "*") == 0)
	{ //soal 3c
		char *curr = getenv("PWD");
		DIR *dir = opendir(curr);
		struct dirent *dp;
		int threadSize = 0;
		while((dp = readdir(dir)) != NULL)
		{
			if(dp->d_type == DT_REG)
			{
				threadSize++;
			}
		}
		kategoriFolder(curr, threadSize);
		closedir(dir);
	}
	
	else
	{
		printf("Format input salah\n");
		return 0;
	}
}

char *cekNama(char *dir)
{
	char *nama = strrchr(dir, '/');
	if(nama == dir) 
		return "";
	return nama + 1;
}

char *hrfKecil(char *str)
{
	unsigned char *temp = (unsigned char *)str;
	while(*temp)
	{
		*temp = tolower(*temp);
		temp++;
	}
	return str;
}

char *cekTipe(char *dir)
{
	char *unknown = {"Unknown"};
	char *hidden = {"Hidden"};
	char *tmp = strrchr(dir, '/'); 
	if(tmp[1] == '.') 
		return hidden;

	int i = 0;
	while(i < strlen(tmp) && tmp[i] != '.') 
		i++;
	if(i == strlen(tmp)) 
		return unknown;

	char tipe[400];
	int j = i;
	while(i < strlen(tmp)) 
		tipe[i-j] = tmp[i], i++;
	return hrfKecil(tipe + 1);
}

void* kategori(void *arg)
{
	char *asal = (char *)arg;
	char asalpath[150];
	memcpy(asalpath, (char*)arg, 400);
	char *asalTipe = cekTipe(asal);
	char tipe[400];
	strcpy(tipe, asalTipe);

	DIR *dir = opendir(asalTipe);
	if(dir) 
		closedir(dir);
		
	else if(ENOENT == errno) 
		mkdir(asalTipe, 0755);

	char *srcName = cekNama(asalpath);
	char *curr = getenv("PWD");

	char pathTujuan[400];
	sprintf(pathTujuan, "%s/%s/%s", curr, tipe, srcName);
	rename(asalpath, pathTujuan);
}

void kategoriFolder(char *folderPath, int threadSize)
{
	DIR *fd = opendir(folderPath);
	struct dirent *dp;
	pthread_t tid[threadSize];
	int count = 0;
	char fileName[400][400];

	while((dp = readdir(fd)) != NULL)
	{
		if(dp->d_type == DT_REG)
		{
			sprintf(fileName[count], "%s/%s", folderPath, dp->d_name);
			pthread_create(&tid[count], NULL, kategori, (void *)fileName[count]);
			count++;
		}
		else if((dp->d_type == DT_DIR) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
		{
            char folderPath2[400];
            sprintf(folderPath2, "%s/%s", folderPath, dp->d_name);
            DIR *fd2 = opendir(folderPath2);
            struct dirent *dp2;
			int threadSize2 = 0;
			while((dp2 = readdir(fd2)) != NULL)
			{
				if(dp2->d_type == DT_REG)
				{
					threadSize2++;
				}
			}
			kategoriFolder(folderPath2, threadSize2);
			closedir(fd2);
        }
	}

	for(int i=0; i<threadSize; i++) 
		pthread_join(tid[i], NULL);
    closedir(fd);
}
