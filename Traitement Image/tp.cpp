#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>

#include "ImageBase.h"

using namespace std;


string histogramme(char * nomFichier ) {
	ImageBase img;
	img.load(nomFichier);

	char nomFichier2[256];
	strcpy(nomFichier2, nomFichier);
	strcat(nomFichier2, ".dat");
	ofstream out(nomFichier2);

cout << nomFichier << " ---- " << nomFichier2 << endl;

	if( strcmp(nomFichier + strlen(nomFichier) - 3, "pgm") == 0 ) {
		// GREY
		int h[256] = {0};
		
		for(int x = 0; x < img.getHeight(); x++) {
			for(int y = 0; y < img.getWidth(); y++) {
				h[ img[x][y] ]++;
			}
		}

		for(int i = 0; i < 256; i++) {
			out << h[i] << endl;
		}
		// ******
	} else if( strcmp(nomFichier + strlen(nomFichier) - 3, "ppm") == 0 ) {
		// COLOR
		ImageBase * imgR = img.getPlan( ImageBase::PLAN_R );
		ImageBase * imgG = img.getPlan( ImageBase::PLAN_G );
		ImageBase * imgB = img.getPlan( ImageBase::PLAN_B );

		int r[256] = {0};
		int g[256] = {0};
		int b[256] = {0};

		for(int x = 0; x < img.getHeight(); x++) {
			for(int y = 0; y < img.getWidth(); y++) {
				r[ img[x*3][y*3+0] ]++;
				g[ img[x*3][y*3+1] ]++;
				b[ img[x*3][y*3+2] ]++;
			}
		}

		for(int i = 0; i < 256; i++) {
			out << i << " " << r[i] << " " << g[i] << " " << b[i] << endl;
		}
		// ******
	} else {
		// ERROR
		cout << "Can't load this file" << endl;
		strcpy(nomFichier2, "O");
	}
	
	out.close();
	string nomF = nomFichier2;
cout << "histo name : " << nomF << endl;
	return nomF;
}


