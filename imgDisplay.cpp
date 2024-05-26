// File: imageDisplay.cpp
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 21, 2024
// Purpose: Reading and Displaying an image

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // Read the image from file
    cv::Mat img = cv::imread("C:/Users/visar/Downloads/img.png");

    // Check if the image is empty or cannot be read
    if (img.empty()) {
        std::cerr << "Error: Image not found or cannot be read." << std::endl;
        return -1;
    }

    // Display the image
    cv::imshow("My Image", img);
    printf("Image is being Displayed\n")
    // Wait for a key press
    while (true) {
        int key = cv::waitKey(0) & 0xff;

        // Check if the key pressed is 'q' or 'Q' to exit
        if (key == 113 || key == 81) {
            printf("\nQuitting ... ")
            cv::destroyAllWindows();
            break;
        }
    }

    return 0;
}