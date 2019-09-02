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
  return 0;
}
