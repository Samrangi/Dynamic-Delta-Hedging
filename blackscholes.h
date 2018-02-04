#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H


class blackScholes
{
public:
	double d1();
	double d2();
	double Price();
	blackScholes(int call_input, double S_input, double K_input, double r_input, double sigma_input, double T_input);
	blackScholes();

	virtual ~blackScholes() {};
	double call;

private:
	double S;
	double K;
	double r;
	double sigma;
	double T;
};

#endif // BLACKSCHOLES_H#pragma once