void expansion( char * imgIn, char * imgOut ) {
cout << "expansion" << endl;
	string histoPath = histogramme(imgIn);
cout << "img : " << imgIn << " -> " << imgOut << endl;
	if(histoPath.length() < 2) { return; }

	ifstream file(histoPath.c_str());

	cout << "histo : " << histoPath << endl;

	if( strcmp(imgIn + strlen(imgIn) - 3, "pgm") == 0 ) {
		// GREY
		int a0 = 255, a1 = 0, amin = 0, amax = 255;
		bool b_a1 = false, b_a0 = true;
	
		int n;
		// Calcul A0 et A1
		int k = 0;
		while(!file.eof()) {
			file >> n;
			if( b_a0 && n != 0) {
				a0 = k;
				b_a0 = false;
				b_a1 = true;
			} else if( b_a1 && n == 0) {
				a1 = k-1;
				b_a1 = false;
			} else if( n != 0 ) {
				b_a1 = true;
			}
			k++;
		}
		// ******

		float alpha = 0, beta = 0;
		//alpha = (amin * a1 + amax * a0 ) / (a1 - a0) * -1;
		alpha = - a0;
		beta = (amax - amin ) / (a1 - a0);
		cout << "\tAlpha : " << alpha << endl << "\tBeta : " << beta << endl;
		cout << "\tA0 : " << a0 << endl << "\tA1 : " << a1 << endl;

		ImageBase img;
		img.load(imgIn);
		
		ImageBase IMGout(img.getWidth(), img.getHeight(), img.getColor());

		for(int x = 0; x < img.getHeight(); ++x) {
			for(int y = 0; y < img.getWidth(); ++y)
			{
				IMGout[x][y] = beta * (img[x][y] + alpha);
			}
		}

		IMGout.save(imgOut);

		histogramme(imgOut);
		// ******
	} else if( strcmp(imgIn + strlen(imgIn) - 3, "ppm") == 0 ) {
		// COLOR
		int amin = 0, amax = 255;

		int ra0 = 255, ra1 = 0;
		bool rb_a1 = false, rb_a0 = true;
		int ga0 = 255, ga1 = 0;
		bool gb_a1 = false, gb_a0 = true;
		int ba0 = 255, ba1 = 0;
		bool bb_a1 = false, bb_a0 = true;

		int rn, gn, bn, x;
		int k = 0;
		while(!file.eof()) {
			file >> x >> rn >> gn >> bn;

			if( rb_a0 && rn != 0) {
				ra0 = k;
				rb_a0 = false;
				rb_a1 = true;
			} else if( rb_a1 && rn == 0) {
				ra1 = k-1;
				rb_a1 = false;
			} else if( rn != 0 ) {
				rb_a1 = true;
			}

			if( gb_a0 && gn != 0) {
				ga0 = k;
				gb_a0 = false;
				gb_a1 = true;
			} else if( gb_a1 && gn == 0) {
				ga1 = k-1;
				gb_a1 = false;
			} else if( gn != 0 ) {
				gb_a1 = true;
			}

			if( bb_a0 && bn != 0) {
				ba0 = k;
				bb_a0 = false;
				bb_a1 = true;
			} else if( bb_a1 && bn == 0) {
				ba1 = k-1;
				bb_a1 = false;
			} else if( bn != 0 ) {
				bb_a1 = true;
			}

			k++;
		}
	
		float ralpha = 0, rbeta = 0;
	
		//ralpha = ((float)amin * (float)ra1 + (float)amax * (float)ra0 ) / ((float)ra1 - (float)ra0) * -1.0f;
		rbeta = ((float)amax - (float)amin ) / ((float)ra1 - (float)ra0);
		
		ralpha = - ra0;

		float galpha = 0, gbeta = 0;

		//galpha = ((float)amin * (float)ga1 + (float)amax * (float)ga0 ) / ((float)ga1 - (float)ga0) * -1.0f;
		gbeta = ((float)amax - (float)amin ) / ((float)ga1 - (float)ga0);
		galpha = - ga0;

		float balpha = 0, bbeta = 0;

		//balpha = ((float)amin * (float)ba1 + (float)amax * (float)ba0 ) / ((float)ba1 - (float)ba0) * -1.0f;
		bbeta = ((float)amax - (float)amin ) / ((float)ba1 - (float)ba0);
		balpha = - ba0;


		cout << "ra0: " << ra0 << " -- rb1: " << ra1 << endl;
		cout << "ga0: " << ga0 << " -- gb1: " << ga1 << endl;
		cout << "ba0: " << ba0 << " -- bb1: " << ba1 << endl;



		cout << "ra: " << ralpha << " -- rb: " << rbeta << endl;
		cout << "ga: " << galpha << " -- gb: " << gbeta << endl;
		cout << "ba: " << balpha << " -- bb: " << bbeta << endl;


		ImageBase img;
		img.load(imgIn);
		
		ImageBase IMGout(img.getWidth(), img.getHeight(), img.getColor());

		// COLOR EN RGB

		for(int x = 0; x < img.getHeight(); ++x) {
			for(int y = 0; y < img.getWidth(); ++y) {
				IMGout[x*3][y*3 + 0] = (char)(rbeta * ((float)img[x*3][y*3+0] + ralpha) );
				IMGout[x*3][y*3 + 1] = (char)(gbeta * ((float)img[x*3][y*3+1] + galpha) );
				IMGout[x*3][y*3 + 2] = (char)(bbeta * ((float)img[x*3][y*3+2] + balpha) );

			}
		}

		IMGout.save(imgOut);

		histogramme(imgOut);

		// ******
	} else {
		// ERROR
		cout << "Can't load this file !" << endl;
	}
}

