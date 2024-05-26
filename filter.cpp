// File: filter.cpp
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 26, 2024
// All visual effects filter functions 

#include "filter.h"

// Apply an alternative grayscale transformation to the source image
int altGreyScale(cv::Mat& src, cv::Mat& dst){
    if (src.empty()) {
        return -1; // Invalid source image
    }

    dst.create(src.size(), src.type());

    // Custom greyscale transformation
    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);
            uchar grey_value = 255 - pixel[2]; // Custom transformation
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(grey_value, grey_value, grey_value);
        }
    }
    return 0; 
}

// Apply a sepia tone filter to the source image
int sepiaTone(cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) {
        return -1; // Invalid source image
    }

    dst.create(src.size(), src.type());

    const double sepiaMatrix[3][3] = {
        {0.272, 0.534, 0.131},
        {0.349, 0.686, 0.168},
        {0.393, 0.769, 0.189}
    };

    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

            // Apply sepia tone filter
            double sepiaR = sepiaMatrix[0][0] * pixel[2] + sepiaMatrix[0][1] * pixel[1] + sepiaMatrix[0][2] * pixel[0];
            double sepiaG = sepiaMatrix[1][0] * pixel[2] + sepiaMatrix[1][1] * pixel[1] + sepiaMatrix[1][2] * pixel[0];
            double sepiaB = sepiaMatrix[2][0] * pixel[2] + sepiaMatrix[2][1] * pixel[1] + sepiaMatrix[2][2] * pixel[0];

            // Clip values to the valid range [0, 255]
            sepiaR = std::min(255.0, std::max(0.0, sepiaR));
            sepiaG = std::min(255.0, std::max(0.0, sepiaG));
            sepiaB = std::min(255.0, std::max(0.0, sepiaB));

            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(sepiaR, sepiaG, sepiaB);
        }
    }

    return 0; // Success
}

// Apply a vignette effect to the source image
void Vignette(cv::Mat& src, cv::Mat& dst, double vignetteStrength, double vignetteRadius) {
    if (src.empty()) {
        return; // Invalid source image
    }

    dst.create(src.size(), src.type());

    cv::Size imgSize = src.size();
    cv::Point center(imgSize.width / 2, imgSize.height / 2);

    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

            // Calculate vignette effect
            double dist = cv::norm(center - cv::Point(x, y)) / cv::norm(center);
            double vignette = 1.0 - vignetteStrength * (1.0 - std::exp(-0.5 * std::pow(dist / vignetteRadius, 2)));

            // Apply vignette
            pixel[0] *= vignette;
            pixel[1] *= vignette;
            pixel[2] *= vignette;

            dst.at<cv::Vec3b>(y, x) = pixel;
        }
    }
}
// Apply a 5x5 blur filter to the source image (version A)
int blur5x5_A(cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) {
        return -1; // Error: Empty source image
    }

    dst = src.clone(); // Initialize dst with the same content as src

    int kernel[5][5] = { {1, 2, 4, 2, 1},
                        {2, 4, 8, 4, 2},
                        {4, 8, 16, 8, 4},
                        {2, 4, 8, 4, 2},
                        {1, 2, 4, 2, 1} };

    int kernelSum = 88; // Sum of all values in the kernel

    // Iterate over inner pixels of the image
    for (int y = 2; y < src.rows - 2; ++y) {
        for (int x = 2; x < src.cols - 2; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Apply the 5x5 kernel
                for (int ky = -2; ky <= 2; ++ky) {
                    for (int kx = -2; kx <= 2; ++kx) {
                        sum += src.at<cv::Vec3b>(y + ky, x + kx)[c] * kernel[ky + 2][kx + 2];
                    }
                }

                // Normalize the result
                sum /= kernelSum;

                // Update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(sum);
            }
        }
    }

    return 0; // Success
}


int blur5x5_B(cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) {
        return -1; // Error: Empty source image
    }

    dst = src.clone(); // Initialize dst with the same content as src

    int kernel5[5] = { -5, 0, 20, 0, -5 };

    // Apply horizontal blur
    for (int y = 0; y < src.rows; ++y) {
        for (int x = 2; x < src.cols - 2; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Apply the 1x5 kernel horizontally
                sum += src.data[(y * src.cols + x - 2) * src.channels() + c] * kernel5[0];
                sum += src.data[(y * src.cols + x - 1) * src.channels() + c] * kernel5[1];
                sum += src.data[(y * src.cols + x) * src.channels() + c] * kernel5[2];
                sum += src.data[(y * src.cols + x + 1) * src.channels() + c] * kernel5[3];
                sum += src.data[(y * src.cols + x + 2) * src.channels() + c] * kernel5[4];

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.data[(y * dst.cols + x) * dst.channels() + c] = static_cast<uchar>(sum);
            }
        }
    }

    // Apply vertical blur
    for (int y = 2; y < src.rows - 2; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Apply the 1x5 kernel vertically
                sum += dst.data[((y - 2) * dst.cols + x) * dst.channels() + c] * kernel5[0];
                sum += dst.data[((y - 1) * dst.cols + x) * dst.channels() + c] * kernel5[1];
                sum += dst.data[(y * dst.cols + x) * dst.channels() + c] * kernel5[2];
                sum += dst.data[((y + 1) * dst.cols + x) * dst.channels() + c] * kernel5[3];
                sum += dst.data[((y + 2) * dst.cols + x) * dst.channels() + c] * kernel5[4];

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.data[(y * dst.cols + x) * dst.channels() + c] = static_cast<uchar>(sum);
            }
        }
    }

    return 0; // Success
}

