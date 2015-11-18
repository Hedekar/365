#include <math.h>	// needed for log2, ceil
// to determine how many bins for the histogram we need to know pixel size of our frames

const int WIDTH = 32;
const int HEIGHT = 32;


////code for rgb to chromaticity shift

// takes rgb input in three ints, outputs an r chromaticity float
float rConvert(int red, int green, int blue){
	return red/(red+green+blue);
}

// takes rgb input in three ints, outputs a g chromaticity float
float gConvert(int red, int green, int blue){
	return green/(red+green+blue);
}


////code for histogram

// gives a rough guideline of how many histogram bins are needed
int binNum(int w=WIDTH){
	return ceil(1+log2(w));
};

// function to divide 0-1 into binNum bins goes here

// create 2-d array for current histogram

// for each pixel in this col/row turn rgb to rg, see what bin it fits in, increment count of hist bin
