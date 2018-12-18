#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "rtklib_base.h"
//using namespace std;
/* --------- 常数值 --------- */
static const double gpst0[] = { 1980,1, 6,0,0,0 }; /* gps time reference */


/* --------- 默认值 --------- */

const prcopt_t prcopt_default = { /* defaults processing options */
	PMODE_SINGLE,0,2,SYS_GPS,   /* mode,soltype,nf,navsys */
	15.0*D2R,{ { 0,0 } },           /* elmin,snrmask */
	0,1,1,1,                    /* sateph,modear,glomodear,bdsmodear */
	5,0,10,1,                   /* maxout,minlock,minfix,armaxiter */
	0,0,0,0,                    /* estion,esttrop,dynamics,tidecorr */
	1,0,0,0,0,                  /* niter,codesmooth,intpref,sbascorr,sbassatsel */
	0,0,                        /* rovpos,refpos */
	{ 100.0,100.0 },              /* eratio[] */
	{ 100.0,0.003,0.003,0.0,1.0 }, /* err[] */
	{ 30.0,0.03,0.3 },            /* std[] */
	{ 1E-4,1E-3,1E-4,1E-1,1E-2,0.0 }, /* prn[] */
	5E-12,                      /* sclkstab */
	{ 3.0,0.9999,0.25,0.1,0.05 }, /* thresar */
	0.0,0.0,0.05,               /* elmaskar,almaskhold,thresslip */
	30.0,30.0,30.0,             /* maxtdif,maxinno,maxgdop */
	{ 0 },{ 0 },{ 0 },                /* baseline,ru,rb */
	{ "","" },                    /* anttype */
	{ { 0 } },{ { 0 } },{ 0 }             /* antdel,pcv,exsats */
};
const solopt_t solopt_default = { /* defaults solution output options */
	SOLF_LLH,TIMES_GPST,1,3,    /* posf,times,timef,timeu */
	0,1,0,0,0,0,0,              /* degf,outhead,outopt,outvel,datum,height,geoid */
	0,0,0,                      /* solstatic,sstat,trace */
	{ 0.0,0.0 },                  /* nmeaintv */
	" ",""                      /* separator/program name */
};


/* --------- 部分函数实现 --------- */

/* time to gps time ------------------------------------------------------------
* convert gtime_t struct to week and tow in gps time
* args   : gtime_t t        I   gtime_t struct
*          int    *week     IO  week number in gps time (NULL: no output)
* return : time of week in gps time (s)
*-----------------------------------------------------------------------------*/
extern double time2gpst(gtime_t t, int *week)
{
	gtime_t t0 = epoch2time(gpst0);
	time_t sec = t.time - t0.time;
	int w = (int)(sec / (86400 * 7));
	if (week) *week = w;
	return (double)(sec - (double)w * 86400 * 7) + t.sec;
}

/* convert calendar day/time to time -------------------------------------------
* convert calendar day/time to gtime_t struct
* args   : double *ep       I   day/time {year,month,day,hour,min,sec}
* return : gtime_t struct
* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
*-----------------------------------------------------------------------------*/
extern gtime_t epoch2time(const double *ep)
{
	const int doy[] = { 1,32,60,91,121,152,182,213,244,274,305,335 };
	gtime_t time = { 0 };
	int days, sec, year = (int)ep[0], mon = (int)ep[1], day = (int)ep[2];

	if (year<1970 || 2099<year || mon<1 || 12<mon) return time;

	/* leap year if year%4==0 in 1901-2099 */
	days = (year - 1970) * 365 + ((year - 1969) / 4)
		+ doy[mon - 1]/*这一项不是0*/ + day - 2 
		+ (year % 4 == 0 && mon >= 3 ? 1 : 0);
	sec = (int)floor(ep[5]);
	time.time = (time_t)days * 86400 + (int)ep[3] * 3600 + (int)ep[4] * 60 + sec;
	time.sec = ep[5] - sec;

	return time;
}