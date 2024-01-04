

#include "feature.h"

#include <math.h>

#include <stdio.h>

#include <Windows.h>

#define _USE_MATH_DEFINES

#define FILTER_DIMENSION 5
double g_filter[FILTER_DIMENSION][FILTER_DIMENSION] = { 1,4,6,4,1,4,6,24,16,4,6,24,36,24,6,4,6,24,16,4,1,4,6,4,1 };


//#define FILTER_DIMENSION 3
//double g_filter[FILTER_DIMENSION][FILTER_DIMENSION] = { 1,-2,1,-2,4,-2,1,-2};


int convolution( char* data, int h, int w) {
	char* newdata = new  char[w * (h + FILTER_DIMENSION)];
	memset(newdata, 0, w * h);

	for (int i = 0; i < h - FILTER_DIMENSION + 1;i ++) {
		for (int j = 0; j < w - FILTER_DIMENSION + 1; j++) {
			
			int p = i * w + j;
			double t = 0;

			for (int k = 0; k < FILTER_DIMENSION; k++) {

				for (int m = 0; m < FILTER_DIMENSION; m++) {
					
					t += data[p + k * w + m] * g_filter[k][m];
				}
			}
			newdata[p] = t/256;
		}
	}

	memcpy(data, newdata, w * h);

	delete[]newdata;

	return 0;
}


int bmpFeature(char* data, int h, int w) {
	char* newdata = new char[w * h];
	memset(newdata, 0, w * h);

	char* td1 = new char[w * h];
	memset(td1, 0, w * h);
	char* td2 = new char[w * h];
	memset(td2, 0, w * h);

	for (int i = 0; i < h - 1; i++) {
		for (int j = 0; j < w - 1; j++) {
			int p = i * w + j;
			int p1 = +1;
			int p2 = p + w;
			td1[p] = data[p1] - data[p];
			td2[p] = data[p2] - data[p];
		}
	}

	double pi = acos(-1);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int p = i * w + j;

			double t1 = ((td1[p]) ^ 2 + (td2[p]) ^ 2);
			double c1 = (t1 - 2) * exp(-t1 / 2) / (2 * pi);

			newdata[p] = c1;
		}
	}

	memcpy(data, newdata, w * h);

	delete[]newdata;

	delete []td1;
	delete []td2;

	return 0;
}



int bmpFeature1( char * data,int h,int w) {
	char* newdata = new char[w * h];
	memset(newdata, 0, w * h);

	double pi = acos(-1);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int x = i * w + j;
			int y = x + w;
			if (y >= h*w) {
				y = x;
			}

			double t1 = ( (data[x]) ^ 2 + (data[y]) ^ 2);
			double c1 =  (t1-2)* exp(-t1/2)/(2*pi);

			y = x + 1;
			if (x % w == 0) {
				//y = x;
			}
			double t2 = ((data[x]) ^ 2 + (data[y]) ^ 2);
			double c2 = (t2 - 2) * exp(-t2 / 2) / (2 * pi);

			//newdata[x] =  256 - (c1 + c2)/2;

			//newdata[x] = 256 - c1;

			newdata[x] = 256 - ( c2) ;
		}
	}

	memcpy(data, newdata, w * h);

	delete []newdata;

	return 0;
}