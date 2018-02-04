#include <iostream>
#include <fstream>
#include <vector>
#include "stdnormalcdf.h"
#include "blackscholes.h"
#include <time.h>
#include <cmath>
#include <random>
#include<string>
#include<numeric>
#include<cstdlib>
#include<cstring>
#include<locale>

#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <ql/quantlib.hpp>


using namespace std;
using namespace QuantLib;

//Quantlib Function to convert String to Date

QuantLib::Date ConvertStrtoDate(std::string str)
{
	int date[3], j(0);
	size_t pos;
	std::string delim = "-";
	while ((pos = str.find(delim)) != std::string::npos)
	{
		date[j++] = atoi(str.substr(0, pos).c_str());
		str.erase(0, pos + delim.length());
	}
	date[j] = atoi(str.c_str());
	return QuantLib::Date(date[2], QuantLib::Month(date[1]), date[0]);
}



//Function to calculate the mean of the Hedging Error

double mean(vector<double> HE_individual)
{
	double HE_individual_sum = 0.0, HE_individual_mean, HE_individual_std = 0.0;
	unsigned int i = 0;
	for (i = 0; i < HE_individual.size(); i++)
	{
		HE_individual_sum += HE_individual[i];
	}
	HE_individual_mean = accumulate(HE_individual.begin(), HE_individual.end(), 0.0) / HE_individual.size();
	return HE_individual_mean;
}


//Function to calculate the standard deviation of the Hedging Error

double std_dev(vector<double> HE_individual)
{
	double HE_individual_mean = mean(HE_individual);
	double HE_individual_std = 0 ;
	for (unsigned int i = 0; i < HE_individual.size(); i++)
	{
		HE_individual_std += pow(HE_individual[i] - HE_individual_mean, 2);
	}
	HE_individual_std = sqrt(HE_individual_std / HE_individual.size());
	return HE_individual_std;
}

// Using bisection method to calculate the value of implied volatility

double interval_bisection(double y_target, double m, double n, double epsilon, double S_input, double K_input, double r_input, double T_input)
{
	double x = 0.5 * (m + n);	// Keeping initial guess as (min+max) /2
	int call_input = 1;			// passing 1 for Call Option, can be changed to -1 for put option
	blackScholes BS(call_input, S_input, K_input, r_input, x, T_input);
	double y = BS.Price();		// Calling BlackScholes to calculate the price

	//Converging the value of x such that the calculated price is closest to the target price
	do
	{
		if (y < y_target)		
		{
			m = x;
		}
		if (y > y_target)
		{
			n = x;
		}
		x = 0.5 * (m + n);
		blackScholes BS(call_input, S_input, K_input, r_input, x, T_input);
		y = BS.Price();
	} while (fabs(y - y_target) > epsilon);		// Continuing to calculate until the tolerance level is reached, also called epsilon
	return x;
}

