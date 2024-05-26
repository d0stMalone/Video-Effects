// File: vidDisplay.cpp
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 21, 2024
// Applying Various visual effects on live video stream

#include <opencv2/opencv.hpp>
#include "filter.h"
#include "faceDetect.h"
#include "VideoDisplay.h"

// Function to toggle the keepStrongColor effect
int pickStrongColorToggle(cv::Mat& frame, cv::Mat& outputFrame, bool& isEnabled, uchar threshold = 128);

// Function to adjust brightness and contrast of an image
void adjustBrightnessContrast(cv::Mat& inputFrame, cv::Mat& outputFrame, float brightness, float contrast) {
    inputFrame.convertTo(outputFrame, -1, contrast, brightness);
}


int main(int argc, char* argv[]) {

    // Open the video device
    cv::VideoCapture* capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return -1;
    }
    else {
        printf("Stream Started!\n");
    }

    // Get properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH), (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    // Create a window to display the video
    cv::namedWindow("Video", 1);

    // Initialize variables for image processing
    cv::Mat frame, outputFrame, sobelX, sobelY, embosingFrame, gradientMagnitude, greyFrame, vignettFrame;
    std::vector<cv::Rect> faces;
    cv::Rect last(0, 0, 0, 0);

    // Flags for different image processing modes
    bool greyScaleMode = false;
    bool altGreyScaleMode = false;
    bool sepiaToneMode = false;
    bool vignetteMode = false;
    bool sobelXMode = false;
    bool sobelYMode = false;
    bool blurEnabled = false;
    bool gradientMagnitudeMode = false;
    bool blurQuantizeMode = false;
    bool faceDetectionMode = false;
    bool heartsMode = false;
    bool embossingEnabled = false;
    bool pickStrongColorMode = false;

    // Parameters for some effects
    double vignetteStrength = 0.8;
    double vignetteRadius = 0.7;

    uchar pickStrongColorThreshold = 128;
    int imageCounter = 0;
    int quantizeLevels = 10;
    float brightness = 1.0f; // Initial brightness value
    float contrast = 1.0f;   // Initial contrast value

    // Main loop for capturing and processing frames
    for (;;) {
        char key = cv::waitKey(10);
        *capdev >> frame;

        // Check if the frame is empty
        if (frame.empty()) {
            printf("Frame is empty\n");
            break;
        }

        // Process user input to toggle various image processing modes
        //GreyScale on 'g'
        if (key == 'g') {
            if (!greyScaleMode) {
                printf("Grey Scale Enabled\n");
                greyScaleMode = !greyScaleMode;
            }
            else {
                printf("Grey Scale Disabled\n");
                greyScaleMode = !greyScaleMode;
            }
        }
        //Alternate Grey Scale on 'h'
        else if (key == 'h') {
            if (!altGreyScaleMode) {
                printf("Alternate Grey Scale Enabled\n");
                altGreyScaleMode = !altGreyScaleMode;
            }
            else {
                printf("Alternate Grey Scale Disabled\n");
                altGreyScaleMode = !altGreyScaleMode;
            }
        }
        //SepiaTone Filter on 't'
        else if (key == 't') {
            if (!sepiaToneMode) {
                printf("Sepia Tone Filter Enabled\n");
                sepiaToneMode = !sepiaToneMode;
            }
            else {
                printf("Sepia Tone Filter Disabled\n");
                sepiaToneMode = !sepiaToneMode;
            }
        }
        //Vigent Filter on 'v'
        else if (key == 'v') {
            if (!vignetteMode) {
                printf("Vignet Filter is Enabled\n");
                vignetteMode = !vignetteMode;
            }
            else {
                printf("Vignet Filter is Disabled\n");
                vignetteMode = !vignetteMode;
            }
        }
        //Blur Filter on 'b'
        else if (key == 'b') {
            if (!blurEnabled) {
                printf("Blur Filter Enabled\n");
                blurEnabled = !blurEnabled;
            }
            else {
                printf("Blur Filter Disabled\n");
                blurEnabled = !blurEnabled;
            }
        }
        //Sobel X filter on 'x'
        else if (key == 'x') {
            if (!sobelXMode) {
                printf("Sobel X Filter Enabled\n");
                sobelXMode = !sobelXMode;
            }
            else {
                printf("Sobel X Filter Disabled\n");
                sobelXMode = !sobelXMode;
            }
        }
        //Sobel Y filter on 'y'
        else if (key == 'y') {
            if (!sobelYMode) {
                printf("Sobel Y Filter Enabled\n");
                sobelYMode = !sobelYMode;
            }
            else {
                printf("Sobel Y Filter Disabled\n");
                sobelYMode = !sobelYMode;
            }
        }
        //Magnitude Gradient on 'm' 
        else if (key == 'm') {
            if (!gradientMagnitudeMode) {
                printf("Gradient Magnitude Enabled\n");
                gradientMagnitudeMode = !gradientMagnitudeMode;
            }
            else {
                printf("Gradient Magnitude Disabled\n");
                gradientMagnitudeMode = !gradientMagnitudeMode;
            }
        }
        //Blur Quantize on 'l'
        else if (key == 'l') {
            if (!blurQuantizeMode) {
                printf("Blur and Quantize - Enabled\n");
                blurQuantizeMode = !blurQuantizeMode;
            }
            else {
                printf("Blur and Quantize - Disabled\n");
                blurQuantizeMode = !blurQuantizeMode;
            }
        }
        //Face detection on 'f'
        else if (key == 'f') {
            if (!faceDetectionMode) {
                printf("Face Detection Enabled");
                faceDetectionMode = !faceDetectionMode;
            }
            else {
                printf("Face Detection Disabled");
                faceDetectionMode = !faceDetectionMode;
            }
        }
        //Pick Strongest color on 'n'
        else if (key == 'n') {
            if (!pickStrongColorMode) {
                printf("Strong Color Mode Enabled\n");
                pickStrongColorMode = !pickStrongColorMode;
            }
            else {
                printf("Strong Color Mode Disabled\n");
                pickStrongColorMode = !pickStrongColorMode;
            }
        }
        //Generate hearts on 'c'
        else if (key == 'c') {
            if (!heartsMode) {
                printf("Halo Enabled\n");
                heartsMode = !heartsMode;
            }
            else {
                printf("Halo Disabled\n");
                heartsMode = !heartsMode;
            }
        }
        //Embossing effect on 'p'
        else if (key == 'p') {
            if (!embossingEnabled) {
                printf("Embossing Enabled\n");
                embossingEnabled = !embossingEnabled;
            }
            else {
                printf("Embossing Disabled\n");
                embossingEnabled = !embossingEnabled;
            }
        }
        else if (key == 'w') {
            // Increase brightness
            brightness += 0.1;
            printf("Contrast: %.2f\n", contrast);
        }
        else if (key == 'e') {
            // Decrease brightness
            brightness -= 0.1;
            printf("Contrast: %.2f\n", contrast);
        }
        else if (key == 'a') {
            // Increase contrast
            contrast += 0.1;
            printf("Contrast: %.2f\n", contrast);
        }
        else if (key == 'd') {
            // Decrease contrast
            contrast -= 0.1;
            printf("Contrast: %.2f\n", contrast);
        }

        try {
            // Applying the selected image processing effect based on the active mode
            if (greyScaleMode) {
                cv::cvtColor(frame, outputFrame, cv::COLOR_BGR2GRAY);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Grayscale image is empty\n");
                }
            }
            else if (altGreyScaleMode) {
                altGreyScale(frame, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Greyscale image is empty\n");
                }
            }
            else if (sepiaToneMode) {
                sepiaTone(frame, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Sepia tone image is empty\n");
                }
            }
            else if (vignetteMode) {
                Vignette(frame, vignettFrame, vignetteStrength, vignetteRadius);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", vignettFrame);
                }
                else {
                    printf("Vignett Frame image is empty\n");
                }
            }
            else if (blurEnabled) {
                blur5x5_B(frame, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Blurred image is empty\n");
                }
            }
            else if (sobelXMode) {
                sobelX3x3(frame, sobelX);
                cv::convertScaleAbs(sobelX, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Sobel X image is empty\n");
                }
            }
            else if (sobelYMode) {

                sobelY3x3(frame, sobelY);
                cv::convertScaleAbs(sobelY, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Sobel Y image is empty\n");
                }
            }
            else if (gradientMagnitudeMode) {
                cv::cvtColor(frame, outputFrame, cv::COLOR_BGR2GRAY);
                sobelX3x3(outputFrame, sobelX);
                sobelY3x3(outputFrame, sobelY);
                magnitude(sobelX, sobelY, gradientMagnitude);
                if (!gradientMagnitude.empty()) {
                    cv::imshow("Video", gradientMagnitude);
                }
                else {
                    printf("Gradient magnitude image is empty\n");
                }
            }
            else if (blurQuantizeMode) {

                blurQuantize(frame, outputFrame, quantizeLevels);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Blurred and quantized image is empty\n");
                }
            }
            else if (faceDetectionMode) {
                cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY, 0);
                detectFaces(greyFrame, faces);
                
                // Add a little smoothing by averaging the last two detections
                drawBoxes(frame, faces);
                if (faces.size() > 0) {
                    last.x = (faces[0].x + last.x) / 2;
                    last.y = (faces[0].y + last.y) / 2;
                    last.width = (faces[0].width + last.width) / 2;
                    last.height = (faces[0].height + last.height) / 2;
                }
                cv::imshow("Video", frame);
            }
            else if (pickStrongColorMode) {
                pickStrongColorToggle(frame, outputFrame, pickStrongColorMode, pickStrongColorThreshold);
            }
            else if (heartsMode) {
                cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY, 0);
                detectFaces(greyFrame, faces);
                if (heartsMode) {
                    drawHearts(frame, faces, 0, 1.0);  // Draw hearts instead of bubbles
                }
                else {
                    drawBoxes(frame, faces);  // Draw boxes if heartsMode is not enabled
                }

                // add a little smoothing by averaging the last two detections
                if (faces.size() > 0) {
                    last.x = (faces[0].x + last.x) / 2;
                    last.y = (faces[0].y + last.y) / 2;
                    last.width = (faces[0].width + last.width) / 2;
                    last.height = (faces[0].height + last.height) / 2;
                }

                // display the frame with the box or heart in it
                cv::imshow("Video", frame);
            }
            else if (embossingEnabled) {
                embossingEffect(frame, embosingFrame);
                cv::convertScaleAbs(embosingFrame, outputFrame);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Embossing image is empty\n");
                }
            }
            else {
                outputFrame = frame.clone();
                adjustBrightnessContrast(outputFrame, outputFrame, brightness, contrast);
                if (!outputFrame.empty()) {
                    cv::imshow("Video", outputFrame);
                }
                else {
                    printf("Original image is empty\n");
                }
            }
        }
        catch (cv::Exception& e) {
            fprintf(stderr, "OpenCV Exception: %s\n", e.what());
            break;
        }
        // Check for key presses to control the application
        if (key == 'q') {
            std::cout << "Quitting" << std::endl;
            break;
        }
        else if (key == 's') {
            std::cout << "Image is being Saved" << std::endl;
            std::string filename = "C:/Users/visar/Desktop/OneDrive - Northeastern University/PRCV/Project1_Images/Image_" + std::to_string(imageCounter) + ".jpg";
            cv::imwrite(filename, outputFrame);
            printf("Image saved as %s\n", filename.c_str());
            imageCounter++;
        }
    }
    // Release resources
    delete capdev;
    return 0;
}


// Function to toggle pickStrongColor effect
int pickStrongColorToggle(cv::Mat& frame, cv::Mat& outputFrame, bool& isEnabled, uchar threshold) {
    if (isEnabled) {
        pickStrongColor(frame, outputFrame, threshold);

        // Check if outputFrame is not empty before displaying
        if (!outputFrame.empty()) {
            cv::imshow("Video", outputFrame);
        }
        else {
            printf("Pick strong color image is empty\n");
        }
    }
    else {
        // Display the original frame if pickStrongColor is not enabled
        cv::imshow("Video", frame);
    }
    return 0;  // Success
}