// Apply a 5x5 blur filter to the source image (version B)
int blur5x5_2(cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) {
        return -1; // Error: Empty source image
    }

    dst = src.clone(); // Initialize dst with the same content as src

    int kernel5[5] = { -5, 0, 20, 0, -5 };

    // Apply horizontal blur
    for (int y = 0; y < src.rows; ++y) {
        for (int x = 2; x < src.cols - 2; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Apply the 1x5 kernel horizontally
                sum += src.data[(y * src.cols + x - 2) * src.channels() + c] * kernel5[0];
                sum += src.data[(y * src.cols + x - 1) * src.channels() + c] * kernel5[1];
                sum += src.data[(y * src.cols + x) * src.channels() + c] * kernel5[2];
                sum += src.data[(y * src.cols + x + 1) * src.channels() + c] * kernel5[3];
                sum += src.data[(y * src.cols + x + 2) * src.channels() + c] * kernel5[4];

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.data[(y * dst.cols + x) * dst.channels() + c] = static_cast<uchar>(sum);
            }
        }
    }

    // Apply vertical blur
    for (int y = 2; y < src.rows - 2; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            for (int c = 0; c < src.channels(); ++c) {
                int sum = 0;

                // Apply the 1x5 kernel vertically
                sum += dst.data[((y - 2) * dst.cols + x) * dst.channels() + c] * kernel5[0];
                sum += dst.data[((y - 1) * dst.cols + x) * dst.channels() + c] * kernel5[1];
                sum += dst.data[(y * dst.cols + x) * dst.channels() + c] * kernel5[2];
                sum += dst.data[((y + 1) * dst.cols + x) * dst.channels() + c] * kernel5[3];
                sum += dst.data[((y + 2) * dst.cols + x) * dst.channels() + c] * kernel5[4];

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.data[(y * dst.cols + x) * dst.channels() + c] = static_cast<uchar>(sum);
            }
        }
    }

    return 0; // Success
}

// Apply a 3x3 Sobel X filter to the source image
int sobelX3x3(cv::Mat& src, cv::Mat& dst) {
    //allocate dst image
    dst = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    cv::Mat temp_h = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    //loop over src and apply a 3x3 filter
    for (int i = 1; i < src.rows - 1; i++) {

        //src pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        //destination pointer
        cv::Vec3s* dptr = temp_h.ptr<cv::Vec3s>(i);
        //for each column 
        for (int j = 1; j < src.cols - 1; j++) {
            //for each color channel 
            for (int c = 0; c < 3; c++) {
                dptr[j][c] = (-1 * rptr[j - 1][c] + 0 * rptr[j][c] + 1 * rptr[j + 1][c]); // Apply 1x3 horizontal filter
            }
        }
    }

    int v_kernel[3] = { 1, 2, 1 }; // 1x3 vertical kernel 
    // Loop over rows
    for (int r = 1; r < src.rows - 1; r++) {
        // Loop over cols
        for (int c = 1; c < src.cols - 1; c++) {
            cv::Vec3s sum(0, 0, 0);
            for (int i = -1; i <= 1; i++)
            {
                int y = r + i;
                if (y < 0 || y >= src.rows)
                    continue;

                //Source pointer
                cv::Vec3s* pixel = temp_h.ptr<cv::Vec3s>(y);
                float weight = v_kernel[i + 1]; //Apply the filter
                sum += pixel[c] * weight;
            }
            //Destination Pointer
            cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(r);
            dptr[c] = sum;
        }
    }
    return 0;
}

