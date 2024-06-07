#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <mqueue.h>
#include "queues.h"

mqd_t serverq;

void cleanup()
{
    closeQueue(serverq);
}

void sig_handler(int sig)
{
    printf("Received signal %d\n", sig);
    cleanup();
    exit(EXIT_SUCCESS);
}

int main()
{
    if (atexit(cleanup) != 0)
    {
        perror("Failed to register the exit function");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
        perror("Failed to register the signal handler");
        exit(EXIT_FAILURE);
    }
    
    sleep(1);

    serverq = openQueue(QUEUE_NAME, O_WRONLY);

    struct mq_attr attr = getQueueAttributes(serverq);
    printf("Created queue: %s \n  Descriptor: %d \n ", QUEUE_NAME, serverq);
    printf("Attributes: \n");
    printf("mq_maxmsg: %ld \n", attr.mq_maxmsg);
    printf("mq_msgsize: %ld \n", attr.mq_msgsize);
    printf("mq_curmsgs: %ld \n", attr.mq_curmsgs);
    printf("mq_flags: %ld \n", attr.mq_flags);

    char responseq_name[30];
    char equation[MAX_MSG_SIZE];
    char msg[MAX_MSG_SIZE + 10];
    sprintf(responseq_name, "/%d", getpid());
    

    while(1)
    {
        printf("Enter an equation in form of 'xoperatory' where operator is +, -, * or /: ");
        if(scanf("%s", equation) == EOF)
        {
            printf("EOF received, now you may close the window\n");
            exit(EXIT_SUCCESS);
        }

        // przekazujemy pid klienta do komunikatu ktory przekazemy serwerowi
        snprintf(msg, MAX_MSG_SIZE + 10, "%d %s", getpid(), equation);

        // wysyłamy komunikat do serwera
        sendMessage(serverq, msg, 1);
        printf("Sent: %s\n", msg);

        // musimy stworzyć kolejkę która odbierze odpowiedź od serwera
        mqd_t responseq = createQueue(responseq_name, O_RDONLY);
        
        printf("Waiting for the result...\n");
        receiveMessage(responseq, msg, NULL);
        printf("Received: %s\n", msg);

        closeQueue(responseq);
        deleteQueue(responseq_name);
    }

    closeQueue(serverq);

    return 0;
}
