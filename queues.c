#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include "queues.h"
#include <signal.h>

// Funkcja tworząca kolejke

mqd_t createQueue(const char *name, int oflag) 
{
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MSGS;
    attr.mq_msgsize = MAX_MSG_SIZE;
    mqd_t mq = mq_open(name, O_CREAT | O_EXCL | oflag, 0666, &attr);
    if (mq == -1) 
    {
        perror("Failed to create a queue");
        exit(1);
    }
    return mq;
}

// Funkcja otwierająca kolejkę

mqd_t openQueue(const char *name, int oflag) 
{
    mqd_t mq = mq_open(name, oflag);
    if (mq == -1) 
    {
        perror("Failed to open a queue");
        exit(1);
    }
    return mq;
}

// Funkcja usuwająca kolejkę

void deleteQueue(const char *name) 
{
    if (mq_unlink(name) == -1) 
    {
        perror("Failed to delete a queue");
        exit(EXIT_FAILURE);
    }
}

// Funkcja zamykająca kolejkę

void closeQueue(mqd_t mq) 
{
    if (mq_close(mq) == -1) 
    {
        perror("Failed to close a queue");
        exit(EXIT_FAILURE);
    }
}

// Funkcja wysyłająca wiadomość

void sendMessage(mqd_t mq, const char *msg, unsigned int priority) 
{
    if (mq_send(mq, msg, MAX_MSG_SIZE, priority) == -1) 
    {
        perror("Failed to send a message");
        exit(EXIT_FAILURE);
    }
}

// Funkcja odbierająca wiadomość

void receiveMessage(mqd_t mq, char *msg, unsigned int *priority) 
{
    if (mq_receive(mq, msg, MAX_MSG_SIZE, priority) == -1) 
    {
        perror("Failed to receive a message");
        exit(EXIT_FAILURE);
    }
}

// Funkcja zwracająca atrybuty kolejki

struct mq_attr getQueueAttributes(mqd_t mq) 
{
    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) 
    {
        perror("Failed to get queue attributes");
        exit(EXIT_FAILURE);
    }
    return attr;
}

