// File: greenScreem.cpp
// Team: Keval Visaria and Chirag Dhoka Jain
// Date: January 26, 2024
// Purpose: This program captures video from a webcam, applies a green screen effect,
//          and allows the user to toggle the green screen on/off using the 'g' key.

#include <opencv2/opencv.hpp>


// Summary: Entry point of the program.
//          Captures video from the default webcam, applies a green screen effect,
//          and allows the user to toggle the green screen on/off using the 'g' key.
int main() {
    // Initialize video capture
    cv::VideoCapture cap(0); // 0 for default webcam

    // Check if the video capture is successful
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }

    // Define green screen range in HSV color space
    cv::Scalar lower_green = cv::Scalar(40, 40, 40);
    cv::Scalar upper_green = cv::Scalar(80, 255, 255);

    // Flag to indicate whether green screen is active
    bool greenScreenActive = false;

    // Main loop
    while (true) {
        // Capture frame from video stream
        cv::Mat frame;
        cap.read(frame);

        // Check if the frame is empty 
        if (frame.empty()) {
            std::cerr << "End of video stream" << std::endl;
            break;
        }

        // Convert frame to HSV color space
        cv::Mat hsv_frame;
        cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);

        // Create a mask using the green screen range if green screen is active
        cv::Mat mask;
        if (greenScreenActive) {
            cv::inRange(hsv_frame, lower_green, upper_green, mask);

            // Invert the mask
            cv::bitwise_not(mask, mask);

            // Apply the mask to the frame
            cv::Mat result;
            cv::bitwise_and(frame, frame, result, mask);

            // Display the result
            cv::imshow("Green Screen", result);
        }
        else {
            // Display the original frame
            cv::imshow("Green Screen", frame);
        }

        // Check for keypress
        int key = cv::waitKey(1);

        // Toggle green screen on 'g' key press
        if (key == 'g') {
            greenScreenActive = !greenScreenActive;
        }

        // Exit the loop when the 'q' key is pressed
        if (key == 'q') {
            break;
        }
    }

    // Release resources
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
