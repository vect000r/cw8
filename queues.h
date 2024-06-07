#ifndef QUEUES_H
#define QUEUES_H

#define MAX_MSGS 10
#define MAX_MSG_SIZE 30
#define QUEUE_NAME "/test_queue"

mqd_t createQueue(const char *name, int oflag);
mqd_t openQueue(const char *name, int oflag);
void deleteQueue(const char *name);
void closeQueue(mqd_t mq);
void sendMessage(mqd_t mq, const char *msg, unsigned int priority);
void receiveMessage(mqd_t mq, char *msg, unsigned int *priority);
struct mq_attr getQueueAttributes(mqd_t mq);

#endif