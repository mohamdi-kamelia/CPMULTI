#include "multithreading.hpp"
#include "../traitement/index.hpp"
#include <iostream>
#include <thread>
#include <mutex>

namespace multithreading {
    std::mutex mtx; // 🔒 Mutex global pour protéger l'accès à `results`

    void processFiltersParallel(const cv::Mat& image, std::vector<cv::Mat>& results) {
        results.resize(8);  // 8 filtres

        std::vector<std::thread> threads;

        // 🔹 Chaque fonction doit être appelée avec les bons arguments
        threads.emplace_back([&]() {
            cv::Mat output = traitement::applyGaussianBlur(image, 15);
            std::lock_guard<std::mutex> lock(mtx);
            results[0] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::applyMedianBlur(image, 5);
            std::lock_guard<std::mutex> lock(mtx);
            results[1] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::applyDenoising(image);
            std::lock_guard<std::mutex> lock(mtx);
            results[2] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::applyEdgeDetection(image, 100.0, 200.0); // Fixe ici
            std::lock_guard<std::mutex> lock(mtx);
            results[3] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::applySobelEdgeDetection(image);
            std::lock_guard<std::mutex> lock(mtx);
            results[4] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::applyFourierTransform(image);
            std::lock_guard<std::mutex> lock(mtx);
            results[5] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::resizeImage(image, 0.5); // Corrigé ici
            std::lock_guard<std::mutex> lock(mtx);
            results[6] = output;
        });

        threads.emplace_back([&]() {
            cv::Mat output = traitement::rotateImage(image, 45.0); // Corrigé ici
            std::lock_guard<std::mutex> lock(mtx);
            results[7] = output;
        });

        // 🔹 Attendre la fin de tous les threads
        for (auto& thread : threads) {
            thread.join();
        }
    }

}

