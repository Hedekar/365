#include <math.h>	// needed for log2, ceil, floor
#include <algorithm.h> // needed for min function
// to determine how many bins for the histogram we need to know pixel size of our frames

const int WIDTH = 32;
const int HEIGHT = 32;


////code for rgb to chromaticity shift

// takes rgb input in three ints, outputs an r chromaticity float
float rConvert(rgb[3]){
	if(rgb[0]==0 && rgb[1]==0 && rgb[2]==0) return 0;
	return rgb[0]/(rgb[0]+rgb[1]+rgb[2]);
}

// takes rgb input in three ints, outputs a g chromaticity float
float gConvert(rgb[3]){
	if(rgb[0]==0 && rgb[1]==0 && rgb[2]==0) return 0;
	return rgb[1]/(rgb[0]+rgb[1]+rgb[2]);
}

// void chromConvert(videofile){
// 	VideoCapture original();
// 	Mat frame;
// 	if(!original.open(videofile)){
// 		cerr << "Video file not opened." << endl;
// 	}else{
// 		original.grab();
// 		original.retrieve(frame, 0);
// 	}
// }


////code for histogram

// gives a rough guideline of how many histogram bins are needed
int binNum(int w=WIDTH){
	return ceil(1+log2(w));
}

// This function returns the I value from any two histograms
float getI(float histT[HEIGHT][HEIGHT], float histTT[HEIGHT][HEIGHT]){
	float sum = 0;
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<HEIGHT;j++){
			sum = sum+min( histT[i][j], histTT[i][j] );
		}
	}
}

// This function takes a 2-d array as an input - rgb values for each pixel in an fcolumn - and returns a pointer to a static histogram
float * makeHist(fColumn[HEIGHT][3]){
	int hBins = binNum(HEIGHT);
	static float fColHist[hBins][hBins] = {};
	int rBin;
	int gBin;
	int rg[2];
	for(int i=1; i<HEIGHT; i++){
		int rgb[3]={ fColumn[i][0], fColumn[i][1], fColumn[2] };
		rg[0]=rConvert(rgb);
		rg[1]=gConvert(rgb)};

		rBin = floor(rg[0]*hBins);
		gBin = floor(rg[1]*hBins);
		fColHist[rBin][gBin] = fColHist[rBin][gBin] + (1/HEIGHT); //increment the hist bin
	}
}


	/* Old code we should delete soon if we don't end up needing the bin values
*	int wBins = binNum(WIDTH);
*	int hBins = binNum(HEIGHT);
*	float wHistBins[wBins]={};
*	float hHistBins[hBins]={};
*	for(int i=0; i<wBins; i++){//actually, is this needed in the code?
*		histBins[i]=i*(1/wBins);		// Upper bounds for each bin.
*	}
*	for(int i=0; i<hBins; i++){
*		histBins[i]=i*(1/hBins);		// w and h are likely =, but we create one for each in case image ratio isn't square
*	}
	*/


int main(){ // maybe command-line filename input?

	// Open file here

	int frameCount = 0; // replace this zero with the number of frames in the image
	// reduce the image to be WIDTH x HEIGHT

// this for loop nesting isn't working in my head right Essentially we only want to use two histograms at a time, not 2x32 (two full frames of 32 columns/histograms)
// we could cycle through the first column of every frame, then repeat for the second.   THIS IS HOW I CURRENTLY HAVE IT WRITTEN  I don't know if the video loading works like that

	for (int j=0; j<WIDTH; j++){ 
		for (int k=0; k<frameCount; k++){
			// get the pixels of this column in rgb form and put it into a 2-D array (r,g,b for each pixel)
			// call the makeHist function to turn the rgb array into a histogram pointer
			// call the getI function on the second iteration of k onward to compare this frame's histogram with last frame's histogram
			// copy the current histogram into the previous frame histogram array
		}
	}
}
