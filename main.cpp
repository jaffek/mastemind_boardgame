#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <cstdlib>
#include "pionek.h"

using namespace cv;
using namespace std;

//*********************************************************************************************
int doit = 0;
int ile = 0;
int lowV = 160;
int lowVP = 0; 
int pom = 0;
char charCheckForEscKey = 0;
char czekaj = 0;
int proba = 0;
int proba1 = -1;
int numer = 0;
int numer1 = -1;
int rzad;
int nowa = 1;
int zaz_pon = 0;
int pokaz = 0;
Mat imgOrigin;
Mat imgOriginal;
Mat imgOriginal1;
Mat imgOrigintmp;
Mat threshImg;
Mat tmp;
Mat im_with_keypoints;
Mat background(800, 800, CV_8UC3, Scalar(0, 0, 0));
Mat pombg(800, 800, CV_8UC3, Scalar(0, 0, 0));
Mat3b okienko;
Pionek **pionki;
Pionek szyfr[4];
Podp **key;
Rect r;
vector<KeyPoint> keypoints;
//*********************************************************************************************
void imgtransf()
{
	int lowH = 0;      
	int highH = 179;
	int lowS = 0;       
	int highS = 255;
	int highV = 255;
	Mat hsvImg;
	cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);     
	inRange(hsvImg, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), threshImg);
	GaussianBlur(threshImg, threshImg, Size(3, 3), 0);  
	int dilate_size = 6;
	Mat element = getStructuringElement(MORPH_ELLIPSE,Size(2 * dilate_size + 1, 2 * dilate_size + 1),Point(dilate_size, dilate_size));
	dilate(threshImg, threshImg, element);        
	int erosion_size = 3;
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
	erode(threshImg, threshImg, element1);
    threshold(threshImg, threshImg, 230, 255, THRESH_BINARY);
}
//----------------------------------------------------------------------------------------------
void blobcreator()
{
	bitwise_not(threshImg, tmp);
	SimpleBlobDetector::Params params;
	params.minThreshold = 200;
	params.maxThreshold = 255;
	params.filterByArea = true;
	params.minArea = 150;
	params.filterByCircularity = true;
	params.minCircularity = 0.6;
	params.filterByConvexity = true;
	params.minConvexity = 0.6;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.6;
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	detector->detect(tmp, keypoints);
	drawKeypoints(threshImg, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}
//----------------------------------------------------------------------------------------------
void okienka()
{
	namedWindow("ustawienia", CV_WINDOW_NORMAL);
	//namedWindow("imgOriginal1", CV_WINDOW_AUTOSIZE);
	namedWindow("game", CV_WINDOW_AUTOSIZE);
	createTrackbar("threshold", "ustawienia", &lowV, 255);
        //createTrackbar("Próba", "ustawienia", &proba, 12);
	//createTrackbar("Numer", "ustawienia", &numer, 3);
	createTrackbar("Repeat ROI", "ustawienia", &zaz_pon, 1);
	createTrackbar("Check", "ustawienia", &doit, 1);
	createTrackbar("NEW GAME", "ustawienia", &nowa, 1);
	createTrackbar("Show key", "ustawienia", &pokaz, 1);
	//imshow("ustawienia", threshImg);
	imshow("ustawienia", im_with_keypoints);
	//imshow("imgOriginal1", imgOriginal1);
	imshow("game", background);

}
//----------------------------------------------------------------------------------------------
void sortowanie()
{
	rzad = ile / 4;
	for (int k = 0; k < ile; k = k + rzad)
		for (int i = 0; i < rzad - 1; i++)
			for (int j = 0; j < rzad - 1; j++)
				if (keypoints[j + k].pt.x > keypoints[j + 1 + k].pt.x)
					swap(keypoints[j + k], keypoints[j + 1 + k]);
}
//----------------------------------------------------------------------------------------------
void tablica()
{
	Mat imgHsv;
	cvtColor(imgOriginal, imgHsv, CV_BGR2HSV);
	pionki = new Pionek * [rzad];
	for(int i = 0; i < rzad; i++)
		pionki[i] = new Pionek[4];
	for (int i = 0; i < rzad; i++)
		for (int j = 0; j < 4; j++)
		{
			if(i != 0)
				pionki[i][j].inicjuj(i, j, rzad, imgOriginal1, imgHsv, keypoints);
			else
				if (nowa == 1 && doit == 1)
				{
					szyfr[j].inicjuj(0, j, rzad, imgOriginal1, imgHsv, keypoints);
					
				}
		}
}
//----------------------------------------------------------------------------------------------
void rysuj()
{
	int koord[13];
	int koord1[13];
	int pom = 750;
	int pom1 = 750;
	for (int i = rzad - 1; i > 0; i--)
	{
		koord[i] = pom;
		pom = pom - 55;
	}
	for (int i = 12; i > 0; i--)
	{
		koord1[i] = pom1;
		pom1 = pom1 - 55;
	}
	for (int i = 12; i > 0; i--)
		for (int j = 3; j >= 0; j--)
			circle(background, Point(300 + 50 * j, koord1[i]), 20, Scalar(255, 255, 255), 1, 8, 0);
	for(int i = rzad - 1; i > 0; i--)
		for (int j = 3; j >= 0; j--)
			circle(background, Point(300+50*j,koord[i]), 20, Scalar((int)pionki[i][j].B, (int)pionki[i][j].G, (int)pionki[i][j].R), -5, 8, 0);
	if (pokaz == 1)
	{
		circle(background, Point(300, 75), 20, Scalar((int)szyfr[0].B, (int)szyfr[0].G, (int)szyfr[0].R), -5, 8, 0);
		circle(background, Point(350, 75), 20, Scalar((int)szyfr[1].B, (int)szyfr[1].G, (int)szyfr[1].R), -5, 8, 0);
		circle(background, Point(400, 75), 20, Scalar((int)szyfr[2].B, (int)szyfr[2].G, (int)szyfr[2].R), -5, 8, 0);
		circle(background, Point(450, 75), 20, Scalar((int)szyfr[3].B, (int)szyfr[3].G, (int)szyfr[3].R), -5, 8, 0);
	}
	for(int i = 300; i<=450; i = i + 50)
		circle(background, Point(i, 75), 20, Scalar(255,255,255), 1, 8, 0);
	for(int i = rzad - 1; i > 0; i--)
		for (int j = 0; j < 4; j++)
		{
			switch (j)
			{
			case 0:
				circle(background, Point(490, koord[i] - 12), 5, Scalar(key[i][j].B, key[i][j].G, key[i][j].R), -1, 2, 0);
				break;
			case 1:
				circle(background, Point(514, koord[i] - 12), 5, Scalar(key[i][j].B, key[i][j].G, key[i][j].R), -1, 2, 0);
				break;
			case 2:
				circle(background, Point(490, koord[i] + 12), 5, Scalar(key[i][j].B, key[i][j].G, key[i][j].R), -1, 2, 0);
				break;
			case 3:
				circle(background, Point(514, koord[i] + 12), 5, Scalar(key[i][j].B, key[i][j].G, key[i][j].R), -1, 2, 0);
				break;
			}
		}
	for (int i = 12; i > 0; i--)
		for (int j = 0; j < 4; j++)
		{
			switch (j)
			{
			case 0:
				circle(background, Point(490, koord1[i] - 12), 5, Scalar(255, 255, 255), 1, 2, 0);
				break;
			case 1:
				circle(background, Point(514, koord1[i] - 12), 5, Scalar(255, 255, 255), 1, 2, 0);
				break;
			case 2:
				circle(background, Point(490, koord1[i] + 12), 5, Scalar(255, 255, 255), 1, 2, 0);
				break;
			case 3:
				circle(background, Point(514, koord1[i] + 12), 5, Scalar(255, 255, 255), 1, 2, 0);
				break;
			}
		}
	for (int i = 722; i >= 172; i = i - 55)
		line(background, Point(260,i), Point(534,i),Scalar(255, 255, 255), 1, 8, 0);
	line(background, Point(260, 110), Point(534, 110), Scalar(255, 255, 255), 2, 8, 0);
	line(background, Point(260, 47), Point(534, 47), Scalar(255, 255, 255), 2, 8, 0);
	line(background, Point(260, 778), Point(534, 778), Scalar(255, 255, 255), 2, 8, 0);
	line(background, Point(260, 47), Point(260, 778), Scalar(255, 255, 255), 2, 8, 0);
	line(background, Point(534, 47), Point(534, 778), Scalar(255, 255, 255), 2, 8, 0);
	line(background, Point(477, 47), Point(477, 778), Scalar(255, 255, 255), 1, 8, 0);
}
//----------------------------------------------------------------------------------------------
void klucz()
{
	int **pomoc = new int *[rzad];
	for (int i = 0; i < rzad; i++)
		pomoc[i] = new int [4];
	for (int i = rzad - 1; i >= 0; i--)
		for (int j = 3; j >= 0; j--)
			pomoc[i][j] = 0;
	int **pomoc1 = new int *[rzad];
	for (int i = 0; i < rzad; i++)
		pomoc1[i] = new int[4];
	for (int i = rzad - 1; i >= 0; i--)
		for (int j = 3; j >= 0; j--)
			pomoc1[i][j] = 0;
	key = new Podp *[rzad];
	for (int i = 0; i < rzad; i++)
		key[i] = new Podp[4];
	// szukanie czerwonych
	for (int i = rzad - 1; i > 0; i--)
		for (int j = 3; j >= 0; j--)
		{
			key[i][j].nadaj(0, 0, 0);
			if ((int)pionki[i][j].H >= ((int)szyfr[j].H - 5) && (int)pionki[i][j].H <= ((int)szyfr[j].H + 5) && (int)pionki[i][j].S >= ((int)szyfr[j].S - 100) && (int)pionki[i][j].S <= ((int)szyfr[j].S + 120))
			{
				key[i][j].nadaj(255, 0, 0);
				pomoc[i][j] = 1;
				pomoc1[i][j] = 1;
				if(key[i][0].R == 255 && key[i][1].R == 255 && key[i][2].R == 255 && key[i][3].R == 255) pokaz = 1;
			}
		}
	// szukanie bia³ych
	for (int i = rzad - 1; i > 0; i--)
		for (int j = 3; j >= 0; j--)
		{
			for (int k = 3; k >= 0; k--)
			{
				if (pomoc[i][j] == 0 && pomoc1[i][k] == 0 && (int)pionki[i][j].H >= ((int)szyfr[k].H - 5) && (int)pionki[i][j].H <= ((int)szyfr[k].H + 5) && (int)pionki[i][j].S >= ((int)szyfr[k].S - 100) && (int)pionki[i][j].S <= ((int)szyfr[k].S + 120))
				{
					key[i][j].nadaj(255, 255, 255);
					pomoc[i][j] = 1;
					pomoc1[i][k] = 1;
				} 
			}
		}
	for (int i = 0; i < rzad - 1; i++)
		delete[] pomoc[i];
	delete[] pomoc;
	for (int i = 0; i < rzad - 1; i++)
		delete[] pomoc1[i];
	delete[] pomoc1;
}
//----------------------------------------------------------------------------------------------
int test()
{
	if (keypoints[0].pt.y < (keypoints[ile / 4 - 1].pt.y - 14) || keypoints[0].pt.y >(keypoints[ile / 4 - 1].pt.y + 6))
		return 0;
	if (keypoints[0].pt.x < (keypoints[3 * ile / 4].pt.x - 8) || keypoints[0].pt.x > (keypoints[3 * ile / 4].pt.x + 8))
		return 0;
	return 1;
}
//----------------------------------------------------------------------------------------------
void errokno(int nr)
{
	while (1)
	{
		char exit = 0;
		namedWindow("Error", CV_WINDOW_AUTOSIZE);
		okienko = Mat3b(200, 300, Vec3b(0, 0, 0));
		putText(okienko, "ENTER aby kontynuowac", Point(okienko.cols / 2 - 100, okienko.rows - 20), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
		if (nr == 1)
		{
			putText(okienko, "Zaznacz cale", Point(40, 30), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255));
			putText(okienko, "pole gry", Point(70, 70), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255));
		}
		if(nr == 2)
		{
			putText(okienko, "Dobierz poprawnie selekcje", Point(40, 30), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
			putText(okienko, "Ustaw plansze poziomo", Point(60, 70), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
			putText(okienko, "Oswietl plansze rownomiernie", Point(30, 110), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
		}
		line(okienko, Point(0, okienko.rows - 50), Point(okienko.cols, okienko.rows - 50), Scalar(255, 255, 255), 1, 8, 0);
		imshow("Error", okienko);
		doit = 0;
		exit = waitKey(1);
		if (exit == 13)
		{
			destroyWindow("Error");
			break;
		}
	}
}
//**********************************************************************************************
int WinMain() 
{

	VideoCapture capWebcam(0);
	if (capWebcam.isOpened() == false) 
	{
		std::cout << "error: Webcam connect unsuccessful\n";
		return(0);           
	}
	capWebcam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capWebcam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {
		bool blnFrameReadSuccessfully;
		Mat poczatek;
		while (czekaj != 13)
		{
			capWebcam.read(imgOrigin);
			poczatek = imgOrigin;
			line(poczatek, Point(0, poczatek.size().height / 3), Point(poczatek.size().width, poczatek.size().height / 3), Scalar(255, 255, 255), 2, 8, 0);
			line(poczatek, Point(0, 2 * poczatek.size().height / 3), Point(poczatek.size().width, 2 * poczatek.size().height / 3), Scalar(255, 255, 255), 2, 8, 0);
			rectangle(poczatek, Point(0, poczatek.rows - 30), Point(poczatek.cols, poczatek.rows), Scalar(0, 0, 0), -1, 8, 0);
			putText(poczatek, "Po ustawieniu zatwierdz ENTER", Point(poczatek.cols / 2 - poczatek.cols / 4,poczatek.rows-10), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
			rectangle(poczatek, Point(0,0), Point(poczatek.cols, 30), Scalar(0, 0, 0), -1, 8, 0);
			putText(poczatek, "Ustaw plansze rownolegle do linni, z szyfrem z lewej strony", Point(40, 20), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
			namedWindow("wstep", CV_WINDOW_AUTOSIZE);
			imshow("wstep", poczatek);
			czekaj = waitKey(500);
		}
		destroyWindow("wstep");
		if (lowVP != lowV || pom == 0 || doit == 1)
			blnFrameReadSuccessfully = capWebcam.read(imgOrigin);
		else
			blnFrameReadSuccessfully = capWebcam.read(imgOrigintmp);
		if (!blnFrameReadSuccessfully || imgOrigin.empty()) {
			cout << "error: frame can't read \n";
			break;
		}
		imgOriginal = imgOrigin;
		if (pom == 0 || zaz_pon == 1)
		{
			zaz_pon = 0;
			int szer = 0;
			int wys = 0;
			while ((szer < (25*imgOrigin.cols/32) || wys < (imgOrigin.rows/4)))
			{
				rectangle(imgOrigin, Point(0, poczatek.rows - 30), Point(poczatek.cols, poczatek.rows), Scalar(0, 0, 0), -1, 8, 0);
				putText(imgOrigin, "Po zaznaczeniu zatwierdz ENTER", Point(poczatek.cols / 2 - poczatek.cols / 4, poczatek.rows - 10), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
				rectangle(imgOrigin, Point(0, 0), Point(poczatek.cols, 30), Scalar(0, 0, 0), -1, 8, 0);
				putText(imgOrigin, "Zaznacz obszar gry (4x13 otworow)", Point(150, 20), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
				r = selectROI(imgOrigin);
				destroyWindow("ROI selector");
				imgOriginal = imgOrigin(r);
				szer = imgOriginal.size().width;
				wys = imgOriginal.size().height;
				if (szer < (25 * imgOrigin.cols / 32) || wys < (imgOrigin.rows / 4))
				{
					errokno(1);
					capWebcam.read(imgOrigin);
				}
				pom = 1;
			}
		}
		else
			imgOriginal = imgOrigin(r);
		if (lowVP != lowV || doit == 1)
		{
			imgtransf();
			blobcreator();
			imgOriginal1 = imgOriginal.clone();
			if (doit == 1)
			{
				background = pombg.clone();
				ile = keypoints.size();
				cout << ile << endl;
				if (ile % 4 != 0 || ile < 4)
				{
					errokno(2);
					goto blad;
				}
				sortowanie();
				if (test() == 0)
				{
					errokno(2);
					cout << "Dobierz poprawnie selekcje i ustaw plansze poziomo  \n";
					goto blad;
				}
				tablica();
				nowa = 0;
				if (proba >= rzad - 1) proba = rzad - 1;
				if (numer >= 3) numer = 3;
			   /* if (proba != 0)
				{
					cout << "H: " << (int)pionki[proba][numer].H << "  " << "S: " << (int)pionki[proba][numer].S << "  " << "V: " << (int)pionki[proba][numer].V << "  " << "HS: " << (int)pionki[proba][numer].sHS << "  " << "X: " << pionki[proba][numer].x << "  " << "Y: " << pionki[proba][numer].y << endl;
					circle(imgOriginal1, pionki[proba][numer].kord, 20, Scalar((int)pionki[proba][numer].B, (int)pionki[proba][numer].G, (int)pionki[proba][numer].R), 10, 8, 0);
				}
				else
				{
					cout << "H: " << (int)szyfr[numer].H << "  " << "S: " << (int)szyfr[numer].S << "  " << "V: " << (int)szyfr[numer].V << "  " << "HS: " << (int)szyfr[numer].sHS << "  " << "X: " << szyfr[numer].x << "  " << "Y: " << szyfr[numer].y << endl;
					circle(imgOriginal1, szyfr[numer].kord, 20, Scalar((int)szyfr[numer].B, (int)szyfr[numer].G, (int)szyfr[numer].R), 10, 8, 0);
				}*/
				klucz();
				rysuj();
				lowVP = lowV;
				numer1 = numer;
				proba1 = proba;
				for (int i = 0; i < rzad; i++)
					delete[] pionki[i];
				delete[] pionki;
				for (int i = 0; i < rzad; i++)
					delete[] key[i];
				delete[] key;
				doit = 0;
			}
			okienka();
		
			
		}
		blad:
		doit = 0;
		charCheckForEscKey = waitKey(1);    
		if (zaz_pon == 1)
			destroyAllWindows();
	}
	return(0);
}