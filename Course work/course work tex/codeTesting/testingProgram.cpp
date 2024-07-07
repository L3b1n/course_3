#include <iostream>
#include <filesystem>
#include <BackgroundFilter.h>
#include <opencv2/opencv.hpp>

int main() {
    BackgroundFilter filter;
    filter.filterUpdateThreads(1);
    filter.filterUpdateModel(MODEL_PPHUMANSEG);
    filter.filterUpdateProvider(USECPU_VINO);
    filter.filterActivateChanges();

    std::string imagePath = std::filesystem::current_path()
                                    .append("input.png").string();
    cv::Mat image_mat = cv::imread(imagePath);

    int max_inference_time_prepairing = 0;
    int min_inference_time_prepairing = INT32_MAX;
    int sum_inference_time_prepairing = 0;

    int numOfPreporation = 100;

    for (int i = 0; i < numOfPreporation; i++) {
        auto start_time_prepairing = std::chrono::high_resolution_clock::now();
        filter.filterVideoTick(
                image_mat.rows, image_mat.cols, 
                image_mat.type(), image_mat.data);
        auto end_time_prepairing = std::chrono::high_resolution_clock::now();
        int inference_time_ms = 
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time_prepairing - start_time_prepairing).count();
        sum_inference_time_prepairing += inference_time_ms;
        max_inference_time_prepairing = 
            std::max(inference_time_ms, max_inference_time_prepairing);
        min_inference_time_prepairing = 
            std::min(inference_time_ms, min_inference_time_prepairing);
    }

    std::cout << "Minimum infirence time of preporation: " 
        << min_inference_time_prepairing << " ms\n";
    std::cout << "Maximun infirence time of preporation: " 
        << max_inference_time_prepairing << " ms\n";
    std::cout << "Average infirence time of preporation: " 
        << sum_inference_time_prepairing / numOfPreporation << " ms\n";

    int max_inference_time = 0;
    int min_inference_time = INT32_MAX;
    int sum_inference_time = 0;

    int numOfIteration = 3000;

    for (int i = 0; i < numOfIteration; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        filter.filterVideoTick(
            image_mat.rows, image_mat.cols, 
            image_mat.type(), image_mat.data);
        auto end_time = std::chrono::high_resolution_clock::now();
        int inference_time_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                    end_time - start_time).count();
        sum_inference_time += inference_time_ms;
        max_inference_time =
            std::max(inference_time_ms, max_inference_time);
        min_inference_time =
            std::min(inference_time_ms, min_inference_time);
    }

    std::cout 
        << "\nMinimum infirence time: " << min_inference_time << " ms\n";
    std::cout 
        << "Maximun infirence time: " << max_inference_time << " ms\n";
    std::cout << "Average infirence time: " 
        << sum_inference_time / numOfIteration << " ms\n";
    
    return 0;
}