//code for rgb to chromaticity shift

float rConvert(int red, int green, int blue){
	return red/(red+green+blue);
}

float gConvert(int red, int green, int blue){
	return green/(red+green+blue);
}
