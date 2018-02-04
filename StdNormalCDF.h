#ifndef STDNORMALCDF_H
#define STDNORMALCDF_H
using namespace std;

class stdNormalCDF
{
public:
	stdNormalCDF() {}
	double operator ()(double) const;
	virtual ~stdNormalCDF() {}
	
};
#endif // STDNORMALCDF_H
