#include <iostream>
#include <opencv2/opencv.hpp>
#include "traitement/index.hpp"
#include "utilitaires/index.hpp"
#include "multithreading/index.hpp"

int main() {
    cv::Mat image = utils::loadImage("../images/website.jpg");


    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> results;
    
    auto startMT = std::chrono::high_resolution_clock::now();
    multithreading::processFiltersParallel(image, results);
    auto endMT = std::chrono::high_resolution_clock::now();

    std::cout << "✅ Tous les filtres ont été appliqués en parallèle !" << std::endl;
    std::cout << "⏳ Temps d'exécution MULTI-THREADING : " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(endMT - startMT).count() 
              << " ms" << std::endl;

    // 🔹 Afficher les résultats
    utils::displayImage("Gaussian Blur", results[0]);
    utils::displayImage("Median Blur", results[1]);
    utils::displayImage("Denoising", results[2]);
    utils::displayImage("Canny Edge Detection", results[3]);
    utils::displayImage("Sobel Edge Detection", results[4]);
    utils::displayImage("Fourier Transform", results[5]);
    utils::displayImage("Resized Image", results[6]);
    utils::displayImage("Rotated Image", results[7]);

    return 0;
}