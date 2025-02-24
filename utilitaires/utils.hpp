#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>

namespace utils {

    cv::Mat loadImage(const std::string& path);
    void saveImage(const std::string& path, const cv::Mat& image);
    void displayImage(const std::string& windowName, const cv::Mat& image);

}

#endif // UTILS_HPP
