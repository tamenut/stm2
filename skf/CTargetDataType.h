#ifndef __CTARGET_DATA_TYPE__
#define __CTARGET_DATA_TYPE__

struct TargetData{
	TargetData()
	{
		seq = 0;
		idx = -1;
//		PosX = 0.;
//		PosY = 0.;
//		PosZ = 0.;
		VelX = 0.;
		VelY = 0.;
		VelZ = 0.;
		AccX = 0.;
		AccY = 0.;
		AccZ = 0.;
		R = 0.;
		B = 0.;
		E = 0.;
		X = 0.;
		Y = 0.;
		Z = 0.;
		Time = 0.;
		ValidateFlag = 0;
	};
	long long seq;
	int idx;
//	double PosX;
//	double PosY;
//	double PosZ;
	double VelX;
	double VelY;
	double VelZ;
	double AccX;
	double AccY;
	double AccZ;
	double R;
	double B;
	double E;
	double X;
	double Y;
	double Z;
	double Time;
	int ValidateFlag;
};

#endif

