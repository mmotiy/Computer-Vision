#include <iostream>
#include "MeanShift.h"
#include "Hough.h"
#include "PerspectiveTransform.h"

bool compare(Point a, Point b) {
	return a.x < b.x;
}

int main() {
	// Load image
	CImg<> Img;
	Img.load("Dataset/2.bmp");
	Img.resize(780, 1052, -100, 3);
	auto src = Img;
	// Show that image
	Img.display();

	//Convert color from RGB to Lab
	Img = Img.get_RGBtoLab();

	// Initilize Mean Shift with spatial bandwith and color bandwith
	MeanShift MSProc(8, 20);
	// Segmentation Process include Filtering Process (Region Growing)
	MSProc.MSSegmentation(Img);

	// Print the bandwith
	cout << "the Spatial Bandwith is " << MSProc.hs << endl;
	cout << "the Color Bandwith is " << MSProc.hr << endl;

	// Convert color from Lab to RGB
	Img = Img.get_LabtoRGB();
	// Show the result image
	Img.display();

	auto grayscaled = CImg<uchar>(Img.width(), Img.height(), 1, 1);
	cimg_forXY(grayscaled, x, y) {
		int r = Img(x, y, 0);
		int g = Img(x, y, 1);
		int b = Img(x, y, 2);

		double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
		if(newValue>60)
			grayscaled(x, y) = 255;
		else {
			grayscaled(x, y) = 0;
		}
	}
	grayscaled.save("result/segmentation/6.bmp");
	auto res = grayscaled;
	CImg_3x3(I, uchar);
	cimg_for3x3(grayscaled, x, y,0,0,I,uchar) {
		if (Ipp==255 &&Icp == 255 &&Inp == 255 &&Ipc == 255 &&Icc == 255 
			&&Inc == 255 &&Ipn == 255 &&Icn == 255 &&Inn == 255) {
			res(x, y) = 0;
		}
	}

	res.save("result/edge/6.bmp");
	res.display();

	Hough hough("result/edge/2.bmp");

	PerspectiveTransform temp;


	PerspectiveTransform H = temp.quadrilateralToQuadrilateral(0, 0, 779, 0, 0, 1051, 779, 1051, 
		hough.points[2].x, hough.points[2].y, hough.points[0].x, hough.points[0].y,
		hough.points[3].x, hough.points[3].y, hough.points[1].x, hough.points[1].y);

	/* Method 1: Projective Transforming */
	CImg<uchar> dest(780, 1052, 1, 3);
	cimg_forXY(dest, x, y)
	{
		double denominator = H.a13 * x + H.a23 * y + H.a33;
		double tx = (H.a11 * x + H.a21 * y + H.a31) / denominator;
		double ty = (H.a12 * x + H.a22 * y + H.a32) / denominator;
		//cout << tx << " " << ty << endl;
		cimg_forC(dest,c)
			dest(x, y, c) = src.linear_atXYZC(tx, ty,c);
	}

	dest.save("result/res/2.bmp");
	dest.display();
	return 0;
}