#include <iostream>
#include "CGeneratioNoise.h"
#include <time.h>

CGeneratioNoise::CGeneratioNoise(void)
{
	Initialize();
}

CGeneratioNoise::~CGeneratioNoise(void)
{
}


void CGeneratioNoise::Initialize(void)
{
/// seed값을 설정
	seed = long(time(NULL));
}

double CGeneratioNoise::NoiseGauss(double i_dblMean, double i_dblDeviation)
{
/**
평균 i_dblMean, 편차가 i_dblDeviation인 가우시안 noise를 생성하여 
리턴값 g에 저장한다.
**/
	double as, a, g;
	int i;

	as = i_dblDeviation;
	a = 0;
	
	 for(i=1; i<13; i++) {
		a = a + NoiseUniran();
	}
	
	g = double((a - 6.0) * as + i_dblMean);
	
	return g;
}

double CGeneratioNoise::NoiseUniran(void)
{
/**
0부터 1사이에서 uniform하게 존재하는 random number를 생성하여
리턴값 ans에 저장
**/
	long int b15=32768, b16=65536, h15, h31, ovf,
			 ms=2147483647, m1=24112, m2=26143, lp, l15;
			 
	double ans;

	h15 = seed/b16;
	lp = (seed-h15*b16)	* m1;
	l15 = lp/b16;
	h31 = h15*m1 + l15;
	ovf = h31/b15;
	seed = (((lp-l15*b16)-ms)+(h31-ovf*b15)*b16) + ovf;
	if(seed<0) 
		seed = seed + ms;

	h15 = seed/b16;
	lp = (seed-h15*b16)	* m2;
	l15 = lp/b16;
	h31 = h15*m2 + l15;
	ovf = h31/b15;
	seed = (((lp-l15*b16)-ms)+(h31-ovf*b15)*b16) + ovf;
	if(seed<0) 
		seed = seed + ms;

	ans = double((2 * (seed/256) +1 )/ 16777216.);

	return ans;
}
