//#include "stdafx.h"
#include "rtklib_base.h"

extern int postpos(gtime_t ts, gtime_t te, double ti, double tu,
	const prcopt_t *popt, const solopt_t *sopt,
	const filopt_t *fopt, char **infile, int n, char *outfile,
	const char *rov, const char *base)
{
	double es[] = { 2018,10,1,0,0,0 };
	
	ts = epoch2time(es);
	int week = 0; double output;
	output = time2gpst(ts, &week);


	return output;	///Êä³ö
}