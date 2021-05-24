#include <stdio.h>
int i;
double x[5] = {0, 31, 63, 95, 127};
double y[3] = {0, 31, 63};
int X[5], Y[3];
int main(void) {
	for(i = 0; i < 5; i++){
		X[i] = (int)(x[i] / 0.03629 + 161);
	}
	for(i = 0; i < 5; i++){
		Y[i] = (int)(y[i] / 0.020725 + 500);
	}
	for(i = 0; i < 8; i++){
		printf("else if (%d <= x && x <= %d && %d <= y && y <= %d){\n", X[i%4]+1, X[(i)%4+1], Y[i/4], Y[i/4+1]);
	}
	return 0;
    //X = (x - 161) * 0.03629;
	//Y = ((y - 500) * 0.020725);
}