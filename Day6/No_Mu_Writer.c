// writer.c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 4096

void read_file(const char *filename, char *buffer, size_t size) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen failed");
        exit(1);
    }

    strncat(buffer, filename, size - strlen(buffer) - 1);
    strncat(buffer, ":\n", size - strlen(buffer) - 1);

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        strncat(buffer, line, size - strlen(buffer) - 1);
    }

    strncat(buffer, "\n", size - strlen(buffer) - 1);
    fclose(f);
}

int main() {
    key_t key = ftok("/etc/inputrc", 65);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *data = (char *)shmat(shmid, NULL, 0);
    if (data == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }


    data[0] = '\0';

    while (1) {
        
        read_file("Ascii-Bild1.txt", data, SHM_SIZE);
        data[0] = '\0';
        read_file("Ascii-Bild2.txt", data, SHM_SIZE);
        data[0] = '\0';
        sleep(1);
    }
    
    shmdt(data);
    return 0;
}