#include <math.h>	// needed for log2, ceil, floor
#include <algorithm> // needed for min function
#include <string>
#include <iostream>
#include <iterator>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
// to determine how many bins for the histogram we need to know pixel size of our frames

using namespace cv;
using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 720;
////code for rgb to chromaticity shift

// takes rgb input in three uchars, outputs an r chromaticity uchar
uchar rConvert(uchar bgr[3]){ //in OpenCV, pixel values are BGR, pixel values typically uchars
	if(bgr[0]==0 && bgr[1]==0 && bgr[2]==0) return 0;
	int b = bgr[0];
	int g = bgr[1];
	int r = bgr[2];
	double convert = r / (b + g + r);
	return (unsigned char)(r / (b + g + r));
}

// takes rgb input in three uchars, outputs a g chromaticity uchar
uchar gConvert(uchar bgr[3]){ //in OpenCV, pixel values are BGR, pixel values typically uchars
	if (bgr[0] == 0 && bgr[1] == 0 && bgr[2] == 0) return 0;
	return bgr[1] / (bgr[0] + bgr[1] + bgr[2]);
}

////code for histogram

// gives a rough guideline of how many histogram bins are needed
int binNum(int w=WIDTH){
	return ceil(1+log2(w));
}

const int HBINS = binNum(HEIGHT);

// This function returns the I value from any two columns
float getI(float histT[HEIGHT][HEIGHT], float histTT[HEIGHT][HEIGHT]){
	float sum = 0;
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<HEIGHT;j++){
			sum = sum+min( histT[i][j], histTT[i][j] );
		}
	}
}

 //This function takes a 2-d array as an input - rgb values for each pixel in an fcolumn - and returns a pointer to a static histogram
//float * makeHist(float fColumn[HEIGHT][3]){
//	static float fColHist[HBINS][HBINS] = {};
//	int rBin;
//	int gBin;
//	int rg[2];
//	for(int i=1; i<HEIGHT; i++){
//		int rgb[3]={ fColumn[i][0], fColumn[i][1], fColumn[2] };
//		rg[0]=rConvert(rgb);
//		rg[1]=gConvert(rgb);
//
//		rBin = floor(rg[0]*HBINS);
//		gBin = floor(rg[1]*HBINS);
//		fColHist[rBin][gBin] = fColHist[rBin][gBin] + (1/HEIGHT); //increment the hist bin
//	}
//}

//float * copyHist(origHist[HBINS][HBINS]){
//	static float newHist[HBINS][HBINS] = {};
//	for(int i=0; i<HBINS; i++){
//		for(int j=0; j<HBINS; j++){
//			newHist[i][j] = origHist[i][j];
//		}
//	}
//}

	/* Old code we should delete soon if we don't end up needing the bin values
*	int wBins = binNum(WIDTH);
*	int HBINS = binNum(HEIGHT);
*	float wHistBins[wBins]={};
*	float hHistBins[HBINS]={};
*	for(int i=0; i<wBins; i++){//actually, is this needed in the code?
*		histBins[i]=i*(1/wBins);		// Upper bounds for each bin.
*	}
*	for(int i=0; i<HBINS; i++){
*		histBins[i]=i*(1/HBINS);		// w and h are likely =, but we create one for each in case image ratio isn't square
*	}
	*/



//takes r g values for two columns, returns column of STI
vector <vector <int>> makeHist(float col[HEIGHT][2]){
	vector <vector <int>> hist;
	hist.resize(HBINS, vector <int>(HBINS, 0));
	int rBin, gBin;

	for(int i = 0; i < HEIGHT; i++){	
		rBin = floor(col[i][0] * HBINS);
		gBin = floor(col[i][1] * HBINS);
		hist[rBin][gBin] = hist[rBin][gBin] + 1; //increment the hist bin
	}
	return hist;
}

int main(){ // maybe command-line filename input?

	const int frameCount = 10; // replace this zero with the number of frames in the image
	int ouputSTI[WIDTH][frameCount - 1] = {};
	float *histT2;
	float *histT1;
	int currentFColumn[HEIGHT][3];
	string filename;
	Mat origFrame, chromFrame;
	Vec3b colourPixel;
	Vec3b chromPixel;
	float pixelArray[3] = {};
	float rg1[HEIGHT][WIDTH][2];
	float rg2[HEIGHT][WIDTH][2];
	float col1[HEIGHT][2];
	float col2[HEIGHT][2];
	bool firstTime = true;
	//cout << "Please enter name of video file: " << endl;
	//cin >> filename;
	filename = "star.mp4";
	VideoCapture cap(filename);
	if(!cap.isOpened()){
		cout << "Video cannot be opened" << endl;
		return 1;
	}else{
		namedWindow("Video", WINDOW_AUTOSIZE);
	}
	while(1){
		cap >> origFrame;
		if(origFrame.empty()){
			destroyWindow("Video");
			break;
		}
		//chromFrame.create(origFrame.rows, origFrame.cols, CV_8UC3);

		for (int i = 0; i < origFrame.cols; i++) {
			for (int j = 0; j < origFrame.rows; j++) {
				Vec3b &colourPixel = origFrame.at<Vec3b>(j, i);
				//Vec3b &chromPixel = chromFrame.at<Vec3b>(j, i);
				pixelArray[0] = colourPixel.val[0]; //get blue
				pixelArray[1] = colourPixel.val[1]; //get green
				pixelArray[2] = colourPixel.val[2]; //get red

				if (pixelArray[0] == 0 && pixelArray[1] == 0 && pixelArray[2] == 0){ // if black pixel
					rg2[j][i][0] = 0;
					rg2[j][i][1] = 0;
				}
				else{
					rg2[j][i][0] = pixelArray[2] / (pixelArray[0] + pixelArray[1] + pixelArray[2]); //calculate r and g values
					rg2[j][i][1] = pixelArray[1] / (pixelArray[0] + pixelArray[1] + pixelArray[2]);
				}

				//chromPixel[0] = 0;			//used for playing chromaticity video
				//chromPixel[1] = g[j] * 255;
				//chromPixel[2] = r[j] * 255;
			}
		}
		if (!firstTime){
			for (int i = 0; i < origFrame.cols; i++) { //get each column, then make histogram and I values
				for (int j = 0; j < origFrame.rows; j++) {
					col1[j][0] = rg1[j][i][0];
					col1[j][1] = rg1[j][i][1];
					col2[j][0] = rg2[j][i][0];
					col2[j][1] = rg2[j][i][1];
				}

			}
		}
		copy(begin(rg2), end(rg2), begin(rg1));
		firstTime = false;

		//imshow("Video", chromFrame);  //used for playing video
		//waitKey(20);
	}



// this for loop nesting isn't working in my head right Essentially we only want to use two histograms at a time, not 2x32 (two full frames of 32 columns/histograms)
// we could cycle through the first column of every frame, then repeat for the second.   THIS IS HOW I CURRENTLY HAVE IT WRITTEN  I don't know if the video loading works like that
	//for (int j=0; j<WIDTH; j++){ 
	//	for (int k=0; k<frameCount; k++){
	//		// get the pixels of this column in rgb form and put it into a 2-D array (r,g,b for each pixel)

	//		// call the makeHist function to turn the rgb array into a histogram pointer
	//		histT2 = makeHist(currentFColumn);

	//		// call the getI function on the second iteration of k onward to compare this frame's histogram with last frame's histogram
	//		if(k>0){outputSTI[j][k-1]= getI(histT2,histT1);}

	//		// copy the current histogram into the previous frame histogram array
	//		histT1 = copyHist(histT2);
	//	}
	//}
}
