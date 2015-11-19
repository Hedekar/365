#include <math.h>	// needed for log2, ceil
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


////code for histogram

// gives a rough guideline of how many histogram bins are needed
int binNum(int w=WIDTH){
	return ceil(1+log2(w));
}

	// function to divide 0-1 into binNum bins goes here (we'll use upper bounds)
int main(){ // maybe command-line filename input?
	int wBins = binNum(WIDTH);
	int hBins = binNum(HEIGHT);
	float wHistBins[wBins]={};
	float hHistBins[hBins]={};
	for(int i=0; i<wBins; i++){//actually, is this needed in the code?
		histBins[i]=i*(1/wBins);		// Upper bounds for each bin.
	}
	for(int i=0; i<hBins; i++){
		histBins[i]=i*(1/hBins);		// w and h are likely =, but we create one for each in case image ratio isn't square
	}

    //// This bit is coded for columns.  We'll need to do the same for rows.
	// create 2-d array for current histogram
	int frameHist[hBins][hBins] = {};
	// for each pixel in this col/row turn rgb to rg, see what bin it fits in, increment count of hist bin
	for(int i=1; i<HEIGHT; i++){
		int rgb[3]={}//get pixel rgb here. (we'll need to wrap this for loop in a couple for loops to cycle through cols & frames)
		int rg[2]={rConvert(rgb),gConvert(rgb)};
		framHist[ floor(rg[0]*hBins) ][ floor(rg[1]*hbins) ]++; //increment the hist bin
	}
}
