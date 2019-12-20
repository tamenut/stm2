#ifndef __CFILTERPARAMETERDATA_H_20090310__
#define __CFILTERPARAMETERDATA_H_20090310__

#include "constant.h"

struct sFILTER_PARAMETER
{
	sFILTER_PARAMETER()
	{
		iTMode = 0;
		iWarfare = 0;
		iFilterStates = 0;

		dblMeasureSigmaR = 0.;
		dblMeasureSigmaB = 0.;
		dblMeasureSigmaE = 0.;

		dblProcessNoiseModelSigmaW = 0.;
		dblProcessNoiseModelTau = 0.;
		
		iEstiFilterModel = 0;

		dblProcessNoiseCVModelSigmaW = 0.;
		dblProcessNoiseCVModelTau = 0.;
		dblProcessNoiseSingerModelSigmaW = 0.;
		dblProcessNoiseSingerModelTau = 0.;
		dblProcessNoiseSongModelSigmaW = 0.;
		dblProcessNoiseSongModelTau = 0.;
		dblProcessNoiseCTRModelSigmaW = 0.;
		dblProcessNoiseCTRModelTau = 0.;
		dblProcessNoiseCANoiseModelSmallQ = 0.;

	}

	int iTMode;
	int iWarfare;
	int iFilterStates;
	
	double dblMeasureSigmaR;
	double dblMeasureSigmaB;
	double dblMeasureSigmaE;

	double dblProcessNoiseModelSigmaW;
	double dblProcessNoiseModelTau;
	

	int iEstiFilterModel;
	
	double dblProcessNoiseCVModelSigmaW;
	double dblProcessNoiseCVModelTau;
	double dblProcessNoiseSingerModelSigmaW;
	double dblProcessNoiseSingerModelTau;
	double dblProcessNoiseSongModelSigmaW;
	double dblProcessNoiseSongModelTau;
	double dblProcessNoiseCTRModelSigmaW;
	double dblProcessNoiseCTRModelTau;
	double dblProcessNoiseCANoiseModelSmallQ;
};

#endif

