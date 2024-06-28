#include <iostream>
#include <typeinfo>
#include <opencv2/opencv.hpp>

#include "gmod_api.h"

int main() {
    IGMOD* test = CreateGMOD();
    test->setCameraProps(0, 1280, 720, 30);
    test->setOverlay(true);

    cv::VideoCapture cap;
    test->init("graph.pbtxt", cap);

    int frames = 0;
    double fps = 0.0;
    std::string path;
    bool load_flag = true;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (load_flag) {
        cv::Mat temp;
        cap >> temp;
        test->start(temp, load_flag);

        frames++;
        auto end_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0;
        if (elapsed_time >= 1.0) {
            fps = frames / elapsed_time;
            frames = 0;
            start_time = std::chrono::high_resolution_clock::now();
        }

        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
        cv::putText(temp, ss.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);

        cv::imshow("test", temp);
        const int pressed_key = cv::waitKey(5);
        if (pressed_key == 27) load_flag = false;
        if (cv::getWindowProperty("test", cv::WND_PROP_VISIBLE) < 1) load_flag = false;
        if (pressed_key == 'w') { 
            int type;
            std::cin >> type; 
            path = "background.jpg";
            if (type == 0) {
                test->setOverlay(false);
                test->init("mediapipe_graphs/selfie_segmentation/ultimate.pbtxt", cap, path, 0);
            } else if (type == 1) {
                test->setOverlay(true);
                test->init("mediapipe_graphs/selfie_segmentation/ultimate.pbtxt", cap, path, 1);
            }
        }
    }
    return 0;
}