
#pragma once

#include "constant.h"
#include "CFilterParameterData.h"
#include "CTargetDataType.h"
#include "CGeneratioNoise.h"
#include "CAxisTransformations.h"
#include "Scenario_Generation_Target_Trajectory.h"
#include "CMatUtil.h"

#include <math.h>
#include <vector>
#include <deque>


class CSKF
{
public :
	CSKF();
	void ScenarioGen();
	//void RunStandardKalmanFilter();

	void SKF_PredictionX();
	void SKF_PredictionP();
	void SKF_RHSetting();
	void SKF_ResidualCV();
	void SKF_SetKalmanFilterTargetData(TargetData stAbsNoiseTargetData);
	void SKF_Residual();
	void SKF_KalmanGain();
	void SKF_UpdateX();
	void SKF_UpdateP();
	void InitKalmanFilter(TargetData target_data);
	void InitDblR(TargetData target_data);
	void RunStandardKalmanFilter(TargetData target_data);
	void _InitKalmanFilter(TargetData target_data);
	void ReSetValue();
	void SetSyncData(const double m_dblXHat[9], const double m_dblPHat[9][9]);
	void GetSyncData(double m_dblXHat[9], double m_dblPHat[9][9]);

private:
	void ClearData();
	

	
	bool is_init_process;
public :
	deque <TargetData> m_stAbsNoiseTargetData;
	deque <TargetData> m_stAbsTrueTargetData;

	int m_iTargetDataSize;

	TargetData pre_target_data;

	double m_dblSystemMatrix[9][9];		//XXX 동기화 필요없음
	double m_dblDataSamplingRate;		//XXX 동기화 필요없음
	double m_dblQ[9][9];				//XXX 동기화 필요없음

	double m_dblXHat[9];
	double m_dblXBar[9];				//XXX 동기화 필요없음
	double m_dblRPol[3][3];				//XXX 동기화 필요없음

	double m_dblPHat[9][9];
	double m_dblPBar[9][9];				//XXX 동기화 필요없음

	double m_dblR[3][3];				//XXX 동기화 필요없음 @@@@
	double m_dblH[3][9];				//XXX 동기화 필요없음
	double m_dblK[9][3];				//XXX 동기화 필요없음

	double m_dblRS[3][3];			//XXX 동기화 필요없음
	double m_dblzRBE[3];			//XXX 동기화 필요없음				// 표적 위치 정보 (R/B/E)
	double m_dblResidual[3];		//XXX 동기화 필요없음


	double m_dblInputTargetPosX;
	double m_dblInputTargetPosY;
	double m_dblInputTargetPosZ;
	double m_dblInputTargetVelX;
	double m_dblInputTargetVelY;
	double m_dblInputTargetVelZ;
	double m_dblInputTargetRange;
	double m_dblInputTargetBearing;
	double m_dblInputTargetElevation;
	double m_dblFilterOutputPosX;
	double m_dblFilterOutputPosY;
	double m_dblFilterOutputPosZ;
	double m_dblFilterOutputVelX;
	double m_dblFilterOutputVelY;
	double m_dblFilterOutputVelZ;
};
