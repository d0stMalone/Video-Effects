// File: memeGen.cpp
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 26, 2024
// Purpose: Adding captions to images or video sequences when they are saved 

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

// Summary: Draw text on the given image with specified position, text color, and background color.
// Parameters:
//   - image: The image to draw on.
//   - text: The text to be drawn.
//   - position: The position of the text.
//   - textColor: The color of the text.
//   - bgColor: The background color for the text.
void drawText(Mat& image, const string& text, Point position, Scalar textColor = Scalar(255, 255, 255), Scalar bgColor = Scalar(0, 0, 0)) {
    // Set the font and scale for the text
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.5;
    int thickness = 3;

    // Get the size of the text
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, nullptr);

    // Create a rectangle around the text with some padding
    Rect backgroundRect(position.x, position.y - textSize.height, textSize.width, textSize.height + 5);

    // Draw a filled black rectangle as the background
    rectangle(image, backgroundRect, bgColor, FILLED);

    // Draw the text on the image
    putText(image, text, position, fontFace, fontScale, textColor, thickness);
}

int main() {
    // Open a connection to the webcam
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Unable to open webcam." << endl;
        return -1;
    }

    Mat frame;

    // Start the live stream
    cout << "Press 's' to capture a frame. Press 'q' to quit." << endl;

    // Capture an initial frame
    cap >> frame;
    imwrite("initial_image.jpg", frame);

    while (true) {
        cap >> frame;

        // Display the live stream
        imshow("Meme Generator - Live Stream", frame);

        // Check for user input
        char key = waitKey(30);

        // 's' key to capture the current frame
        if (key == 's') {
            // Show the captured frame
            imshow("Meme Generator - Captured Frame", frame);
            waitKey(0);

            // Ask user for text position
            cout << "Enter text position (x y): ";
            int x, y;
            cin >> x >> y;

            // Ask user for text/caption
            string caption;
            cout << "Enter text/caption: ";
            cin.ignore(); // Ignore newline character left in buffer
            getline(cin, caption);

            // Draw text on the image with a black background
            drawText(frame, caption, Point(x, y), Scalar(255, 255, 255), Scalar(0, 0, 0));

            // Show the final meme
            imshow("Meme Generator - Final Image", frame);
            waitKey(0);

            // Ask user if they want to save the image
            char saveChoice;
            cout << "Do you want to save the image? (y/n): ";
            cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y') {
                imwrite("C:/Users/visar/Desktop/OneDrive - Northeastern University/PRCV/Project1_Images/final_meme.jpg", frame);
                cout << "Image saved as 'final_meme.jpg'" << endl;
            }
        }
        // 'q' key to quit
        else if (key == 'q') {
            break;
        }
    }

    // Close the webcam
    cap.release();
    destroyAllWindows();

    return 0;
}
