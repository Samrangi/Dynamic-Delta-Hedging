#include "stdnormalcdf.h"
#include <cmath>
#include <math.h>
#include <iostream>

using namespace std;

const double A1 = 0.319381530;
const double A2 = -0.356563782;
const double A3 = 1.781477937;
const double A4 = -1.821255978;
const double A5 = 1.330274429;
const double B = 0.2316419;


#define My_PI 3.14159265358979323846
double stdNormalCDF::operator()(double x) const {
	//ctor

	double var = fabs(x);
	double z = 1 / (1 + B * var);
	double R_z = z*(A1 + z*(A2 + z*(A3 + z*(A4 + z*A5))));
	double N_x = 1 - (1 / (sqrt(2 * My_PI))*exp(-(pow(x, 2) / 2))*R_z);
	if (x<0)
		N_x = 1 - N_x;

	//  In order to check the output of the CDF value;
	//	cout << "The Normal CDF output is: " << N_x << endl ;

	return N_x;
}

//stdNormalCDF::~Normal_Calculation()
//{
//    //dtor
//}