int main()
{

	//Question 2

	/*****************************************************************/
	/*****************************************************************/

	//Declaring vectors to input all data given in the csv files
	//Reading all data is not necessary but can be useful later on

	//Vector for the interest free rates
	vector<string> interest_date;
	vector<double> interest_rate;

	//Vector for the adjusted closed prices
	vector<string> sec_goog_date;
	vector<double> sec_goog_close;

	//Vector for the data of op_GOOG
	vector<string> op_goog_date;
	vector<string> op_goog_exdate;
	vector<string> op_goog_cpflag;
	vector<double> op_goog_strike_price;
	vector<double> op_goog_best_bid;
	vector<double> op_goog_best_offer;

	/*****************************************************************/

	//Declaring the strings for reading the values from csv to string and then passing onto the vectors
	string interest_date_input;
	string interest_rate_input;
	ifstream interest("./interest.csv");

	string sec_goog_date_input;
	string sec_goog_close_input;
	ifstream sec_goog("./sec_GOOG.csv");

	string op_goog_date_input;
	string op_goog_exdate_input;
	string op_goog_cpflag_input;
	string op_goog_strike_price_input;
	string op_goog_best_bid_input;
	string op_goog_best_offer_input;
	ifstream op_goog("./op_GOOG.csv");

	/*****************************************************************/

	//Checking for file reading error
	if (interest.fail() || sec_goog.fail() || op_goog.fail())
	{
		cerr << "Error opening one of the files";
		exit(1);	// just a check to exit the program for sure
	}
	else
	{
		cout << "Successfully imported the files" << endl;
	}
	/*****************************************************************/

	//Reading the values from csv to string and then passing onto the vectors
	while (!interest.eof())
	{
		getline(interest, interest_date_input, ',');
		getline(interest, interest_rate_input, '\n');
		if (isdigit(interest_date_input[0]))
		{
			interest_date.push_back(interest_date_input);
			interest_rate.push_back((stod(interest_rate_input)) / 100);
		}
	}


	while (!sec_goog.eof())
	{
		getline(sec_goog, sec_goog_date_input, ',');
		getline(sec_goog, sec_goog_close_input, '\n');
		if (isdigit(sec_goog_date_input[0]))
		{
			sec_goog_date.push_back(sec_goog_date_input);
			sec_goog_close.push_back(stod(sec_goog_close_input));
		}
	}

	while (!op_goog.eof())
	{
		getline(op_goog, op_goog_date_input, ',');
		getline(op_goog, op_goog_exdate_input, ',');
		getline(op_goog, op_goog_cpflag_input, ',');
		getline(op_goog, op_goog_strike_price_input, ',');
		getline(op_goog, op_goog_best_bid_input, ',');
		getline(op_goog, op_goog_best_offer_input, '\n');
		if (isdigit(op_goog_date_input[0]))
		{
			op_goog_date.push_back(op_goog_date_input);
			op_goog_exdate.push_back(op_goog_exdate_input);
			op_goog_cpflag.push_back(op_goog_cpflag_input);
			op_goog_strike_price.push_back(stod(op_goog_strike_price_input));
			op_goog_best_bid.push_back(stod(op_goog_best_bid_input));
			op_goog_best_offer.push_back(stod(op_goog_best_offer_input));

		}
	}

	/*****************************************************************/

	//Closing the files opened

	interest.close();
	sec_goog.close();
	op_goog.close();

	/*****************************************************************/
	/*****************************************************************/

	/*****************************************************************/

	//Creating csv files to be written onto

	ofstream result("./result.csv");		//Original Output File
	ofstream myresult("./myresult.csv");	//This is to be used inly to check all the generated values, like delta, B and standard values like risk free rate, strike price tc.

	string date_start_input;
	string date_end_input;
	string time_input;
	string Call;
	double strike_input;

	//The following snippet to be used when taking input from the user

	/*cout << "Enter the start date in yyyy-mm-dd format: ";
	cin >> date_start_input ;
	cout << "Enter the end date in yyyy-mm-dd format: ";
	cin >> date_end_input ;
	cout << "Enter 'C' for Call: ";
	cin >> Call;
	cout << "Enter the date of Maturity: ";
	cin >> time_input ;
	cout << "Enter the strike price: ";
	cin >> strike_input ;*/

	//The following snippet to be used when hard coding the input
	//Comment out the next 6 lines and uncomment the above snippet

	date_start_input = "2011-07-05";
	date_end_input = "2011-07-29";
	time_input = "2011-09-17";
	Call = 'C';
	strike_input = 500;


	//Filtering the data to be needed for ease of access and to reduce access time

	vector<string> filter_start_date;	//Creating a vector for the starting date
	vector<string> filter_end_date;		//Creating a vector for the ending date
	vector<double> filter_strike_price;	//Creating a vector for the expiry date
	vector<double> filter_mid_price;	//Creating a vector for the option price
	vector<double> imp_Vol;				//Creating a vector for the implied volatility
	vector<double> delta;				//Creating a vector for delta calculation, required for B and HE
	vector<double> B;					//Creating B Vector
	vector<double> HE_cumulative;		//Creating Hegdging Error Vector
	vector<double> PNL;					//Creating a vector for the PNL
	vector<double> HE_individual;		//Creating a vector for the individual Hedging Errors

	//Creating an iterator to go through the vectors
	unsigned int i = 0;

	//CReating flags to specify that the data passed is present in the files

	int flag1 = 0, flag2 = 0, flag3 = 0;

	//Reading the filtered data

	for (i = 0; i < op_goog_date.size(); i++)
	{
		if (op_goog_date[i] >= date_start_input && op_goog_date[i] <= date_end_input  && time_input == op_goog_exdate[i] && strike_input == op_goog_strike_price[i] && Call == op_goog_cpflag[i])
		{
			filter_start_date.push_back(op_goog_date[i]);
			filter_end_date.push_back(op_goog_date[i]);
			filter_strike_price.push_back(op_goog_strike_price[i]);
			filter_mid_price.push_back((op_goog_best_bid[i] + op_goog_best_offer[i]) / 2);
			flag1 = 1;
		}

	}

	//Reading the filtered interest rate

	//Creating a vector for the interest rate

	vector<double> filter_interest_rate;
	for (i = 0; i < interest_date.size(); i++)
	{
		if (date_start_input <= interest_date[i] && date_end_input >= interest_date[i])
		{
			filter_interest_rate.push_back(interest_rate[i]);
			flag2 = 1;
		}
	}


	//Reading the filtered interest rate

	//Creating a vector for the adjusted close price
	vector<double> filter_close;

	for (i = 0; i < sec_goog_date.size(); i++)
	{
		if (sec_goog_date[i] >= date_start_input  && sec_goog_date[i] <= date_end_input)
		{
			filter_close.push_back(sec_goog_close[i]);
			flag3 = 1;
		}

	}

	//Checking whether data was read properly and that entries existed in the given data
	if (flag1 == 1 && flag2 == 1 && flag3 == 1)
	{
		cout << "Entries found. Please wait!" << endl;
	
	
	//The following snippet can be used to check that the size of all the entries are the same.

	/*cout << "Size of filter start date: " << filter_start_date.size() << endl;
	cout << "Size of filter end date: " << filter_end_date.size() << endl;
	cout << "Size of filter interest rate: " << filter_interest_rate.size() << endl;
	cout << "Size of filter strike price: " << filter_strike_price.size() << endl;
	cout << "Size of filter option(mid) price: " << filter_mid_price.size() << endl;
	cout << "Size of filter close: " << filter_close.size() << endl;*/

		myresult << "Date" <<  "," << "Spot Price" << "," << "r" << "," << "Strike Price" << "," << "V" << "," << "Delta" << "," << "B"<< "," << "HE" << "," << "PNL" << "," << "HE_individual" << "\n";
		
		//Creating the headers of the csv file

		result << "Date" << "," << "Spot Price" << "," << "V" << "," << "Implied Vol" << "," << "HE_cumulative" << "," << "PNL" << "," << "HE_individual"<< "," << "\n";
		
		//Using Quantlib's inbuilt Calender to convert the dates to standard format and considering US holidays
		
		Calendar US_Cal = UnitedStates();
		Date end_date = ConvertStrtoDate(time_input);
		Date start_date = ConvertStrtoDate(date_start_input);

		//Counting the number of days
		double days_count = US_Cal.businessDaysBetween(start_date, end_date);

		unsigned int k;
		double min_Vol = 0.001;
		double max_Vol = 1;
		double tolerance = 0.000001;
		double K = strike_input;

		//Calculating the implied Volatility across all N periods

		for (k = 0; k < filter_start_date.size() ; k++)
		{
			
			double mid = filter_mid_price[k];		//option price
			double S = filter_close[k];				//adjusted close price
			double rfr = filter_interest_rate[k];	//risk free rate
			double T = (days_count - k) / 252;		//Time to maturity

			//Calling the bisection method to calculate the value of sigma

			double sigma = interval_bisection(mid, min_Vol, max_Vol, tolerance, S, K, rfr, T);
			imp_Vol.push_back(sigma);

			//The following if condition is only for the initial values of B and HE as they have a different formula
			if (k == 0)
			{
				blackScholes BS(1, S, K, rfr, sigma, T);
				stdNormalCDF Norm;
				double d1_value = BS.d1();
				delta.push_back(Norm(d1_value));
				B.push_back(filter_mid_price[0] - (delta[0] * filter_close[0]));
				HE_cumulative.push_back(0);
				HE_individual.push_back(0);
				myresult << filter_start_date[0] << "," << S << "," << filter_interest_rate[0] << "," << K << "," << mid << "," << imp_Vol[0] << "," << B[0] << "," << HE_cumulative[0] << "," << (filter_mid_price[0] - filter_mid_price[0]) << "," << HE_individual[0] << "\n";
				result << filter_start_date[0] << "," << S << "," << mid << "," << imp_Vol[0] << "," << HE_cumulative[0] << "," << (filter_mid_price[0] - filter_mid_price[0]) << "," << HE_individual[0] << "\n";
				
			}

			//Calculating the corresponsing values of delta, B, Cumulative Hedging Error, PNL and individual Hedging Error

			else 
			{
				blackScholes BS(1, S, K, rfr, sigma, T);
				stdNormalCDF Norm;
				double d1_value = BS.d1();
				delta.push_back(Norm(d1_value));
				B.push_back((delta[k - 1] * filter_close[k]) + (B[k - 1] * exp(filter_interest_rate[k-1]/252)) - (delta[k] * filter_close[k]));
				HE_cumulative.push_back(delta[k - 1] * filter_close[k] + (B[k - 1] * exp(filter_interest_rate[k-1]/252)) - filter_mid_price[k]);
				HE_individual.push_back(HE_cumulative[k] - HE_cumulative[k-1]);

				//Writing the generated values to the csv files created

				myresult << filter_start_date[k] << "," << S << "," << filter_interest_rate[k] << "," << K << "," << mid << "," << imp_Vol[k] << "," << B[k] << "," << HE_cumulative[k] << "," << (filter_mid_price[k] - filter_mid_price[0]) << "," << HE_individual[k] << "\n";
				result << filter_start_date[k] << "," << S << "," << mid << "," << imp_Vol[k] << "," << HE_cumulative[k] << "," << (filter_mid_price[k] - filter_mid_price[0]) << "," << HE_individual[k]<< "\n";
			}
		}

	}

	//The control will go to 'else' only if the data entered by the user was not found
	else
	{
		cout << "Bad Input" << endl;
		exit(1);
	}

	//The following is to calculate the mean and standard deviation of the  hedging error


	double HE_individual_mean = mean(HE_individual);	//mean of the individual error
	double HE_individual_std = std_dev(HE_individual);	//standard deviation of the individual hedging error
	double count = 0.0;									//to count the number of hedging errors within twice of standard deviations
	for (i = 0; i < HE_individual.size(); i++)
	{
		if (HE_individual[i] <= (HE_individual_mean + 2 * HE_individual_std) || HE_individual[i] >= (HE_individual_mean - 2 * HE_individual_std))
			count = count + 1.0;
	}

	double pct_95_HE_individual = (count / i) * 100;	// Calculating the percentage of entries within twice of standard deviations

	myresult << '\n' << '\n' << "The mean of Hedging Error is : " << HE_individual_mean << endl;
	myresult << "The standard deviation of Hedging Error is : " << HE_individual_std << endl;
	myresult << '\n' << " Number within 2 standard deviation: " << pct_95_HE_individual << endl;

	
	result << '\n' << '\n' << "The mean of Hedging Error is : " << HE_individual_mean << endl;
	result << "The standard deviation of Hedging Error is : " << HE_individual_std << endl;
	result<< '\n' << " Number within 2 standard deviation: " << pct_95_HE_individual << endl ;

	//Closing the csv files

	myresult.close();
	result.close();

	cout << "End of the Program ! " << endl;
	return 0;
}