// Apply a 3x3 Sobel Y filter to the source image
int sobelY3x3(cv::Mat& src, cv::Mat& dst) {
    //allocate dst image
    dst = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    cv::Mat temp_h = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    //loop over src and apply a 3x3 filter
    for (int i = 1; i < src.rows - 1; i++) {
        //src pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        //destination pointer
        cv::Vec3s* dptr = temp_h.ptr<cv::Vec3s>(i);
        //for each column 
        for (int j = 1; j < src.cols - 1; j++) {
            //for each color channel 
            for (int c = 0; c < 3; c++) {
                dptr[j][c] = (1 * rptr[j - 1][c] + 2 * rptr[j][c] + 1 * rptr[j + 1][c]); //Apply 1x3 horizontal filter
            }
        }
    }

    int v_kernel[3] = { 1, 0, -1 }; //vertical kernel 
    // Loop over rows
    for (int r = 1; r < src.rows - 1; r++) {
        // Loop over cols
        for (int c = 1; c < src.cols - 1; c++) {
            cv::Vec3s sum(0, 0, 0);
            for (int i = -1; i <= 1; i++)
            {
                int y = r + i;
                if (y < 0 || y >= src.rows)
                    continue;

                //Source pointer
                cv::Vec3s* pixel = temp_h.ptr<cv::Vec3s>(y);
                float weight = v_kernel[i + 1]; //Apply 1x3 vertical filter 
                sum += pixel[c] * weight;
            }
            //Destination Pointer
            cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(r);
            dptr[c] = sum;
        }
    }
    return 0;
}
int gradientMagnitudeEuclidean(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {
    // Create the destination matrix with the same size as Sobel X (sx) and Sobel Y (sy)
    // The destination matrix is of type CV_32FC3 (32-bit floating-point with 3 channels)
    dst.create(sx.size(), CV_32FC3);

    // Loop over each row in the images
    for (int i = 0; i < sx.rows; i++) {
        // Get pointers to the current row in Sobel X, Sobel Y, and the destination matrices
        cv::Vec3s* xptr = sx.ptr<cv::Vec3s>(i);
        cv::Vec3s* yptr = sy.ptr<cv::Vec3s>(i);
        cv::Vec3f* dptr = dst.ptr<cv::Vec3f>(i);

        // Loop over each column in the images
        for (int j = 0; j < sx.cols; j++) {
            // Loop over each channel (R, G, B) in the images
            for (int k = 0; k < sx.channels(); k++) {
                // Calculate the Euclidean distance (magnitude) for each channel
                float magnitude = sqrt(static_cast<float>(xptr[j][k]) * static_cast<float>(xptr[j][k]) +
                    static_cast<float>(yptr[j][k]) * static_cast<float>(yptr[j][k]));

                // Normalize the magnitude to the range [0, 255]
                magnitude = std::min(std::max(magnitude, 0.0f), 255.0f);

                // Store the normalized magnitude in the destination matrix
                dptr[j][k] = magnitude;
            }
        }
    }

    return 0;
}


void blurQuantize(cv::Mat& src, cv::Mat& dst, int levels) {
    // Apply a 5x5 blur using separable filters
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(5, 5), 0);

    // Quantize the image
    dst = blurred.clone();

    // Calculate the size of a bucket
    float bucketSize = 255.0f / levels;

    // Quantize each pixel in each color channel
    for (int y = 0; y < dst.rows; ++y) {
        for (int x = 0; x < dst.cols; ++x) {
            for (int c = 0; c < dst.channels(); ++c) {
                float originalValue = dst.at<cv::Vec3b>(y, x)[c];
                float quantizedValue = floor(originalValue / bucketSize + 0.5) * bucketSize;
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(quantizedValue);
            }
        }
    }
}

int embossingEffect(cv::Mat& src, cv::Mat& dst) {
    // Create temporary matrices for Sobel X and Sobel Y results
    cv::Mat sobelX, sobelY;

    // Apply SobelX filter
    sobelX3x3(src, sobelX);

    // Apply SobelY filter
    sobelY3x3(src, sobelY);

    // Combine SobelX and SobelY results for embossing effect
    dst = cv::Mat::zeros(src.size(), CV_16SC3); // Initialize destination matrix

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            for (int c = 0; c < 3; c++) {
                // Combine SobelX and SobelY results
                int embossValue = std::abs(sobelX.at<cv::Vec3s>(i, j)[c]) + std::abs(sobelY.at<cv::Vec3s>(i, j)[c]);

                // Clamp the result to 255 to prevent overflow
                embossValue = std::min(embossValue, 255);

                // Assign the embossing value to the destination pixel
                dst.at<cv::Vec3s>(i, j)[c] = embossValue;
            }
        }
    }

    return 0;
}

int pickStrongColor(cv::Mat& src, cv::Mat& dst, uchar threshold) {
    if (src.empty()) {
        return -1; // Invalid source image
    }

    dst.create(src.size(), src.type());

    // Iterate over each pixel in the image
    for (int y = 0; y < src.rows; ++y) {
        for (int x = 0; x < src.cols; ++x) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

            // Calculate the intensity (brightness) of the pixel
            uchar intensity = static_cast<uchar>((pixel[0] + pixel[1] + pixel[2]) / 3);

            // Check if intensity is greater than the threshold
            if (intensity > threshold) {
                // Keep the original color for strong pixels
                dst.at<cv::Vec3b>(y, x) = pixel;
            }
            else {
                // Convert everything else to greyscale
                uchar grey_value = intensity;
                dst.at<cv::Vec3b>(y, x) = cv::Vec3b(grey_value, grey_value, grey_value);
            }
        }
    }

    return 0; // Success
}