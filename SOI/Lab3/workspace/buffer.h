#ifndef BUFFER_H
#define BUFFER_H

#include <sys/types.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <limits.h>

#define M 100

typedef struct Buffer{
    sem_t buf_sem;
    sem_t read_lock;

    int data[M];
    int size;
    int headId;
    int tailId;

    int num_even;
    int num_odd;
} Buffer;

Buffer * Buffer__bind(){
    static int shm_id = 0;
    if (shm_id == 0)
        shm_id = shmget(IPC_PRIVATE, sizeof(Buffer), SHM_W | SHM_R);

    if (shm_id <= 0){
        printf("shmget failed...\n");
        abort();
    }

    return (Buffer *) shmat(shm_id, NULL, 0);
}

void Buffer__init(Buffer * self){
    sem_init(&self->buf_sem, 1, 1);
    self->headId = self->size = 0;
    self->tailId = M - 1;

    self->num_even = 0;
    self->num_odd = 0;
}

void Buffer__put(Buffer * self, int value){
    sem_wait(&self->buf_sem);
    if (self->size <  M){
        self->tailId = (self->tailId + 1) % M;
        self->data[self->tailId] = value;
        self->size++;

        if (value % 2 == 0){
            self->num_even++;
        }
        else{
            self->num_odd++;
        }
    }
    sem_post(&self->buf_sem);
}

int Buffer__pop(Buffer * self){
    int temp;
    sem_wait(&self->buf_sem);
    if (self->size <= 0)
        temp = INT_MIN;
    else{
        temp = self->data[self->headId];
        self->headId = (self->headId + 1) % M;
        self->size--;

        if (temp % 2 == 0){
            self->num_even--;
        }
        else{
            self->num_odd--;
        }
    }
    sem_post(&self->buf_sem);
    return temp;
}

int Buffer__peek(Buffer * self){
    int temp;
    sem_wait(&self->buf_sem);
    if (self->size <= 0)
        temp = INT_MIN;
    else{
        temp = self->data[self->headId];
    }
    sem_post(&self->buf_sem);
    return temp;
}

int Buffer__get_size(Buffer * self){
    int temp;
    sem_wait(&self->buf_sem);
    temp = self->size;
    sem_post(&self->buf_sem);
    return temp;
}

int Buffer__get_num_even(Buffer * self){
    int temp;
    sem_wait(&self->buf_sem);
    temp = self->num_even;
    sem_post(&self->buf_sem);
    return temp;
}

int Buffer__get_num_odd(Buffer * self){
    int temp;
    sem_wait(&self->buf_sem);
    temp = self->num_odd;
    sem_post(&self->buf_sem);
    return temp;
}

int Buffer__print(Buffer * self){
    sem_wait(&self->buf_sem);
    printf("[");
    for (int i = self->headId; i != self->tailId; i = (i + 1)%M){
        printf("%d, ", self->data[i]);
    }
    printf("%d, ", self->data[self->tailId]);
    printf("]\n");
    sem_post(&self->buf_sem);
}

#endif
