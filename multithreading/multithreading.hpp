#ifndef MULTITHREADING_HPP
#define MULTITHREADING_HPP

#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>
#include <mutex>

namespace multithreading {
    void processFiltersParallel(const cv::Mat& image, std::vector<cv::Mat>& results);
}

#endif // MULTITHREADING_HPP
