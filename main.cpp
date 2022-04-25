#include "circular_buffer.h"
#include <cstdio>
#include <algorithm>
#include <thread>
#include "opencv2/opencv.hpp"

void readbuff(circular_buffer<cv::Mat> /*std::queue<cv::Mat>*/& ioBuff, std::mutex& iLock)
{
    int count = 200;
    double time_execute = 0;
    while (count > 0)
    {
        std::lock_guard<std::mutex> lock_read(iLock);
        if (!ioBuff.empty())
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            cv::Mat frameRead = ioBuff.pop();
            //cv::Mat frameRead = ioBuff.front();
            //ioBuff.pop();
            auto end_time = std::chrono::high_resolution_clock::now();
            count--;
            time_execute += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            if (frameRead.empty())
            {
                printf("Buffer is empty %d \n", count);
                continue;
            }
            cv::imshow("test2", frameRead);
            cv::waitKey(100);
        }
    }
    printf("Time of execution read buff %.4f count = %d\n", time_execute / 200.0, count);
}

void writebuff(circular_buffer<cv::Mat>/*std::queue<cv::Mat>*/& ioBuff, std::mutex& iLock)
{
    int count = 200;
    double time_execute = 0;

    cv::VideoCapture cap(0);
    while (count > 0 && cap.isOpened())
    {
        cv::Mat frameRead;
        bool result = cap.read(frameRead);
        if (result)
        {
            std::lock_guard<std::mutex> lock_read(iLock);
            auto start_time = std::chrono::high_resolution_clock::now();
            //            if(ioBuff.size() > 10)
            //                ioBuff.pop();
            ioBuff.push(frameRead);
            auto end_time = std::chrono::high_resolution_clock::now();
            count--;
            time_execute += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }
        //        cv::imshow("test", frameRead);
        //        cv::waitKey(10);
    }

    printf("time of write %.4f count = %d \n", time_execute / 200.0, count);
}

int main(int argc, char* argv[])
{
    circular_buffer<cv::Mat> buffer_custom(100);
    std::queue<cv::Mat> buffer;
    std::mutex lockMutex;
    std::thread write_buf = std::thread(writebuff, std::ref(buffer_custom), std::ref(lockMutex));
    std::thread read_buf = std::thread(readbuff, std::ref(buffer_custom), std::ref(lockMutex));
    write_buf.detach();
    read_buf.join();
    return 0;
}