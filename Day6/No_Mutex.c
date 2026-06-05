#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM_SIZE 4096

int main() {
    key_t key = ftok("/etc/inputrc", 65);
    printf("Key: %#x\n", key);

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    char *data = (char *)shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat failed");
        exit(1);
    }
    strncpy(data, "Hello", SHM_SIZE);
    printf("Message written to shared memory.\n");
    if (shmdt(data) == -1) {
        perror("shmdt failed");
        exit(1);
    } 
    
};