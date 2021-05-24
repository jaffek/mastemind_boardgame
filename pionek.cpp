#include "pionek.h"

using namespace std;
using namespace cv;

void Pionek::inicjuj(int nr_rzedu, int poz, int l_rzedow, const Mat& obraz, const Mat& obrazhsv, const vector<KeyPoint>& bloby)
{
		int k = nr_rzedu + (poz * l_rzedow);
		Vec3b intensity1 = obraz.at<Vec3b>(bloby[k].pt.y + bloby[k].size / 4, bloby[k].pt.x);
		Vec3b intensity2 = obraz.at<Vec3b>(bloby[k].pt.y - bloby[k].size / 4, bloby[k].pt.x);
		Vec3b intensity3 = obraz.at<Vec3b>(bloby[k].pt.y, bloby[k].pt.x + bloby[k].size / 4);
		Vec3b intensity4 = obraz.at<Vec3b>(bloby[k].pt.y, bloby[k].pt.x - bloby[k].size / 4);
		Vec3b intensity;
		intensity.val[0] = (intensity1.val[0] + intensity2.val[0] + intensity3.val[0] + intensity4.val[0]) / 4;
		intensity.val[1] = (intensity1.val[1] + intensity2.val[1] + intensity3.val[1] + intensity4.val[1]) / 4;
		intensity.val[2] = (intensity1.val[2] + intensity2.val[2] + intensity3.val[2] + intensity4.val[2]) / 4;
	   	Vec3b intensity5 = obrazhsv.at<Vec3b>(bloby[k].pt.y + bloby[k].size / 4, bloby[k].pt.x);
		Vec3b intensity6 = obrazhsv.at<Vec3b>(bloby[k].pt.y - bloby[k].size / 4, bloby[k].pt.x);
		Vec3b intensity7 = obrazhsv.at<Vec3b>(bloby[k].pt.y, bloby[k].pt.x + bloby[k].size / 4);
		Vec3b intensity8 = obrazhsv.at<Vec3b>(bloby[k].pt.y, bloby[k].pt.x - bloby[k].size / 4);
		Vec3b intensity0;
		int pom = -1;
		int pom1 = -1;
		if ((int)intensity5.val[0] < 20 || (int)intensity6.val[0] < 20 || (int)intensity7.val[0] < 20 || intensity8.val[0] < 20)
			pom = 1;
		if ((int)intensity5.val[0] > 160 || (int)intensity6.val[0] > 160 || (int)intensity7.val[0] > 160 || intensity8.val[0] > 160)
			pom1 = 1;
		intensity0.val[0] = (intensity5.val[0] + intensity6.val[0] + intensity7.val[0] + intensity8.val[0]) / 4;
		intensity0.val[1] = (intensity5.val[1] + intensity6.val[1] + intensity7.val[1] + intensity8.val[1]) / 4;
		intensity0.val[2] = (intensity5.val[2] + intensity6.val[2] + intensity7.val[2] + intensity8.val[2]) / 4;
		x = bloby[k].pt.x;
		y = bloby[k].pt.y;
		if (pom == 1 && pom1 == 1 || (int)intensity0.val[0] < 5)
			H = 179;
		else
			H = intensity0.val[0];
		S = intensity0.val[1];
		V = intensity0.val[2];
		B = intensity.val[0];
		G = intensity.val[1];
		R = intensity.val[2];
		kord = Point(bloby[k].pt.x, bloby[k].pt.y);
		sHS = (int)H + (int)S;
}
void Podp::nadaj(int red, int green, int blue)
{
	R = red;
	G = green;
	B = blue;
}