void seuilExtrema( char * imgPath, char * outPath )
{
	string ss = histogramme(imgPath);
	ifstream file( ss.c_str());

	if( strcmp(imgPath + strlen(imgPath) - 3, "pgm") == 0 ) {

		int n, nbPixel = 0;
		while(!file.eof()) {
			file >> n;
			nbPixel += n;
		}
	
		int nbMin = nbPixel * 0.25, nbMax = nbPixel * 0.75;
		file.seekg(0);

		int Smin = 0, Smax = 255;
		int k = 0;
		nbPixel = 0;
		while(!file.eof()) {
			file >> n;
			nbPixel += n;
			if(nbPixel < nbMin) {
				Smin = k;
			}
				if(nbPixel < nbMax) {
				Smax = k;
			}
	
			k++;
		}
	
		ImageBase img;
		img.load( imgPath );
		
		ImageBase res(img.getWidth(), img.getHeight(), img.getColor());
	
		for(int x = 0; x < img.getHeight(); ++x) {
			for(int y = 0; y < img.getWidth(); ++y) {
				if(img[x][y] < Smin) { res[x][y] = Smin; }
				if(img[x][y] > Smax) { res[x][y] = Smax; }
			}
		}

		res.save( outPath);

	} else if( strcmp(imgPath + strlen(imgPath) - 3, "ppm") == 0 ) {

		int n, r, g, b, nbPixelR = 0, nbPixelG = 0, nbPixelB = 0;
		int tr[256] = {0};
		int tg[256] = {0};
		int tb[256] = {0};
		while(!file.eof()) {
			file >> n >> r >> g >> b;
			nbPixelR += r;
			nbPixelG += g;
			nbPixelB += b;
			tr[n] = r;
			tg[n] = g;
			tb[n] = b;
		}

		int nbMinR = nbPixelR * 0.2, nbMaxR = nbPixelR * 0.8;
		int nbMinG = nbPixelG * 0.2, nbMaxG = nbPixelG * 0.8;
		int nbMinB = nbPixelB * 0.2, nbMaxB = nbPixelB * 0.8;

		//file.seekg(0);

		int SminR = 0, SmaxR = 255;
		int SminG = 0, SmaxG = 255;
		int SminB = 0, SmaxB = 255;
		int k = 0;
		nbPixelR = 0;
		nbPixelG = 0;
		nbPixelB = 0;

		for(int i = 0; i < 256; i++) {
			nbPixelR += tr[i];
			nbPixelG += tg[i];
			nbPixelB += tb[i];

			if(nbPixelR < nbMinR) {
				SminR = i;
			}
			if(nbPixelR < nbMaxR) {
				SmaxR = i;
			}
	
			if(nbPixelG < nbMinG) {
				SminG = i;
			}
			if(nbPixelG < nbMaxG) {
				SmaxG = i;
			}

			if(nbPixelB < nbMinB) {
				SminB = i;
			}
			if(nbPixelB < nbMaxB) {
				SmaxB = i;
			}
		}

cout << "1" << endl;
	
		ImageBase img;
		img.load( imgPath );

		ImageBase res(img.getWidth(), img.getHeight(), img.getColor());
	
		for(int x = 0; x < img.getHeight(); ++x) {
			for(int y = 0; y < img.getWidth(); ++y) {
				res[x*3][y*3+0] = img[x*3][y*3+0];
				res[x*3][y*3+1] = img[x*3][y*3+1];
				res[x*3][y*3+2] = img[x*3][y*3+2];

				if(img[x*3][y*3+0] < SminR) { res[x*3][y*3+0] = SminR; }
				if(img[x*3][y*3+0] > SmaxR) { res[x*3][y*3+0] = SmaxR; }

				if(img[x*3][y*3+1] < SminG) { res[x*3][y*3+1] = SminG; }
				if(img[x*3][y*3+1] > SmaxG) { res[x*3][y*3+1] = SmaxG; }

				if(img[x*3][y*3+2] < SminB) { res[x*3][y*3+2] = SminB; }
				if(img[x*3][y*3+2] > SmaxB) { res[x*3][y*3+2] = SmaxB; }
			}
		}

		res.save( outPath );

	} else {
		cout << "ni pgm ni ppm" << endl;
	}

	histogramme(outPath);
}


char * ddp( char * imgIn, char * ddpName )
{
	string histo = histogramme( imgIn );
//	char * ddpName = malloc(255 * sizeof(char) );
//	strcpy(ddpName, imgIn);
//	strcat(ddpName, "_ddp.dat");

	ifstream fileHisto( histo.c_str() );
	ofstream fileddp( ddpName );

	int n;

	ImageBase img;
	img.load( imgIn );

	fileHisto >> n;
	do {
		fileddp << ((double)n / (double)( img.getWidth() * img.getHeight()) ) << endl;
		fileHisto >> n;
	} while(!fileHisto.eof());

	fileHisto.close();
	fileddp.close();

	return ddpName;
}

void F( char * img, char * fname )
{
	char fame[255];
	strcpy(fame, img);
	strcat(fame, "_ddp.dat");
	char * ddpName( ddp(img, fame));
	
	ifstream file( ddpName );
	ofstream out( fname );
	double f[256] = {0};
	int k = 1;
	file >> f[0];
	out << f[0] << endl;

	file >> f[k];

	do {
		f[k] += f[k-1];
		out << f[k] << endl;
		k++;
		
		file >> f[k];
	} while( ! file.eof() ); 

	file.close();
	out.close();

	//return f;
}

void egalisation(char * imgIn, char * imgOut)
{
	char ffile[256];
	strcpy(ffile, imgIn);
	strcat(ffile, "_f.dat");

	F(imgIn, ffile);

	ifstream file(ffile);
	double f[256];
	int k = 0;
	while( !file.eof()) {
		file >> f[k];
		k++;
	}
	
	file.close();

	for(int i=0;i<256;i++) {
		f[i] *= 255;
	}

	ImageBase img;
	img.load(imgIn);
	ImageBase imgEgal(img.getWidth(), img.getHeight(), img.getColor());

	for(int x=0;x<img.getHeight();x++) {
		for(int y=0;y<img.getWidth();y++) {
			imgEgal[x][y] = (int)f[ img[x][y] ];
		}
	}

	imgEgal.save(imgOut);

	histogramme(imgOut);
}

void Ti(char * imgIn, char * imgOut, char * r)
{
	ifstream file(r);
	double f[256];
	int k = 0;
	while( !file.eof()) {
		file >> f[k];
		k++;
	}
	
	file.close();

	for(int i=0;i<256;i++) {
		//f[i] *= 255;
	}

	ImageBase img;
	img.load(imgIn);
	ImageBase imgEgal(img.getWidth(), img.getHeight(), img.getColor());

	for(int x=0;x<img.getHeight();x++) {
		for(int y=0;y<img.getWidth();y++) {
			imgEgal[x][y] = (int)f[ img[x][y] ];
		}
	}

	imgEgal.save(imgOut);
}

