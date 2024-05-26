// File: faceDetect.cpp
// Author: Keval Visaria and Chirag Dhoka Jain
// Date: January 26, 2024
//The path to the Haar cascade file is define in faceDetect.h also added drawing hearts on the face detected function

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "faceDetect.h"


/*
  Arguments:
  cv::Mat grey  - a greyscale source image in which to detect faces
  std::vector<cv::Rect> &faces - a standard vector of cv::Rect rectangles indicating where faces were found
     if the length of the vector is zero, no faces were found
 */
int detectFaces( cv::Mat &grey, std::vector<cv::Rect> &faces ) {
  // a static variable to hold a half-size image
  static cv::Mat half;
  
  // a static variable to hold the classifier
  static cv::CascadeClassifier face_cascade("C:/Users/visar/source/repos/VideoDisplay/VideoDisplay/haarcascade_frontalface_alt2.xml");

  // the path to the haar cascade file
  static cv::String face_cascade_file(FACE_CASCADE_FILE);

  if( face_cascade.empty() ) {
    if( !face_cascade.load( face_cascade_file ) ) {
      printf("Unable to load face cascade file\n");
      printf("Terminating\n");
      exit(-1);
    }
  }

  // clear the vector of faces
  faces.clear();
  
  // cut the image size in half to reduce processing time
  cv::resize( grey, half, cv::Size(grey.cols/2, grey.rows/2) );

  // equalize the image
  cv::equalizeHist( half, half );

  // apply the Haar cascade detector
  face_cascade.detectMultiScale( half, faces );

  // adjust the rectangle sizes back to the full size image
  for(int i=0;i<faces.size();i++) {
    faces[i].x *= 2;
    faces[i].y *= 2;
    faces[i].width *= 2;
    faces[i].height *= 2;
  }

  return(0);
}

/* Draws rectangles into frame given a vector of rectangles
   
   Arguments:
   cv::Mat &frame - image in which to draw the rectangles
   std::vector<cv::Rect> &faces - standard vector of cv::Rect rectangles
   int minSize - ignore rectangles with a width small than this argument
   float scale - scale the rectangle values by this factor (in case frame is different than the source image)
 */
int drawBoxes( cv::Mat &frame, std::vector<cv::Rect> &faces, int minWidth, float scale  ) {
  // The color to draw, you can change it here (B, G, R)
  cv::Scalar wcolor(170, 120, 110);

  for(int i=0;i<faces.size();i++) {
    if( faces[i].width > minWidth ) {
      cv::Rect face( faces[i] );
      face.x *= scale;
      face.y *= scale;
      face.width *= scale;
      face.height *= scale;
      cv::rectangle( frame, face, wcolor, 3 );
    }
  }

  return(0);
}

// Put sparkles in a halo above the face. /
/* Draws rectangles into frame given a vector of rectangles

   Arguments:
   cv::Mat &frame - image in which to draw the rectangles
   std::vector<cv::Rect> &faces - standard vector of cv::Rect rectangles
   int minSize - ignore rectangles with a width small than this argument
   float scale - scale the rectangle values by this factor (in case frame is different than the source image)
 */

 /*
   Function to draw heart shapes above detected faces in an image.

   Arguments:
   cv::Mat &frame - image in which to draw the hearts
   std::vector<cv::Rect> &faces - standard vector of cv::Rect rectangles
   int minWidth - ignore rectangles with a width smaller than this argument
   float scale - scale the rectangle values by this factor (in case frame is different than the source image)
  */

void drawHeart(cv::Mat& img, const cv::Point& center, int size) {
    // The color to draw the hearts
    cv::Scalar heartColor(0, 0, 255); // Pink color

    int thickness = -1; // Fill the heart shape

    // Draw two filled circles
    cv::ellipse(img, center + cv::Point(0, size / 4), cv::Size(size / 2, size / 2), 0, 0, 180, heartColor, thickness, 8, 0);
    cv::ellipse(img, center + cv::Point(size / 2, size / 4), cv::Size(size / 2, size / 2), 0, 0, 180, heartColor, thickness, 8, 0);

    // Draw the triangle part of the heart
    cv::Point trianglePoints[3];
    trianglePoints[0] = center + cv::Point(0, size / 4);
    trianglePoints[1] = center + cv::Point(size / 2, size / 2);
    trianglePoints[2] = center + cv::Point(size, size / 4);
    const cv::Point* ppt[1] = { trianglePoints };
    int npt[] = { 3 };
    cv::fillPoly(img, ppt, npt, 1, heartColor, 8);
}


/*Function to draw a heart shape at a specific position in an image.

Arguments:
cv::Mat& img - image in which to draw the heart
const cv::Point & center - position of the heart
int size - size of the heart
*/
int drawHearts(cv::Mat& frame, std::vector<cv::Rect>& faces, int minWidth, float scale) {
 // Random seed for heart positions
     std::srand(static_cast<unsigned int>(std::time(0)));

     for (size_t i = 0; i < faces.size(); i++) {
         if (faces[i].width > minWidth) {
             cv::Rect face(faces[i]);
             face.x *= scale;
             face.y *= scale;
             face.width *= scale;
             face.height *= scale;

             // Draw the rectangle around the face
             cv::Scalar wcolor(170, 120, 110);
             cv::rectangle(frame, face, wcolor, 3);

             // Draw hearts above the face
             int numHearts = 5;
             for (int j = 0; j < numHearts; j++) {
                 int heartSize = std::rand() % 30 + 20; // Random size between 20 and 50

                 // Randomly generate heart position outside the face rectangle
                 int heartX = face.x + std::rand() % (face.width - heartSize);
                 int heartY = face.y - heartSize - std::rand() % (2 * heartSize);

                 // Draw the heart
                 drawHeart(frame, cv::Point(heartX, heartY), heartSize);
             }
         }
     }

     return 0;
 }


