#include "blackscholes.h"
#include "stdnormalcdf.h"
#include <math.h>
#include<iostream>
#include<stdlib.h>


blackScholes::blackScholes()
{
	//ctor
}

blackScholes::blackScholes(int call_input, double S_input, double K_input, double r_input, double sigma_input, double T_input)
{
	//ctor
	call = call_input;
	S = S_input;
	K = K_input;
	r = r_input;
	sigma = sigma_input;
	T = T_input;
}

double blackScholes::d1()
{
	return ((log(S / K) + (r + sigma*sigma / 2.)*T) / (sigma*sqrt(T)));
}

double blackScholes::d2()
{
	return ((log(S / K) + (r - sigma*sigma / 2.)*T) / (sigma*sqrt(T)));
}

double blackScholes::Price()
{
	stdNormalCDF Norm;
	double x = S*Norm(call*d1());
	double y = K*exp(-1 * r*T)*Norm(call*d2());
	if (call == 1)
	{
		//        cout<<"The price of the call is "<<call*(x-y)<<endl; // if the values need to be printed in the output screen
		return call*(x - y);
	}
	if (call == -1)
	{
		//        cout<<"The price of the put is "<<call*(x-y)<<endl; // if the values need to be printed in the output screen
		return call*(x - y);
	}

}
