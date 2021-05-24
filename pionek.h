#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

class Pionek
{
public:
	float x;
	float y;
	int H;
	int S;
	int V;
	uchar B;
	uchar G;
	uchar R;
	int sHS;  // suma kanalow H i S
	float size;  //rozmiar bloba
	Point kord;
    void inicjuj(int nr_rzedu, int poz, int l_rzedow, const Mat& obraz, const Mat& obrazhsv, const vector<KeyPoint>& bloby);
};

class Podp
{
public:
	int B;
	int G;
	int R;
	void nadaj(int red, int green, int blue);
};
