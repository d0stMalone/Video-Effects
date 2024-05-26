// File: filter.h
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 26, 2024

#pragma once
#include <opencv2/opencv.hpp>

int altGreyScale(cv::Mat& src, cv::Mat& dst);

int sepiaTone(cv::Mat& src, cv::Mat& dst);
void Vignette(cv::Mat& src, cv::Mat& dst, double vignetteStrength = 0.8, double vignetteRadius = 0.7);

int blur5x5_A(cv::Mat& src, cv::Mat& dst);
int blur5x5_B(cv::Mat& src, cv::Mat& dst);

int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);

int gradientMagnitudeEuclidean(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);

void blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);

int embossingEffect(cv::Mat& src, cv::Mat& dst);

int drawHearts(cv::Mat& frame, std::vector<cv::Rect>& faces, int minWidth, float scale);

void drawHeart(cv::Mat& img, const cv::Point& center, int size);

int pickStrongColor(cv::Mat& src, cv::Mat& dst, uchar threshold = 128);

