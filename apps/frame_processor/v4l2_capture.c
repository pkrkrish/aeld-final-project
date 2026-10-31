#include "v4l2_capture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

struct buffer {
    void   *start;
    size_t  length;
};

int init_v4l2_camera(const char *dev_name, int *fd, struct buffer **bufs_out) {
    *fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if (*fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 1280;
    fmt.fmt.pix.height = 720;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (ioctl(*fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT failed");
        return -1;
    }

    struct v4l2_requestbuffers req = {0};
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(*fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS failed");
        return -1;
    }

    struct buffer *buffers = calloc(req.count, sizeof(struct buffer));
    for (unsigned int i = 0; i < req.count; ++i) {
        struct v4l2_buffer buf = {0};
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (ioctl(*fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF failed");
            return -1;
        }
        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, buf.m.offset);
        if (buffers[i].start == MAP_FAILED) {
            perror("mmap failed");
            return -1;
        }
    }
    *bufs_out = buffers;
    return 0;
}