#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <mqueue.h>
#include <opencv2/opencv.hpp>

// Include the shared C header with proper C linkage handling
extern "C" {
    #include "v4l2_capture.h"
}

// Clean initialization of the 3x3 sharpening kernel (avoids deprecated comma warnings)
void apply_frame_sharpening(const cv::Mat &input_frame, cv::Mat &output_frame) {
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 
        0.0f, -1.0f,  0.0f,
       -1.0f,  5.0f, -1.0f,
        0.0f, -1.0f,  0.0f
    );
    cv::filter2D(input_frame, output_frame, input_frame.depth(), kernel);
}

int main(int argc, char *argv[]) {
    std::cout << "[INFO] Starting Frame Processor on Raspberry Pi 5..." << std::endl;

    int camera_fd = -1;
    struct buffer *buffers = nullptr;

    const char *dev_node = "/dev/video0";
    if (init_v4l2_camera(dev_node, &camera_fd, &buffers) < 0) {
        std::cerr << "[ERROR] Failed to initialize V4L2 camera from " << dev_node << std::endl;
        return -1;
    }
    std::cout << "[SUCCESS] V4L2 Camera initialized successfully." << std::endl;

    // ... (rest of your main loop and cleanup logic)
    
    return 0;
}