#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

// Mutex pour protéger l'accès aux ressources partagées
std::mutex mtx;

// 📌 Appliquer un Flou Gaussien de manière séquentielle
cv::Mat applyGaussianBlur(const cv::Mat& img) {
    cv::Mat result;
    cv::GaussianBlur(img, result, cv::Size(15, 15), 0);
    return result;
}

// 📌 Appliquer la Détection de Contours (Canny)
cv::Mat applyCannyEdgeDetection(const cv::Mat& img) {
    cv::Mat edges;
    cv::Canny(img, edges, 100, 200);
    return edges;
}

// 📌 Traitement Multi-threadé : Diviser l'image en parties et appliquer un filtre
void processSection(const cv::Mat& input, cv::Mat& output, int startRow, int endRow) {
    cv::Mat section = input(cv::Range(startRow, endRow), cv::Range::all());

    std::lock_guard<std::mutex> lock(mtx);  // Protection contre l'accès concurrent
    cv::GaussianBlur(section, output(cv::Range(startRow, endRow), cv::Range::all()), cv::Size(15, 15), 0);
}

int main() {
    std::string imagePath = "C:/Users/kamil/OneDrive/Bureau/CPMULTI/bg.png";  // Assurez-vous que l'image est bien à cet emplacement
    cv::Mat image = cv::imread(imagePath);

    if (image.empty()) {
        std::cout << "Erreur : Impossible de charger l'image !" << std::endl;
        return -1;
    }

    // 📌 Mesurer le temps d'exécution SEQUENTIELLE
    auto start_seq = std::chrono::high_resolution_clock::now();
    cv::Mat blurred_seq = applyGaussianBlur(image);
    auto stop_seq = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::milliseconds>(stop_seq - start_seq);
    std::cout << "Temps d'exécution SEQUENTIELLE : " << duration_seq.count() << " ms" << std::endl;

    // 📌 Initialisation pour le traitement multi-threadé
    cv::Mat blurred_mt = image.clone();
    int height = image.rows;

    // 📌 Mesurer le temps d'exécution MULTI-THREADING
    auto start_mt = std::chrono::high_resolution_clock::now();
    
    std::thread t1(processSection, std::ref(image), std::ref(blurred_mt), 0, height / 2);
    std::thread t2(processSection, std::ref(image), std::ref(blurred_mt), height / 2, height);

    t1.join();
    t2.join();

    auto stop_mt = std::chrono::high_resolution_clock::now();
    auto duration_mt = std::chrono::duration_cast<std::chrono::milliseconds>(stop_mt - start_mt);
    std::cout << "Temps d'exécution MULTI-THREADÉ : " << duration_mt.count() << " ms" << std::endl;

    // 📌 Affichage des résultats
    cv::imshow("Image Originale", image);
    cv::imshow("Flou Gaussien Séquentiel", blurred_seq);
    cv::imshow("Flou Gaussien Multi-threadé", blurred_mt);
    cv::waitKey(0);

    return 0;
}
