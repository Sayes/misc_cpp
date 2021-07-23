/*
 * Copyright (c) 2017-2018 SYZ
 *
 * g++ opencv_op.cc -I$OPENCV2_HOME/include -L$OPENCV2_HOME/lib -lopencv_core
 * -lopencv_highgui -lopencv_imgproc -o release/opencv_op
 *
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("usage: opencv_op src dest\n");
    return 0;
  }
  std::string imgfn(argv[1]);
  std::string imgdestfn(argv[2]);
  cv::Mat img = cv::imread(imgfn);
  cv::Mat mat = img.clone();
  cv::resize(mat, mat, cv::Size(100, 100));
  cv::imwrite(imgdestfn, mat);

  cv::Mat img2;
  std::vector<unsigned char> vecImg;
  std::vector<int> vecCompression_params;
  vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  vecCompression_params.push_back(90);
  bool result = cv::imencode(".jpg", img2, vecImg, vecCompression_params);
  if (!result) {
    printf("imencode() failed\n");
  }

  return 0;
}
