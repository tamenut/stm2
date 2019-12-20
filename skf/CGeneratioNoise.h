#pragma once

class CGeneratioNoise
{
public:
	CGeneratioNoise(void);
	~CGeneratioNoise(void);
	void Initialize(void);
	
	long int seed;
	double NoiseGauss(double i_dblMean, double i_dblDeviation);
	double NoiseUniran(void);
};
