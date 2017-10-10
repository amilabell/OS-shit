#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct buffer {
    long mtype;
    char text[200];
};

int main(int argc, char *argv[])
{
    struct buffer buffer;
    int queue_id;
    key_t key;

    if ((key = ftok("anne.c", 'B')) == -1) {  
        perror("ftok");
        exit(1);
    }

    if ((queue_id = msgget(key, 0644)) == -1) {
        if ((queue_id = msgget(key, 0644 | IPC_CREAT)) == -1) {
            perror("Error while connecting to queue");
            exit(1);
        }
    }
    
    if (argc > 1) {
        if (strcmp("receive", argv[1]) == 0){
            printf("receiving messages:\n");

            for(;;) { 
                if (msgrcv(queue_id,: &buffer, sizeof buffer.text, 0, 0) == -1) {
                    perror("msgrcv");
                    exit(1);
                }
                printf("sent message: \"%s\"\n", buffer.text);
            }
        } else if(strcmp("send", argv[1]) == 0){
            printf("Enter lines of text, ^C to quit:\n");

            buffer.mtype = 1; 

            while(fgets(buffer.text, sizeof buffer.text, stdin) != NULL) {
                int len = strlen(buffer.text);

                if (buffer.text[len-1] == '\n') buffer.text[len-1] = '\0';

                if (msgsnd(queue_id, &buffer, len+1, 0) == -1) 
                    perror("error with sending the message");
            }

            if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(1);
            }
        }
    } else{
        printf("Please state if the program should send or receive \n");
        exit (1);
    } 

    return 0;
}
