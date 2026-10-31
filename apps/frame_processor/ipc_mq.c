#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

#define MQ_NAME "/frame_queue"
#define MAX_MSG_SIZE 256

mqd_t setup_message_queue(void) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // Clean up any stale queue instance
    mq_unlink(MQ_NAME);

    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
        exit(1);
    }
    return mq;
}

void send_frame_notification(mqd_t mq, int frame_index) {
    char msg[MAX_MSG_SIZE];
    snprintf(msg, sizeof(msg), "FRAME:%d", frame_index);
    if (mq_send(mq, msg, strlen(msg) + 1, 0) < 0) {
        perror("mq_send failed");
    }
}