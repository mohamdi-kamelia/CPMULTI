#ifndef TRAITEMENT_HPP
#define TRAITEMENT_HPP

#include <opencv2/opencv.hpp>
#include <vector> // Ajout nécessaire pour utiliser std::vector

namespace traitement {

    // 🔹 FILTRAGE
    cv::Mat applyGaussianBlur(const cv::Mat& image, int kernelSize);
    cv::Mat applyMedianBlur(const cv::Mat& image, int kernelSize);
    cv::Mat applyDenoising(const cv::Mat& image);

    // 🔹 DÉTECTION DE CONTOURS
    cv::Mat applyEdgeDetection(const cv::Mat& image, double threshold1, double threshold2);
    cv::Mat applySobelEdgeDetection(const cv::Mat& image);

    // 🔹 TRANSFORMATIONS
    cv::Mat applyFourierTransform(const cv::Mat& image);
    cv::Mat resizeImage(const cv::Mat& image, double scale);
    cv::Mat rotateImage(const cv::Mat& image, double angle);

    // 🚀 Ajout de la fonction pour le traitement séquentiel
    void processFiltersSequential(const cv::Mat& image, std::vector<cv::Mat>& results);

}

#endif // TRAITEMENT_HPP
