
#include <windows.h>
#include "FileHelper.h"

#include "feature.h"



int rotate(char* data, int height, int width, int bytes,char * dst) {

	char* s = data + (height * width - width) * bytes;
	char* d = data;

	char* tmp = new char[width * bytes];
	while(s > d)
	{
		memcpy(tmp, d, width * bytes);
		memcpy(d, s, width * bytes);
		memcpy(s, tmp, width * bytes);
		s -= width * bytes;
		d += width * bytes;

	}

	//memcpy(dst, data, width * bytes * height);

	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j ++) {
			double y = i;
			double x = j;

			int newx = 0.966 * x - 0.2588 * y;
			int newy = 0.2588 * x + 0.966 * y;
			int dstpos = (newx * height+ newy)*bytes;
			int srcpos = (i * height + j) * bytes ;

			if (dstpos < 0)
			{
				//continue;
				dstpos = 0;
			}
			
			if (srcpos > height*width*bytes || dstpos > height * width * bytes)
			{
				break;
			}
			if (*(dst + dstpos) != 0) {
				memcpy(dst + dstpos, data + srcpos, bytes);
			}
		}
	}
	


	s = dst + (height * width - width) * bytes;
	d = dst;
	while (s > d)
	{
		
		memcpy(tmp, d, width * bytes);
		memcpy(d, s, width * bytes);
		memcpy(s, tmp, width * bytes);
		s -= width * bytes;
		d += width * bytes;
	}
	return 0;
}


int frame(char* data, int height, int width,int ll,int hh) {


	char* dst = new char[width * height];

	char* s = data + height * width - width;
	char* d = dst;
	for (int i = 0; i < height; i++)
	{
		memcpy(d, s, width);
		s -= width;
		d += width;
	}



	int top = 20;

	int p = (width - ll) / 2;

	char* lp = dst + top*width + p;

	for (int i = 0; i < ll; i++)
	{
		lp[i] = 255;
	}


	for (int i = 0; i < hh; i++)
	{
		lp[i*width] = 255;
	}


	for (int i = 0; i < hh; i++)
	{
		lp[i * width + ll] = 255;
	}


	lp = dst + top * width + p + hh*width;

	for (int i = 0; i < ll; i++)
	{
		lp[i] = 255;
	}

	char* tmp = new char[width];

	s = dst + height * width - width;
	d = dst;
	while (s > d)
	{
		
		memcpy(tmp, d, width);
		memcpy(d, s, width);
		memcpy(s, tmp, width);
		s -= width;
		d += width;
	}

	memcpy(data, dst, width * height);
	delete[]dst;
	return 0;

}

int reverseBmp(char* data, int height, int width) {

	char * dst = new char[width * height];

	char* s = data + height * width - width;
	char* p = dst;
	for (int i = 0;i < height;i ++)
	{
		memcpy(p, s, width);
		s -= width;
		p += width;
	}
	
	p = dst + 50 * width;
	s = dst;
	for (int i = 0;i < 50;i ++)
	{
		memcpy(p, s, width);
		s += width;
		p += width;
	}

	s = dst + height * width - width;
	p = dst;
	char* tmp = new char[width];
	while(s > p)
	{

		memcpy(tmp, p, width);
		memcpy(p, s, width);
		memcpy(s, tmp, width);
		s -= width;
		p += width;
	}

	memcpy(data, dst, width * height);
	delete[]dst;
	return 0;

}

void gamma(char* data) {
	RGBQUAD* q = (RGBQUAD*)(data);
	for (int i = 0; i < 256; i++)
	{
		q[i].rgbBlue = i;
		q[i].rgbGreen = i;
		q[i].rgbRed = i;
		q[i].rgbReserved = 0;
	}
}

void setColorTable_reverse(char* data) {
	RGBQUAD* q = (RGBQUAD*)(data);
	for (int i = 0; i < 256; i++)
	{
		q[i].rgbBlue = 256 - i;
		q[i].rgbGreen = 256 - i;
		q[i].rgbRed = 256 - i;
		q[i].rgbReserved = 0;
	}
}


void setColorTable(char* data) {
	RGBQUAD* q = (RGBQUAD*)(data);
	for (int i = 0; i < 256; i++)
	{
		q[i].rgbBlue = i;
		q[i].rgbGreen =  i;
		q[i].rgbRed =  i;
		q[i].rgbReserved = 0;
	}
}


void synthesize(char * s,char * d,int height,int width,int bpp) {
	int ret = 0;

	double alpha = 0.3;

// 	int size = width * height * bpp;
// 	for (int p = 0;p < size;p ++)
// 	{
// 		d[p] = d[p] * alpha + s[p] * (1 - alpha);
// 	}
// 	return;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			DWORD v = 0;
			DWORD v2 = 0;
			for (int k = 0; k < bpp; k++)
			{
				int pos = (i * height + j) * bpp + k;
// 				*((char*)&v + k) = s[pos];
// 				*((char*)&v2 + k) = s[pos];
				
				d[pos] = d[pos] * alpha + s[pos] * (1 - alpha);
			}

		}
	}
}

