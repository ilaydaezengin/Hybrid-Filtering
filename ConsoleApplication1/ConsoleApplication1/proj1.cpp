/*Before trying to construct hybrid images, it is suggested that you
implement my_imfilter() and then debug it using proj1_test_filtering.cpp */


#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat my_imfilter(Mat, Mat);
Mat vis_hybrid_image(Mat);



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



	////////////////////////
	//Write your code here
	////////////////////////




	return outI;


}

Mat vis_hybrid_image(Mat hybrid_image)
{
	//visualize a hybrid image by progressively downsampling the image and
	//concatenating all of the images together.		
	int scales = 5; //how many downsampled versions to create		
	double scale_factor = 0.5; //how much to downsample each time		
	int padding = 5; //how many pixels to pad.
	int original_height = hybrid_image.rows; // height of the image
	int num_colors = hybrid_image.channels(); //counting how many color channels the input has
	Mat output = hybrid_image;
	Mat cur_image = hybrid_image;

	for (int i = 2; i <= scales; i++)
	{
		//add padding
		hconcat(output, Mat::ones(original_height, padding, CV_8UC3), output);

		//dowsample image;
		resize(cur_image, cur_image, Size(0, 0), scale_factor, scale_factor, INTER_LINEAR);

		//pad the top and append to the output
		Mat tmp;
		vconcat(Mat::ones(original_height - cur_image.rows, cur_image.cols, CV_8UC3), cur_image, tmp);
		hconcat(output, tmp, output);
	}

	return output;
}

int main()
{
	//// Setup  ////
	//Read images
	Mat image1 = imread("../data/dog.bmp");
	if (!image1.data)                              // Check for invalid image
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat image2 = imread("../data/cat.bmp");
	if (!image2.data)                              // Check for invalid image
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	image1.convertTo(image1, CV_64FC3);
	image2.convertTo(image2, CV_64FC3);


	/*Several additional test cases are provided for you, but feel free to make
	your own(you'll need to align the images in a photo editor such as
	Photoshop).The hybrid images will differ depending on which image you
	assign as image1(which will provide the low frequencies) and which image
	you asign as image2(which will provide the high frequencies) */


	////  Filtering and Hybrid Image construction  ////
	int cutoff_frequency = 7; /*This is the standard deviation, in pixels, of the
							  Gaussian blur that will remove the high frequencies from one image and
							  remove the low frequencies from another image (by subtracting a blurred
							  version from the original version). You will want to tune this for every
							  image pair to get the best results.*/

	Mat filter = getGaussianKernel(cutoff_frequency * 4 + 1, cutoff_frequency, CV_64F);
	filter = filter*filter.t();


	/////////////////////////////////////////////////////////////////
	// YOUR CODE BELOW. Use my_imfilter(), create 'low_frequencies' and
	// 'high_frequencies' and then combine them to create 'hybrid_image'
	//////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Remove the high frequencies from image1 by blurring it.The amount of
	// blur that works best will vary with different image pairs
	///////////////////////////////////////////////////////////////////////

	//Mat low_frequencies =

	/////////////////////////////////////////////////////////////////////////
	// Remove the low frequencies from image2. The easiest way to do this is to
	// subtract a blurred version of image2 from the original version of image2.
	// This will give you an image centered at zero with negative values.
	//////////////////////////////////////////////////////////////////////////

	//Mat high_frequencies =

	////////////////////////////////////////////////////////////////////////
	// Combine the high frequencies and low frequencies
	/////////////////////////////////////////////////////////////////////////

	//Mat hybrid_image =


	////  Visualize and save outputs  ////
	//Convert the resulting images type to the 8 bit unsigned integer matrix with 3 channels
	high_frequencies = high_frequencies + Scalar(0.5, 0.5, 0.5) * 255;//the scalar added because the output image is centered around zero otherwise and mostly black	
	high_frequencies.convertTo(high_frequencies, CV_8UC3);
	low_frequencies.convertTo(low_frequencies, CV_8UC3);
	hybrid_image.convertTo(hybrid_image, CV_8UC3);

	Mat vis = vis_hybrid_image(hybrid_image);

	imshow("Low frequencies", low_frequencies); waitKey(0);
	imshow("High frequencies", high_frequencies);	waitKey(0);
	imshow("Hybrid image", vis); waitKey(0);

	imwrite("low_frequencies.jpg", low_frequencies);
	imwrite("high_frequencies.jpg", high_frequencies);
	imwrite("hybrid_image.jpg", hybrid_image);
	imwrite("hybrid_image_scales.jpg", vis);
}