void inverse(char * fn, char * fni) 
{
	ifstream file(fn);
	double f[256];
	int f2[256];
	double fi[256];
	int k = 0;
	while( !file.eof()) {
		file >> f[k];
		k++;
	}
	file.close();

	for(int i=0; i<256; i++) {
		fi[ i ] = 0;
		f2[i] = 255 * f[i];
	}

	for(int i=0; i<256; i++) {
		fi[ (int)f2[i] ] = i;
cout << f2[i] << endl;
	}
	
	ofstream o(fni);
	for(int i=0; i<256; i++) {
		o << fi[i] << endl;
	}

	o.close();

}

void specification(char * imgIn, char * imgRef, char * imgOut)
{
	char st[] = "egal.pgm";
	egalisation(imgIn, st);
	char ffile[256];
	strcpy(ffile, imgRef);
	strcat(ffile, "_f.dat");
	char fifile[256];
	strcpy(fifile, imgRef);
	strcat(fifile, "_finv.dat");

	F(imgRef, ffile);
	inverse(ffile, fifile);
	Ti(st, imgOut, fifile);
	histogramme(imgOut);
}


void seuil(char * cNomImgLue, char * cNomImgEcrite, vector<int> S)
{
	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

	for(int x = 0; x < imIn.getHeight(); ++x)
		for(int y = 0; y < imIn.getWidth(); ++y)
		{
			bool none = true;
			for(int i=0; i<S.size();i++) {
				if (none && imIn[x][y] < S[i]) 
				{
					imOut[x][y] = (255 / S.size()) * i;
					none = false;
				}
			}

			if(none) { imOut[x][y] = 255; }
		}
		
	imOut.save(cNomImgEcrite);
}


