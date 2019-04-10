 #include <stdio.h>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
using namespace cv;
using namespace std;

int threshold_value = 100;//pixel threshold
int threshold_type = 3;//Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted
int const max_BINARY_value = 255;//max pixel value

double alpha = 1;
int beta = -100;

int scale = 1;
int delta = 0;
int ddepth = CV_16S;
int main(int argc, char** argv ){
  VideoCapture stream1(0);//open camera stream
  if(!stream1.isOpened()){
    cout << "Cannot open stream";
  }
  Mat src, src_grey, dst;//declare mats
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  while(true){//for every frame:
    stream1.read(src);//read stream and insert frame into mat
    GaussianBlur( src, src, Size(3, 3), 0, 0, BORDER_DEFAULT );//blur
    for( int y = 0; y < src.rows; y++ ) {
        for( int x = 0; x < src.cols; x++ ) {
            for( int c = 0; c < 3; c++ ) {
                src.at<Vec3b>(y,x)[c] =
                saturate_cast<uchar>( alpha*( src.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }
    cvtColor(src, src_grey, COLOR_BGR2GRAY);//convert frame into black and white
    threshold(src_grey, src_grey, threshold_value, max_BINARY_value, threshold_type );//threshold cut
    imshow("sat", src_grey);//show image
    imshow("cam", src);
    if(waitKey(30) >= 0){
      for(int i = 0; i < src_grey.cols; i++){
        cout << (int) src_grey.at<uchar>(i, 100) << " ";
        if(i == src_grey.cols - 1)
          cout << "width: " << i;
      }
      break;
    }
    this_thread::sleep_for(chrono::milliseconds(16));//wait at camera frame rate
  }
  return 0;
}
