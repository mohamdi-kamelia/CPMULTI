#include "utils.hpp"
#include <iostream>

namespace utils {

    cv::Mat loadImage(const std::string& path) {
        return cv::imread(path);
    }

    void saveImage(const std::string& path, const cv::Mat& image) {
        cv::imwrite(path, image);
    }

    void displayImage(const std::string& windowName, const cv::Mat& image) {
        cv::imshow(windowName, image);
        cv::waitKey(0);
    }

}