void dilatation(char * imgIn, char * imgOut, int force)
{
	ImageBase imIn;
	imIn.load(imgIn);

	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());


	if( strcmp(imgIn + strlen(imgIn) - 3, "pgm") == 0 ) {

		for(int x = 0; x < imIn.getHeight(); ++x) {
			for(int y = 0; y < imIn.getWidth(); ++y) {
				imOut[x][y] = imIn[x][y];
			}
		}

		for(int i=0;i<force;i++) {
			for(int x = 0; x < imIn.getHeight(); ++x) {
				for(int y = 0; y < imIn.getWidth(); ++y) {
					int min = imIn[x][y];
					if(x > 0 && min > imIn[x-1][y]) { min = imIn[x-1][y]; }
					if(x < imOut.getWidth() - 1 && min > imIn[x+1][y]) { min = imIn[x+1][y]; }
					if(y > 0 && min > imIn[x][y-1]) { min = imIn[x][y-1]; }
					if(y < imOut.getHeight() - 1 && min > imIn[x][y+1]) { min = imIn[x][y+1]; }
					imOut[x][y] = min;
					/*
					if(x > 0 && imIn[x][y] > imOut[x-1][y]) { imOut[x-1][y] = imIn[x][y]; }
					if(x < imOut.getWidth() - 1 && imIn[x][y] > imOut[x+1][y]) { imOut[x+1][y] = imIn[x][y]; }
					if(y > 0 && imOut[x][y] > imIn[x][y-1]) { imOut[x][y-1] = imIn[x][y]; }
					if(y < imOut.getHeight() - 1 && imIn[x][y] > imOut[x][y+1]) { imOut[x][y+1] = imIn[x][y]; }
					*/
				}
			}
			imOut.save(imgOut);
			imIn.load(imgOut);
		}

		imOut.save(imgOut);

	} else if( strcmp(imgIn + strlen(imgIn) - 3, "ppm") == 0 ) {

		for(int x = 0; x < imIn.getHeight(); ++x) {
			for(int y = 0; y < imIn.getWidth(); ++y) {
				imOut[x*3][y*3+0] = imIn[x*3][y*3+0];
				imOut[x*3][y*3+1] = imIn[x*3][y*3+1];
				imOut[x*3][y*3+2] = imIn[x*3][y*3+2];
			}
		}

		for(int i=0;i<force;i++) {
			for(int x = 0; x < imIn.getHeight(); ++x) {
				for(int y = 0; y < imIn.getWidth(); ++y) {
					int minR = imIn[x*3][y*3+0];
					if(x > 0 && minR > imIn[(x-1)*3][y*3+0]) { minR = imIn[(x-1)*3][y*3+0]; }
					if(x < imOut.getWidth() - 1 && minR > imIn[(x+1)*3][y*3+0]) { minR = imIn[(x+1)*3][y*3+0]; }
					if(y > 0 && minR > imIn[x*3][(y-1)*3+0]) { minR = imIn[x*3][(y-1)*3+0]; }
					if(y < imOut.getHeight() - 1 && minR > imIn[x*3][(y+1)*3+0]) { minR = imIn[x*3][(y+1)*3+0]; }
					imOut[x*3][y*3+0] = minR;

					int minG = imIn[x*3][y*3+1];
					if(x > 0 && minG > imIn[(x-1)*3][y*3+1]) { minG = imIn[(x-1)*3][y*3+1]; }
					if(x < imOut.getWidth() - 1 && minG > imIn[(x+1)*3][y*3+1]) { minG = imIn[(x+1)*3][y*3+1]; }
					if(y > 0 && minG > imIn[x*3][(y-1)*3+1]) { minG = imIn[x*3][(y-1)*3+1]; }
					if(y < imOut.getHeight() - 1 && minG > imIn[x*3][(y+1)*3+1]) { minG = imIn[x*3][(y+1)*3+1]; }
					imOut[x*3][y*3+1] = minG;
					
					int minB = imIn[x*3][y*3+2];
					if(x > 0 && minB > imIn[(x-1)*3][y*3+2]) { minB = imIn[(x-1)*3][y*3+2]; }
					if(x < imOut.getWidth() - 1 && minB > imIn[(x+1)*3][y*3+2]) { minB = imIn[(x+1)*3][y*3+2]; }
					if(y > 0 && minB > imIn[x*3][(y-1)*3+2]) { minB = imIn[x*3][(y-1)*3+2]; }
					if(y < imOut.getHeight() - 1 && minB > imIn[x*3][(y+1)*3+2]) { minB = imIn[x*3][(y+1)*3+2]; }
					imOut[x*3][y*3+2] = minB;

					/*
					if(x > 0 && imIn[x*3][y*3+0] > imOut[(x-1)*3][y*3+0]) { imOut[(x-1)*3][y*3+0] = imIn[x*3][y*3+0]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+0] > imOut[(x+1)*3][y*3+0]) { imOut[(x+1)*3][y*3] = imIn[x*3][y*3+0]; }
					if(y > 0 && imOut[x*3][y*3+0] > imIn[x*3][(y-1)*3+0]) { imOut[x*3][(y-1)*3+0] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+0] > imOut[x*3][(y+1)*3+0]) { imOut[x*3][(y+1)*3+0] = imIn[x*3][y*3+0]; }

					if(x > 0 && imIn[x*3][y*3+1] > imOut[(x-1)*3][y*3+1]) { imOut[(x-1)*3][y*3+1] = imOut[x*3][y*3+1]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+1] > imOut[(x+1)*3][y*3+1]) { imIn[(x+1)*3][y*3] = imIn[x*3][y*3+1]; }
					if(y > 0 && imOut[x*3][y*3+1] > imIn[x*3][(y-1)*3+1]) { imOut[x*3][(y-1)*3+1] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+1] > imOut[x*3][(y+1)*3+1]) { imOut[x*3][(y+1)*3+1] = imIn[x*3][y*3+1]; }

					if(x > 0 && imIn[x*3][y*3+2] > imOut[(x-1)*3][y*3+2]) { imOut[(x-1)*3][y*3+2] = imIn[x*3][y*3+2]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+2] > imOut[(x+1)*3][y*3+2]) { imOut[(x+1)*3][y*3] = imIn[x*3][y*3+2]; }
					if(y > 0 && imOut[x*3][y*3+2] > imIn[x*3][(y-1)*3+2]) { imOut[x*3][(y-1)*3+2] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+2] > imOut[x*3][(y+1)*3+2]) { imOut[x*3][(y+1)*3+2] = imIn[x*3][y*3+2]; }
					*/
				}
			}
			imOut.save(imgOut);
			imIn.load(imgOut);
		}

		imOut.save(imgOut);
	}
}