int mainproc() {
	char* sfile = 0;
	int sfs = 0;
	int ret = 0;

	ret = FileHelper::fileReader("water.bmp", &sfile, &sfs);

	BITMAPFILEHEADER* sbf = (BITMAPFILEHEADER*)sfile;

	BITMAPINFOHEADER* sbi = (BITMAPINFOHEADER*)(sfile + sizeof(BITMAPFILEHEADER));
	
	char* file = 0;
	int fs = 0;
	ret = FileHelper::fileReader("mandrill.bmp", &file, &fs);
	if (ret)
	{
		BITMAPFILEHEADER* bf = (BITMAPFILEHEADER*)file;
		if (bf->bfType == 'MB')
		{

			BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)(file + sizeof(BITMAPFILEHEADER));

			synthesize((char*)bf + bf->bfOffBits, (char*)sbf + sbf->bfOffBits,
				bi->biHeight, bi->biWidth,bi->biBitCount/8);
			ret = FileHelper::fileWriter("mandrill_new_synthesize.bmp", (char*)sbf, sbf->bfSize, TRUE);

			int bs = bf->bfSize;
			
			int cnt = bi->biSizeImage;

			char* nd = new char[fs];
			memset(nd, 0, fs);
			int nd_size = 0;
			BITMAPFILEHEADER* nd_bf = (BITMAPFILEHEADER*)nd;
			BITMAPINFOHEADER* nd_bi = (BITMAPINFOHEADER*)(nd + sizeof(BITMAPFILEHEADER));
			memcpy(nd, file, sizeof(BITMAPFILEHEADER));
			memcpy(nd + sizeof(BITMAPFILEHEADER), bi, bi->biSize);
			nd_size = nd_size + bf->bfOffBits+1024;

			char* pixel = file + bf->bfOffBits ;

			//setColorTable_reverse(nd + bf->bfOffBits);
			setColorTable(nd + bf->bfOffBits);

			for (int i = 0;i < bi->biHeight;i++ )
			{
				unsigned char* line = (unsigned char* )pixel + i * bi->biWidth*( bi->biBitCount/8);
				for (int j = 0;j < bi->biWidth;j++)
				{
					if (bi->biBitCount == 8)
					{

					}
					else if (bi->biBitCount == 16)
					{

					}
					else if (bi->biBitCount == 24)
					{
						ULONG v = line[j*3] + line[j*3 + 1] + line[j * 3 + 2];
						nd[nd_size++] = v/3;


					}
					else if (bi->biBitCount == 32)
					{
						ULONG v = line[j * 4] + line[j * 4 + 1] + line[j * 4 + 2] + line[j * 4 + 3];
						nd[nd_size++] =v/4;


					}
				}
			}

			nd_bi->biBitCount = 8;
			nd_bi->biSizeImage = bi->biHeight*bi->biWidth;
			nd_bf->bfSize = nd_size;
			nd_bf->bfOffBits += 1024;

			ret = FileHelper::fileWriter("mandrill_new.bmp", nd, nd_bf->bfSize,TRUE);


			char* feature = new char[nd_bf->bfSize];
			memcpy(feature, nd, nd_bf->bfSize);
			convolution(( char*)feature + nd_bf->bfOffBits, nd_bi->biHeight, nd_bi->biWidth);
			bmpFeature(( char*)feature + nd_bf->bfOffBits, nd_bi->biHeight, nd_bi->biWidth);
			ret = FileHelper::fileWriter("mandrill_new_5.bmp", feature, nd_bf->bfSize, TRUE);


			char* framedata = new char[nd_bf->bfSize + 1024];
			memcpy(framedata, nd, nd_bf->bfSize);
			reverseBmp(nd + nd_bf->bfOffBits, nd_bi->biHeight, nd_bi->biWidth);
			ret = FileHelper::fileWriter("mandrill_new_2.bmp", nd, nd_bf->bfSize, TRUE);


			frame(framedata + nd_bf->bfOffBits, nd_bi->biHeight, nd_bi->biWidth,100,50);
			ret = FileHelper::fileWriter("mandrill_new_3.bmp", framedata, nd_bf->bfSize, TRUE);


			char* rotatesrc = new char[bf->bfSize + 1024];
			char* rotatedata = new char[bf->bfSize + 1024];
			memcpy(rotatesrc, file, bf->bfSize);
			rotate(rotatesrc + bf->bfOffBits, bi->biHeight, bi->biWidth,bi->biBitCount/8, rotatedata);
			memcpy(rotatesrc + bf->bfOffBits, rotatedata, bi->biSizeImage);
			ret = FileHelper::fileWriter("mandrill_new_4.bmp", rotatesrc, bf->bfSize, TRUE);
		}
	}

	return 0;
}




int main() {

	int ret = 0;

	ret = mainproc();

	return 0;
}