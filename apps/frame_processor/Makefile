BUILDROOT_DIR = $(HOME)/buildroot
CROSS_COMPILE = $(BUILDROOT_DIR)/output/host/bin/aarch64-linux-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
SYSROOT = $(BUILDROOT_DIR)/output/host/aarch64-buildroot-linux-gnu/sysroot

# Add OpenCV include path relative to the sysroot
CXXFLAGS = --sysroot=$(SYSROOT) -Wall -O2 -I$(SYSROOT)/usr/include/opencv4
CFLAGS = --sysroot=$(SYSROOT) -Wall -O2

# Link OpenCV libraries
LDFLAGS = --sysroot=$(SYSROOT) -lopencv_core -lopencv_imgproc -lrt

OBJS = v4l2_capture.o main.o

all: frame_processor

frame_processor: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o frame_processor