void erosion(char * imgIn, char * imgOut, int force)
{
	ImageBase imIn;
	imIn.load(imgIn);

	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

	if( strcmp(imgIn + strlen(imgIn) - 3, "pgm") == 0 ) {

		for(int x = 0; x < imIn.getHeight(); ++x) {
			for(int y = 0; y < imIn.getWidth(); ++y) {
				imOut[x][y] = imIn[x][y];
			}
		}

		for(int i=0;i<force;i++) {
			for(int x = 0; x < imIn.getHeight(); ++x) {
				for(int y = 0; y < imIn.getWidth(); ++y) {
					int max = imIn[x][y];
					if(x > 0 && max < imIn[x-1][y]) { max = imIn[x-1][y]; }
					if(x < imOut.getWidth() - 1 && max < imIn[x+1][y]) { max = imIn[x+1][y]; }
					if(y > 0 && max < imIn[x][y-1]) { max = imIn[x][y-1]; }
					if(y < imOut.getHeight() - 1 && max < imIn[x][y+1]) { max = imIn[x][y+1]; }
					imOut[x][y] = max;
					/*
					if(x > 0 && imIn[x][y] < imOut[x-1][y]) { imOut[x-1][y] = imIn[x][y]; }
					if(x < imOut.getWidth() - 1 && imIn[x][y] < imOut[x+1][y]) { imOut[x+1][y] = imIn[x][y]; }
					if(y > 0 && imOut[x][y] < imIn[x][y-1]) { imOut[x][y-1] = imIn[x][y]; }
					if(y < imOut.getHeight() - 1 && imIn[x][y] < imOut[x][y+1]) { imOut[x][y+1] = imIn[x][y]; }
					*/
				}
			}
			imOut.save(imgOut);
			imIn.load(imgOut);
		}

		imOut.save(imgOut);
	} else if( strcmp(imgIn + strlen(imgIn) - 3, "ppm") == 0 ) {
		
		for(int x = 0; x < imIn.getHeight(); ++x) {
			for(int y = 0; y < imIn.getWidth(); ++y) {
				imOut[x*3][y*3+0] = imIn[x*3][y*3+0];
				imOut[x*3][y*3+1] = imIn[x*3][y*3+1];
				imOut[x*3][y*3+2] = imIn[x*3][y*3+2];
			}
		}

		for(int i=0;i<force;i++) {
			for(int x = 0; x < imIn.getHeight(); ++x) {
				for(int y = 0; y < imIn.getWidth(); ++y) {
					int maxR = imIn[x*3][y*3+0];
					if(x > 0 && maxR < imIn[(x-1)*3][y*3+0]) { maxR = imIn[(x-1)*3][y*3+0]; }
					if(x < imOut.getWidth() - 1 && maxR < imIn[(x+1)*3][y*3+0]) { maxR = imIn[(x+1)*3][y*3+0]; }
					if(y > 0 && maxR < imIn[x*3][(y-1)*3+0]) { maxR = imIn[x*3][(y-1)*3+0]; }
					if(y < imOut.getHeight() - 1 && maxR < imIn[x*3][(y+1)*3+0]) { maxR = imIn[x*3][(y+1)*3+0]; }
					imOut[x*3][y*3+0] = maxR;

					int maxG = imIn[x*3][y*3+1];
					if(x > 0 && maxG < imIn[(x-1)*3][y*3+1]) { maxG = imIn[(x-1)*3][y*3+1]; }
					if(x < imOut.getWidth() - 1 && maxG < imIn[(x+1)*3][y*3+1]) { maxG = imIn[(x+1)*3][y*3+1]; }
					if(y > 0 && maxG < imIn[x*3][(y-1)*3+1]) { maxG = imIn[x*3][(y-1)*3+1]; }
					if(y < imOut.getHeight() - 1 && maxG < imIn[x*3][(y+1)*3+1]) { maxG = imIn[x*3][(y+1)*3+1]; }
					imOut[x*3][y*3+1] = maxG;
					
					int maxB = imIn[x*3][y*3+2];
					if(x > 0 && maxB < imIn[(x-1)*3][y*3+2]) { maxB = imIn[(x-1)*3][y*3+2]; }
					if(x < imOut.getWidth() - 1 && maxB < imIn[(x+1)*3][y*3+2]) { maxB = imIn[(x+1)*3][y*3+2]; }
					if(y > 0 && maxB < imIn[x*3][(y-1)*3+2]) { maxB = imIn[x*3][(y-1)*3+2]; }
					if(y < imOut.getHeight() - 1 && maxB < imIn[x*3][(y+1)*3+2]) { maxB = imIn[x*3][(y+1)*3+2]; }
					imOut[x*3][y*3+2] = maxB;

					/*
					if(x > 0 && imIn[x*3][y*3+0] < imOut[(x-1)*3][y*3+0]) { imOut[(x-1)*3][y*3+0] = imIn[x*3][y*3+0]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+0] < imOut[(x+1)*3][y*3+0]) { imOut[(x+1)*3][y*3] = imIn[x*3][y*3+0]; }
					if(y > 0 && imOut[x*3][y*3+0] < imIn[x*3][(y-1)*3+0]) { imOut[x*3][(y-1)*3+0] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+0] < imOut[x*3][(y+1)*3+0]) { imOut[x*3][(y+1)*3+0] = imIn[x*3][y*3+0]; }

					if(x > 0 && imIn[x*3][y*3+1] < imOut[(x-1)*3][y*3+1]) { imOut[(x-1)*3][y*3+1] = imIn[x*3][y*3+1]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+1] < imOut[(x+1)*3][y*3+1]) { imOut[(x+1)*3][y*3] = imIn[x*3][y*3+1]; }
					if(y > 0 && imOut[x*3][y*3+1] < imIn[x*3][(y-1)*3+1]) { imOut[x*3][(y-1)*3+1] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+1] < imOut[x*3][(y+1)*3+1]) { imOut[x*3][(y+1)*3+1] = imIn[x*3][y*3+1]; }

					if(x > 0 && imIn[x*3][y*3+2] < imOut[(x-1)*3][y*3+2]) { imOut[(x-1)*3][y*3+2] = imIn[x*3][y*3+2]; }
					if(x < imOut.getWidth() - 1 && imIn[x*3][y*3+2] < imOut[(x+1)*3][y*3+2]) { imOut[(x+1)*3][y*3] = imIn[x*3][y*3+2]; }
					if(y > 0 && imOut[x*3][y*3+2] < imIn[x*3][(y-1)*3+2]) { imOut[x*3][(y-1)*3+2] = imIn[x*3][y*3]; }
					if(y < imOut.getHeight() - 1 && imIn[x*3][y*3+2] < imOut[x*3][(y+1)*3+2]) { imOut[x*3][(y+1)*3+2] = imIn[x*3][y*3+2]; }
					*/
				}
			}
			imOut.save(imgOut);
			imIn.load(imgOut);
		}

		imOut.save(imgOut);
	}
}

