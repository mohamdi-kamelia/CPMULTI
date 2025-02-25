#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "traitement.hpp"
#include "utilitaires/index.hpp"
#include "multithreading/index.hpp"

// Fonction pour afficher un graphique avec OpenCV avec axes et labels
void afficherGraphiqueOpenCV(long timeSeq, long timeMT) {
    int largeur = 600, hauteur = 400;
    cv::Mat graphe = cv::Mat::zeros(hauteur, largeur, CV_8UC3);

    // Définition des couleurs
    cv::Scalar couleurSeq(255, 0, 0);  // Bleu pour Séquentiel
    cv::Scalar couleurMT(0, 255, 0);   // Vert pour Multi-threading
    cv::Scalar couleurAxes(255, 255, 255); // Blanc pour les axes

    // Normalisation des valeurs
    int maxTemps = std::max(timeSeq, timeMT);
    int hauteurSeq = (timeSeq * (hauteur - 100)) / maxTemps; // -100 pour laisser la place aux labels
    int hauteurMT = (timeMT * (hauteur - 100)) / maxTemps;

    // Position des barres
    int largeurBarre = 100;
    int espace = 150;

    // 🔹 Dessiner les axes (Y = Temps, X = Labels)
    cv::line(graphe, cv::Point(50, 50), cv::Point(50, hauteur - 50), couleurAxes, 2); // Axe Y
    cv::line(graphe, cv::Point(50, hauteur - 50), cv::Point(largeur - 50, hauteur - 50), couleurAxes, 2); // Axe X

    // 🔹 Ajouter les valeurs sur l'axe Y (temps)
    for (int i = 0; i <= 5; ++i) { // 5 niveaux pour l'échelle
        int y = hauteur - 50 - (i * (hauteur - 100) / 5);
        int valeur = (maxTemps * i) / 5;
        cv::putText(graphe, std::to_string(valeur) + " ms", cv::Point(5, y), cv::FONT_HERSHEY_SIMPLEX, 0.5, couleurAxes, 1);
    }

    // 🔹 Dessiner les barres
    cv::rectangle(graphe, cv::Point(espace, hauteur - 50 - hauteurSeq), cv::Point(espace + largeurBarre, hauteur - 50), couleurSeq, -1);
    cv::rectangle(graphe, cv::Point(espace * 2, hauteur - 50 - hauteurMT), cv::Point(espace * 2 + largeurBarre, hauteur - 50), couleurMT, -1);

    // 🔹 Ajouter les légendes sous les barres (labels X)
    cv::putText(graphe, "Seq", cv::Point(espace + 30, hauteur - 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, couleurSeq, 2);
    cv::putText(graphe, "MT", cv::Point(espace * 2 + 30, hauteur - 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, couleurMT, 2);

    // 🔹 Ajouter un titre
    cv::putText(graphe, "Comparaison des Temps d'Execution", cv::Point(100, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, couleurAxes, 2);

    // Affichage du graphique
    cv::imshow("Comparaison des performances", graphe);
}

int main() {
    cv::Mat image = utils::loadImage("../images/bg.png");

    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> results_seq;
    std::vector<cv::Mat> results_mt;

    // 🔹 Exécution Séquentielle
    auto startSeq = std::chrono::high_resolution_clock::now();
    traitement::processFiltersSequential(image, results_seq);
    auto endSeq = std::chrono::high_resolution_clock::now();
    long timeSeq = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();
    std::cout << "Temps SÉQUENTIEL : " << timeSeq << " ms" << std::endl;

    // 🔹 Exécution Multi-threading
    auto startMT = std::chrono::high_resolution_clock::now();
    multithreading::processFiltersParallel(image, results_mt);
    auto endMT = std::chrono::high_resolution_clock::now();
    long timeMT = std::chrono::duration_cast<std::chrono::milliseconds>(endMT - startMT).count();
    std::cout << "Temps MULTI-THREADING : " << timeMT << " ms" << std::endl;

    // Vérification du nombre d'images traitées
    if (results_mt.size() < 8) {
        std::cerr << "Erreur : Nombre d'images insuffisant !" << std::endl;
        return -1;
    }

    // 🔹 Affichage des images filtrées
    cv::imshow("Gaussian Blur", results_mt[0]);
    cv::imshow("Median Blur", results_mt[1]);
    cv::imshow("Denoising", results_mt[2]);
    cv::imshow("Canny Edge Detection", results_mt[3]);
    cv::imshow("Sobel Edge Detection", results_mt[4]);
    cv::imshow("Fourier Transform", results_mt[5]);
    cv::imshow("Resized Image", results_mt[6]);
    cv::imshow("Rotated Image", results_mt[7]);

    // 🔹 Attendre l'appui sur "G" pour afficher le graphique
    std::cout << "Appuyez sur 'G' pour afficher le graphique, ou 'ESC' pour quitter.\n";

    while (true) {
        int key = cv::waitKey(0);
        if (key == 'g' || key == 'G') {
            afficherGraphiqueOpenCV(timeSeq, timeMT);
        } else if (key == 27) { // Touche ESC pour quitter
            break;
        }
    }

    return 0;
}
