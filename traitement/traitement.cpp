#include "traitement.hpp"

namespace traitement {

    // 🔹 Flou Gaussien
    cv::Mat applyGaussianBlur(const cv::Mat& image, int kernelSize) {
        cv::Mat blurredImage;
        cv::GaussianBlur(image, blurredImage, cv::Size(kernelSize, kernelSize), 0);
        return blurredImage;
    }

    // 🔹 Filtre Médian (Réduction du bruit)
    cv::Mat applyMedianBlur(const cv::Mat& image, int kernelSize) {
        cv::Mat blurredImage;
        cv::medianBlur(image, blurredImage, kernelSize);
        return blurredImage;
    }

    // 🔹 Débruitage avec filtre bilatéral
    cv::Mat applyDenoising(const cv::Mat& image) {
        cv::Mat denoisedImage;
        cv::fastNlMeansDenoisingColored(image, denoisedImage, 10, 10, 7, 21);
        return denoisedImage;
    }

    // 🔹 Détection des contours de Canny
    cv::Mat applyEdgeDetection(const cv::Mat& image, double threshold1, double threshold2) {
        cv::Mat grayImage, edges;
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        cv::Canny(grayImage, edges, threshold1, threshold2);
        return edges;
    }

    // 🔹 Détection des contours avec Sobel
    cv::Mat applySobelEdgeDetection(const cv::Mat& image) {
        cv::Mat grayImage, grad_x, grad_y, abs_grad_x, abs_grad_y, sobelImage;

        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        cv::Sobel(grayImage, grad_x, CV_16S, 1, 0, 3);
        cv::Sobel(grayImage, grad_y, CV_16S, 0, 1, 3);

        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);

        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelImage);
        return sobelImage;
    }

    // 🔹 Transformation de Fourier pour le filtrage fréquentiel
    cv::Mat applyFourierTransform(const cv::Mat& image) {
        cv::Mat grayImage;
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

        cv::Mat padded;
        int m = cv::getOptimalDFTSize(grayImage.rows);
        int n = cv::getOptimalDFTSize(grayImage.cols);
        cv::copyMakeBorder(grayImage, padded, 0, m - grayImage.rows, 0, n - grayImage.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
        cv::Mat complexI;
        cv::merge(planes, 2, complexI);
        cv::dft(complexI, complexI);

        cv::split(complexI, planes);
        cv::magnitude(planes[0], planes[1], planes[0]);
        cv::Mat magI = planes[0];

        magI += cv::Scalar::all(1);
        cv::log(magI, magI);

        magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
        cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX);
        return magI;
    }

    // 🔹 Redimensionnement
    cv::Mat resizeImage(const cv::Mat& image, double scale) {
        cv::Mat resizedImage;
        cv::resize(image, resizedImage, cv::Size(), scale, scale, cv::INTER_LINEAR);
        return resizedImage;
    }

    // 🔹 Rotation
    cv::Mat rotateImage(const cv::Mat& image, double angle) {
        cv::Mat rotatedImage;
        cv::Point2f center(image.cols / 2.0F, image.rows / 2.0F);
        cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::warpAffine(image, rotatedImage, rotationMatrix, image.size());
        return rotatedImage;
    }

    // 🔹 🚀 Fonction pour exécuter tous les filtres de manière séquentielle
    void processFiltersSequential(const cv::Mat& image, std::vector<cv::Mat>& results) {
        results.clear(); // Nettoyer le vecteur avant d'ajouter les résultats

        results.push_back(applyGaussianBlur(image, 5));   // Flou Gaussien
        results.push_back(applyMedianBlur(image, 5));     // Filtre Médian
        results.push_back(applyDenoising(image));         // Débruitage
        results.push_back(applyEdgeDetection(image, 100, 200)); // Détection de contours Canny
        results.push_back(applySobelEdgeDetection(image)); // Détection de contours Sobel
        results.push_back(applyFourierTransform(image));  // Transformation de Fourier
        results.push_back(resizeImage(image, 0.5));       // Redimensionnement
        results.push_back(rotateImage(image, 45));        // Rotation
    }

}