void fermeture(char * img, char * res, int force)
{
	dilatation(img, res, force);
	erosion(res, res, force);
}

void ouverture(char * img, char * res, int force)
{
	erosion(img, res, force);
	dilatation(res, res, force);
}

void contour(char * img1, char * img2, char * imgR)
{
	ImageBase im1;
	im1.load(img1);

	ImageBase im2;
	im2.load(img2);

	ImageBase imOut(im1.getWidth(), im1.getHeight(), im1.getColor());

	for(int x = 0; x < im1.getHeight(); ++x) {
		for(int y = 0; y < im1.getWidth(); ++y) {
			if(im1[x][y] == im2[x][y]) { imOut[x][y] = 255; }
			else { imOut[x][y] = 0; }
		}
	}

	imOut.save(imgR);
}


void RGBtoGrey(char * nomRGB, char * nomGrey)
{
	ImageBase imgRGB;
	imgRGB.load(nomRGB);

	ImageBase imgGrey(imgRGB.getWidth(), imgRGB.getHeight(), ! imgRGB.getColor());

	for(int x=0;x<imgRGB.getHeight();x++) {
		for(int y=0;y<imgRGB.getWidth();y++) {
			imgGrey[x][y] = imgRGB[x*3][y*3+0] * 0.299 + imgRGB[x*3][y*3+1] * 0.587 + imgRGB[x*3][y*3+2] * 0.114;
		}
	}

	imgGrey.save(nomGrey);
}

double floute(ImageBase & img, int x, int y)
{
	double p = 0;
	double ratio = 0;
	if(y > 0) {
		if( x > 0 ) { p += img[x-3][y-3]; ratio += 1.0; }
		p += img[x][y-3]; ratio += 1.0;
		if(x < img.getHeight()*3 - 3) { p += img[x+3][y-3]; ratio += 1.0; }
	}

	if( x > 0 ) { p += img[x-3][y]; ratio += 1.0; }
	p += img[x][y]; ratio += 1.0;
	if(x < img.getHeight()*3 - 3) { p += img[x+3][y]; ratio += 1.0; }

	if(y < img.getWidth()*3 - 3) {
		if( x > 0 ) { p += img[x-3][y+3]; ratio += 1.0; }
		p += img[x][y+3]; ratio += 1.0;
		if(x < img.getHeight()*3 - 3) { p += img[x+3][y+3]; ratio += 1.0; }
	}

	return p / ratio;
}


void flouteIMG(char * n1, char * n2)
{
	ImageBase img1;
	img1.load(n1);

	ImageBase imgf(img1.getWidth(), img1.getHeight(), img1.getColor());


	for(int x=0;x<img1.getHeight();x++) {
		for(int y=0;y<img1.getWidth();y++) {
			imgf[x*3][y*3+0] = floute(img1, x*3, y*3+0);
			imgf[x*3][y*3+1] = floute(img1, x*3, y*3+1);
			imgf[x*3][y*3+2] = floute(img1, x*3, y*3+2);
		}
	}

	imgf.save(n2);

}

void flouteFond(char * n1, char * n2, char * nf)
{
	ImageBase imgba;
	imgba.load(n1);

	ImageBase imgbg;
	imgbg.load(n2);

	ImageBase imgf(imgbg.getWidth(), imgbg.getHeight(), imgba.getColor());


	for(int x=0;x<imgbg.getHeight();x++) {
		for(int y=0;y<imgbg.getWidth();y++) {
			if(imgbg[x][y] > 128) { // Si blanc
				imgf[x*3][y*3+0] = floute(imgba, x*3, y*3+0);
				imgf[x*3][y*3+1] = floute(imgba, x*3, y*3+1);
				imgf[x*3][y*3+2] = floute(imgba, x*3, y*3+2);
			} else {
				imgf[x*3][y*3+0] = imgba[x*3][y*3+0];
				imgf[x*3][y*3+1] = imgba[x*3][y*3+1];
				imgf[x*3][y*3+2] = imgba[x*3][y*3+2];
			}
		}
	}

	imgf.save(nf);

}



