#include <iostream>
#include <opencv2/opencv.hpp>
#include "traitement/index.hpp"
#include "utilitaires/index.hpp"
#include "multithreading/index.hpp"

int main() {
    cv::Mat image = utils::loadImage("../images/bg.png");

    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> results;

    auto startMT = std::chrono::high_resolution_clock::now();
    multithreading::processFiltersParallel(image, results);
    auto endMT = std::chrono::high_resolution_clock::now();

    std::cout << "Tous les filtres ont été appliqués en parallèle !" << std::endl;
    std::cout << "Temps d'exécution MULTI-THREADING : " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(endMT - startMT).count() 
              << " ms" << std::endl;

 
    if (results.size() < 8) {
        std::cerr << "Erreur : Le nombre d'images traitées est insuffisant !" << std::endl;
        return -1;
    }


    cv::imshow("Gaussian Blur", results[0]);
    cv::imshow("Median Blur", results[1]);
    cv::imshow("Denoising", results[2]);
    cv::imshow("Canny Edge Detection", results[3]);
    cv::imshow("Sobel Edge Detection", results[4]);
    cv::imshow("Fourier Transform", results[5]);
    cv::imshow("Resized Image", results[6]);
    cv::imshow("Rotated Image", results[7]);


    cv::waitKey(0);
    
    return 0;
}
