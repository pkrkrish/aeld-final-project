#ifndef V4L2_CAPTURE_H
#define V4L2_CAPTURE_H

#include <stddef.h>

struct buffer {
    void   *start;
    size_t  length;
};

#ifdef __cplusplus
extern "C" {
#endif

int init_v4l2_camera(const char *dev_name, int *fd, struct buffer **bufs_out);

#ifdef __cplusplus
}
#endif

#endif // V4L2_CAPTURE_H