#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

struct args {
    int i;
    int j;
};

void *perkalian(void *arg) {
    int i = ((struct args*) arg) -> i;
    int j = ((struct args*) arg) -> j;

    for (int k = 0; k < 3; k++) {
        matriksZ[i][j] += matriksX[i][k] * matriksY[k][j];
    }
}

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

    printf("Hasil Matriks: \n");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%4d", matriksZ[i][j]);
        }
        printf("\n");
    }
    
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(matriksZ), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int *p = (int *) value;

    memcpy(p, matriksZ, 95);

    shmdt(value);
    
    return 0;
}