int main(int argc, char** argv)
{
	if(argc < 3) { std::cout << "Manque arg" << std::endl; return 0; }

	//expansion( argv[1], argv[2] );
	
	//seuilExtrema( argv[1], argv[2] );
	//histogramme( argv[1] );
	//expansion(argv[2], argv[3]);

	//specification(argv[1], argv[2], argv[3]);

	//egalisation( argv[1], argv[3] );

// SEUIL /////
/*
	char cNomImgLue[250], cNomImgEcrite[250];
	std::vector<int> S;
  
	if (argc < 4) 
	{
		printf("Usage: ImageIn.pgm ImageOut.pgm Seuil1 ... SeuilN \n"); 
		return 1;
	}
	sscanf(argv[1],"%s",cNomImgLue) ;
	sscanf(argv[2],"%s",cNomImgEcrite);

	for(int i=3; i < argc; i++) {
		int k = 0;
		sscanf (argv[i],"%d", &k);
		S.push_back(k);
	}

	seuil(cNomImgLue, cNomImgEcrite, S);
//////////////*/
	//fermeture(argv[1], argv[2], 1);
	//ouverture(argv[1], argv[2], 1);

	//dilatation(argv[1], argv[2], 1);
	//erosion(argv[1], argv[2], 1);
	//contour(argv[1], argv[2], argv[3]);

	//RGBtoGrey(argv[1], argv[2]);


//	flouteIMG(argv[1], argv[2]);



	string arg = argv[1];

	if(arg == "-h") { // Histogramme
		histogramme( argv[2] );
	} else if( arg == "-x" ) {
		seuilExtrema( argv[2], argv[3] );
	} else if( arg == "-p" ) {
		expansion( argv[2], argv[3] );
	} else if( arg == "-S" ) {
		specification( argv[2], argv[3], argv[4] );
	} else if( arg == "-g" ) {
		egalisation( argv[2], argv[3] );
	} else if( arg == "-s" ) {
		char cNomImgLue[250], cNomImgEcrite[250];
		std::vector<int> S;
  
		if (argc < 5) 
		{
			printf("Usage: ImageIn.pgm ImageOut.pgm Seuil1 ... SeuilN \n"); 
			return 1;
		}
		sscanf(argv[2],"%s",cNomImgLue) ;
		sscanf(argv[3],"%s",cNomImgEcrite);

		for(int i=4; i < argc; i++) {
			int k = 0;
			sscanf (argv[i],"%d", &k);
			S.push_back(k);
		}
	
		seuil(cNomImgLue, cNomImgEcrite, S);

	} else if( arg == "-f" ) {
		if( argc > 4 ) {
			fermeture( argv[3], argv[4], atoi(argv[2]) );
		} else {
			fermeture( argv[2], argv[3], 1 );
		}
	} else if( arg == "-o" ) {
		if( argc > 4 ) {
			ouverture( argv[3], argv[4], atoi(argv[2]) );
		} else {
			ouverture( argv[2], argv[3], 1 );
		}
	} else if( arg == "-d" ) {
		if(argc > 4) {
			dilatation( argv[3], argv[4], atoi(argv[2]) );
		} else {
			dilatation( argv[2], argv[3], 1);
		}
	} else if( arg == "-e" ) {
		if(argc > 4) {
			erosion( argv[3], argv[4], atoi(argv[2]) );
		} else {
			erosion( argv[2], argv[3], 1);
		}
	} else if( arg == "-c" ) {
		contour( argv[2], argv[3], argv[4] );
	} else if( arg == "-t" ) {
		RGBtoGrey( argv[2], argv[3] );
	} else if( arg == "-F" ) {
		if( argc > 4 ) {
				flouteIMG( argv[3], argv[4] );
			for( int i = 1; i < atoi(argv[2]); i++ ) {
				flouteIMG( argv[4], argv[4] );
			}
		} else {
			flouteIMG( argv[2], argv[3] );
		}
	} else if( arg == "-Ff" ) {
		if( argc > 5 ) {
				flouteFond( argv[3], argv[4], argv[5] );
			for(int i = 0; i < atoi(argv[2]); i++) {
				flouteFond( argv[5], argv[4], argv[5] );
			}
		} else {
			flouteFond( argv[2], argv[3], argv[4] );
		}
	} else {
		cout << "Bad argument." << endl;
	}



	return 0;
}



