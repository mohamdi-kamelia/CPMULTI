#include <iostream>
#include <fstream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "traitement/index.hpp"
#include "utilitaires/index.hpp"
#include "multithreading/index.hpp"

void generateData(const std::vector<long>& sequentialTimes, const std::vector<long>& multiThreadedTimes) {
    std::ofstream dataFile("performance_data.txt");
    if (!dataFile) {
        std::cerr << "Erreur lors de la création du fichier de données !" << std::endl;
        return;
    }

    // Vérifier que les vecteurs ont bien les bonnes valeurs
    if (sequentialTimes.size() != multiThreadedTimes.size()) {
        std::cerr << "Erreur : les tailles des données séquentielles et multi-threading ne correspondent pas !" << std::endl;
        return;
    }

    // Écriture des temps d'exécution dans le fichier
    dataFile << "# Filter Sequential Multi-threaded\n";
    dataFile << "Gaussian_Blur " << sequentialTimes[0] << " " << multiThreadedTimes[0] << "\n";
    dataFile << "Median_Blur " << sequentialTimes[1] << " " << multiThreadedTimes[1] << "\n";
    dataFile << "Denoising " << sequentialTimes[2] << " " << multiThreadedTimes[2] << "\n";
    dataFile << "Sobel_Edge " << sequentialTimes[3] << " " << multiThreadedTimes[3] << "\n";
    dataFile << "Fourier_Transform " << sequentialTimes[4] << " " << multiThreadedTimes[4] << "\n";
    dataFile << "Resize " << sequentialTimes[5] << " " << multiThreadedTimes[5] << "\n";
    dataFile << "Rotate " << sequentialTimes[6] << " " << multiThreadedTimes[6] << "\n";

    dataFile.close();
}

void plotGraph() {
    std::ofstream scriptFile("plot_script.gnuplot");
    if (!scriptFile) {
        std::cerr << "Erreur lors de la création du script Gnuplot !" << std::endl;
        return;
    }

    scriptFile << "set terminal pngcairo size 900,600\n";
    scriptFile << "set output 'performance_comparison.png'\n";
    scriptFile << "set title 'Comparaison du Temps d Execution : Séquentiel vs Multi-threading'\n";
    scriptFile << "set xlabel 'Filtres Appliqués'\n";
    scriptFile << "set ylabel 'Temps d Execution (ms)'\n";
    scriptFile << "set style data histograms\n";
    scriptFile << "set style fill solid\n";
    scriptFile << "set boxwidth 0.5\n";
    scriptFile << "set xtic rotate by -45\n";
    scriptFile << "plot 'performance_data.txt' using 2:xtic(1) title 'Séquentiel' lt rgb 'red', '' using 3 title 'Multi-threading' lt rgb 'blue'\n";

    scriptFile.close();

    // Exécuter Gnuplot pour générer le graphique
    system("gnuplot plot_script.gnuplot");
}

int main() {
    cv::Mat image = utils::loadImage("../images/website.jpg");
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> results_sequential;
    std::vector<cv::Mat> results_parallel;

    std::vector<long> sequentialTimes;
    std::vector<long> multiThreadedTimes;

    // ✅ **Mesurer le temps séquentiel**
    auto startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::applyGaussianBlur(image, 5));
    auto endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::applyMedianBlur(image, 5));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::applyDenoising(image));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::applySobelEdgeDetection(image));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::applyFourierTransform(image));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::resizeImage(image, 0.5));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    startSeq = std::chrono::high_resolution_clock::now();
    results_sequential.push_back(traitement::rotateImage(image, 45));
    endSeq = std::chrono::high_resolution_clock::now();
    sequentialTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count());

    // ✅ **Mesurer le temps multi-threading**
    auto startMT = std::chrono::high_resolution_clock::now();
    multithreading::processFiltersParallel(image, results_parallel);
    auto endMT = std::chrono::high_resolution_clock::now();
    
    // On suppose que tous les filtres sont traités en parallèle, on prend le temps global
    long totalMultiThreadedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endMT - startMT).count();
    
    // Stocker les temps multi-threading (chaque filtre prend environ le même temps)
    for (size_t i = 0; i < sequentialTimes.size(); i++) {
        multiThreadedTimes.push_back(totalMultiThreadedTime);
    }

    std::cout << "✅ Tous les filtres ont été appliqués !" << std::endl;
    std::cout << "⏳ Temps d'exécution MULTI-THREADING : " << totalMultiThreadedTime << " ms" << std::endl;

    // Générer le fichier de données et tracer le graphique
    generateData(sequentialTimes, multiThreadedTimes);
    plotGraph();

    // 🔹 Afficher les résultats des filtres
    utils::displayImage("Gaussian Blur", results_parallel[0]);
    utils::displayImage("Median Blur", results_parallel[1]);
    utils::displayImage("Denoising", results_parallel[2]);
    utils::displayImage("Sobel Edge Detection", results_parallel[3]);
    utils::displayImage("Fourier Transform", results_parallel[4]);
    utils::displayImage("Resized Image", results_parallel[5]);
    utils::displayImage("Rotated Image", results_parallel[6]);

    return 0;
}