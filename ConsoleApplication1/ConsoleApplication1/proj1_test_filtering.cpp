
/*This script has test cases to help you test my_imfilter() which you will
write.You should verify that you get reasonable output here before using
your filtering to construct a hybrid image in proj1.cpp.The outputs are
all saved and you can include them in your writeup. You can add calls to
filter2D() if you want to check that my_imfilter() is doing something
similar.*/

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat my_imfilter(Mat, Mat);



Mat my_imfilter(Mat im, Mat filter)
{
	/*This function is intended to behave like the built in function filter2D()

	Your function should work for color images. Simply filter each color
	channel independently.

	Your function should work for filters of any width and height
	combination, as long as the width and height are odd(e.g. 1, 7, 9).This
	restriction makes it unambigious which pixel in the filter is the center
	pixel.

	Boundary handling can be tricky.The filter can't be centered on pixels
	at the image boundary without parts of the filter being out of bounds.
	There are several options to deal with boundaries. -- pad the input image with zeros, and
	return a filtered image which matches the input resolution. A better
	approach is to mirror the image content over the boundaries for padding.*/

	Mat outI;
	int top = (filter.rows - 1) / 2; int bottom = (filter.rows - 1) / 2;
	int left = (filter.rows - 1) / 2; int right = (filter.rows - 1) / 2;
	int channel = im.channels();
	Scalar value = Scalar(0, 0, 0);

	copyMakeBorder(im, outI, top, bottom, left, right, BORDER_CONSTANT, value);

	for (int y = (filter.rows - 1) / 2; y < outI.rows - ((filter.rows - 1) / 2); y++) {
		for (int x = (filter.cols - 1) / 2; x < outI.cols - ((filter.cols - 1) / 2); x++) {

			double sum1 = 0.0;
			double sum2 = 0.0;
			double sum3 = 0.0;
			for (int k = -(filter.rows - 1) / 2; k <= (filter.rows - 1) / 2; k++) {
				for (int j = -(filter.cols - 1) / 2; j <= (filter.cols - 1) / 2; j++) {

					Vec3d intensity = outI.at<Vec3d>(y+k,x+j);
					double chan1 = intensity.val[0];
					double chan2 = intensity.val[1];
					double chan3 = intensity.val[2];

					sum1 += chan1 * filter.at<double>(j+ (filter.rows-1)/2 ,k+ (filter.rows - 1) / 2);
					sum2 += chan2 * filter.at<double>(j + (filter.rows - 1) / 2, k + (filter.rows - 1) / 2);
					sum3 += chan3 * filter.at<double>(j + (filter.rows - 1) / 2, k + (filter.rows - 1) / 2);
				}
			}

			outI.at<Vec3d>(y, x).val[0] = sum1;
			outI.at<Vec3d>(y, x).val[1] = sum2;
			outI.at<Vec3d>(y, x).val[2] = sum3;


		}
	}
	return outI;

}


	

int main()
{
	//// Setup  ////
	//Load the test image
	Mat test_image = imread("../data/cat.bmp");
	if (!test_image.data)                              // Check for invalid image
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	imshow("Test image", test_image);                   // Show the test image.
	waitKey(0);                                          // Wait for a keystroke in the window
	test_image.convertTo(test_image, CV_64FC3);

	//// Identify filter  ////
	//This filter should do nothing regardless of the padding method you use.
	Mat identity_filter = (Mat_<double>(3, 3) << 0, 0, 0, 0, 1, 0, 0, 0, 0);
	//cout << identity_filter.at<double>(0,0); 
	Mat identity_image = my_imfilter(test_image, identity_filter);
	identity_image.convertTo(identity_image, CV_8UC3);
	imshow("Identity image", identity_image);
	waitKey(0);
	imwrite("identity_image.jpg", identity_image); //save the identity image as jpeg

												   ////  Small blur with a box filter ////
												   //This filter should remove some high frequencies
	Mat blur_filter = (Mat_<double>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	blur_filter = blur_filter / sum(blur_filter)[0]; //making the filter sum to 1
	Mat blur_image = my_imfilter(test_image, blur_filter);
	blur_image.convertTo(blur_image, CV_8UC3);
	imshow("Blur image", blur_image);
	waitKey(0);
	imwrite("blur_image.jpg", blur_image); //save the blur image as jpeg

										   ////   Large blur  ////
										   //This blur would be slow to do directly, so we instead use the fact that
										   //Gaussian blurs are separable and blur sequentially in each direction.
	Mat large_1d_blur_filter = getGaussianKernel(25, 10, CV_64F);
	Mat large_blur_image = my_imfilter(test_image, large_1d_blur_filter);
	large_blur_image = my_imfilter(large_blur_image, large_1d_blur_filter.t()); //notice the t() operator which transposes the filter
	large_blur_image.convertTo(large_blur_image, CV_8UC3);
	imshow("Large blur image", large_blur_image); waitKey(0);
	imwrite("large_blur_image.jpg", large_blur_image); //save the large blur image as jpeg


													   ////  Oriented filter(Sobel Operator)  ////
	Mat sobel_filter = (Mat_<double>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1); //should respond to horizontal gradients
	Mat sobel_image = my_imfilter(test_image, sobel_filter);

	//the scalar value is added because the output image is centered around zero otherwise and mostly black
	sobel_image = sobel_image + Scalar(0.5, 0.5, 0.5) * 255;
	sobel_image.convertTo(sobel_image, CV_8UC3);
	imshow("Sobel image", sobel_image); waitKey(0);
	imwrite("sobel_image.jpg", sobel_image);


	////  High pass filter(Discrete Laplacian)   ////
	Mat laplacian_filter = (Mat_<double>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
	Mat laplacian_image = my_imfilter(test_image, laplacian_filter);
	//the scalar value is added because the output image is centered around zero otherwise and mostly black
	laplacian_image = laplacian_image + Scalar(0.5, 0.5, 0.5) * 255;
	laplacian_image.convertTo(laplacian_image, CV_8UC3);
	imshow("Laplacian image", laplacian_image); waitKey(0);
	imwrite("laplacian_image.jpg", laplacian_image);

	//// High pass "filter" alternative  ////
	blur_image.convertTo(blur_image, CV_64FC3);
	Mat high_pass_image = test_image - blur_image; //simply subtract the low frequency content

												   //the scalar value is added because the output image is centered around zero otherwise and mostly black
	high_pass_image = high_pass_image + Scalar(0.5, 0.5, 0.5) * 255;
	high_pass_image.convertTo(high_pass_image, CV_8UC3);
	imshow("high pass image", high_pass_image); waitKey(0);
	imwrite("high_pass_image.jpg", high_pass_image);

}