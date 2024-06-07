#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <mqueue.h>
#include "queues.h"

void cleanup()
{
    deleteQueue(QUEUE_NAME);
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
    
    mqd_t mq = createQueue(QUEUE_NAME, O_RDWR);

    struct mq_attr attr = getQueueAttributes(mq);
    printf("Created queue: %s \n  Descriptor: %d \n ", QUEUE_NAME, mq);
    printf("Attributes: \n");
    printf("mq_maxmsg: %ld \n", attr.mq_maxmsg);
    printf("mq_msgsize: %ld \n", attr.mq_msgsize);
    printf("mq_curmsgs: %ld \n", attr.mq_curmsgs);
    printf("mq_flags: %ld \n", attr.mq_flags);

    int number1, number2;
    int result;
    int client_pid;
    //size_t pid_size = 10;
    char resultMsg[MAX_MSG_SIZE];
    char client_pid_s[MAX_MSG_SIZE];
    char operator;
    char msg[MAX_MSG_SIZE];
    mqd_t response;

    while(1)
    {
        receiveMessage(mq, msg, NULL);
        sscanf(msg, "%d %d%c%d", &client_pid, &number1, &operator, &number2);
        printf("Received: %d %c %d\n", number1, operator, number2);

        if(operator == '+')
        {
            result = number1 + number2;
            snprintf(resultMsg, MAX_MSG_SIZE, "%d", result);
        }
        else if(operator == '-')
        {
            result = number1 - number2;
            snprintf(resultMsg, MAX_MSG_SIZE, "%d", result);
        }
        else if(operator == '*')
        {
            result = number1 * number2;
            snprintf(resultMsg, MAX_MSG_SIZE, "%d", result);
        }
        else if(operator == '/')
        {
            result = number1 / number2;
            snprintf(resultMsg, MAX_MSG_SIZE, "%d", result);
        }
        else
        {
            result = 0;
        }

        sprintf(client_pid_s, "/%d", client_pid);
        response = openQueue(client_pid_s, O_WRONLY);
        openQueue("/response", O_WRONLY);
        printf("Response: %s to client: %d\n", resultMsg, client_pid);
        sendMessage(response, resultMsg, 1);
        closeQueue(response);
    }

    deleteQueue(QUEUE_NAME);
    return 0;
}