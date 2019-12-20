#include "skf.h"
#include <iostream>

#define STATES 9

CAxisTransformations m_cAxisTransform;	//좌표 변환 Class
CGeneratioNoise m_cNoise;
CMatUtil m_cMat;

CSKF::CSKF()
{
	is_init_process = false;
	ClearData();
}

void CSKF::ScenarioGen()
{
	double m_dblInitTargetPosX = 1000;
	double m_dblInitTargetPosY = 1000;
	double m_dblInitTargetPosZ = 1000;
	double m_dblInitTargetRoll = 0;
	double m_dblInitTargetPitch = 0;
	double m_dblInitTargetYaw = 45;
	double m_dblInitTargetVel= 30;
	double m_dblInitTargetAcc = 0;
	double m_dblInitTargetBeta = 0;
	double m_dblInitTargetEpsil = 0;

	double m_dblTotalSimulationTime = 10000;
	Scenario_Generation_Target_Trajectory SGTT;


	double m_dblMeasureSigmaR = MEASUREMENT_NOISE_SIGMA_RANGE_MFR_AIR;
	double m_dblMeasureSigmaB = MEASUREMENT_NOISE_SIGMA_BEARING_MFR_AIR;
	double m_dblMeasureSigmaE = MEASUREMENT_NOISE_SIGMA_ELEVATION_MFR_AIR;

	TargetData stAbsTrueTargetData;
	TargetData stAbsNoiseTargetData;

	int nTimeIdx = 0;
	int seq = 1;

	m_stAbsNoiseTargetData.clear();

	// 시나리오 초기값 설정
	SGTT.InputScenarioParam(
			m_dblInitTargetPosY,	//Pos North	
		m_dblInitTargetPosX,	//Pos East
		m_dblInitTargetPosZ,	//Pos Up
		m_dblInitTargetVel,		//Vel
		m_dblInitTargetRoll,	//Roll
		m_dblInitTargetPitch,	//Pitch
		m_dblInitTargetYaw,		//Heading
		m_dblInitTargetAcc,		//Acc
		m_dblInitTargetBeta,	//Beta
		m_dblInitTargetEpsil);	//Epsil

	// 센서 정의(데이터 샘플링 rate)
	SGTT.Scenario_Initialize_Target_Trajectory(TRACKING);


	while(SGTT.OUTPUT_m_TOF_T < m_dblTotalSimulationTime)	//0.005(적분시간)sec 마다 iteration이 수행.
	{	
		SGTT.runge_kutta_Target();

		stAbsTrueTargetData.R = 0.;
		stAbsTrueTargetData.B = 0.;
		stAbsTrueTargetData.E = 0.;
		stAbsTrueTargetData.X = 0.;
		stAbsTrueTargetData.Y = 0.;
		stAbsTrueTargetData.Z = 0.;

		stAbsTrueTargetData.X = SGTT.OUTPUT_m_Target_Position_LI[0];
		stAbsTrueTargetData.Y = SGTT.OUTPUT_m_Target_Position_LI[1];
		//NED를 NEU로 변경
		stAbsTrueTargetData.Z = SGTT.OUTPUT_m_Target_Position_LI[2]*-1.;

		m_cAxisTransform.Cart2Sph(stAbsTrueTargetData.X, stAbsTrueTargetData.Y, stAbsTrueTargetData.Z,
			stAbsTrueTargetData.R, stAbsTrueTargetData.B, stAbsTrueTargetData.E);
	

		// 향후 True 값. 기준값을 위하여...
		stAbsTrueTargetData.VelX = SGTT.OUTPUT_m_Target_Velocity_LI[0];
		stAbsTrueTargetData.VelY = SGTT.OUTPUT_m_Target_Velocity_LI[1];
		//NED를 NEU로 변경
		stAbsTrueTargetData.VelZ = SGTT.OUTPUT_m_Target_Velocity_LI[2]*-1.;

		stAbsTrueTargetData.AccX = SGTT.OUTPUT_m_Target_Acceleration_LI[0];
		stAbsTrueTargetData.AccY = SGTT.OUTPUT_m_Target_Acceleration_LI[1];
		//NED를 NEU로 변경
		stAbsTrueTargetData.AccZ = SGTT.OUTPUT_m_Target_Acceleration_LI[2]*-1.;

		stAbsTrueTargetData.Time = nTimeIdx * SGTT.m_stt;
		stAbsTrueTargetData.ValidateFlag = true;

		stAbsTrueTargetData.seq = seq;
		stAbsTrueTargetData.idx = 1;

		m_stAbsTrueTargetData.push_back(stAbsTrueTargetData);

		stAbsNoiseTargetData.seq = seq++;
		stAbsNoiseTargetData.idx = 1;

#if 0	//RBE 노이즈 생성
/*
		stAbsNoiseTargetData.R = stAbsTrueTargetData.R + 10*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.B = stAbsTrueTargetData.B + 10*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaB);
		stAbsNoiseTargetData.E = stAbsTrueTargetData.E + 10*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaE);
*/
		stAbsNoiseTargetData.R = stAbsTrueTargetData.R + 6*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.B = stAbsTrueTargetData.B + 6*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaB);
		stAbsNoiseTargetData.E = stAbsTrueTargetData.E + 6*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaE);

		m_cAxisTransform.Sph2Cart(stAbsNoiseTargetData.R, stAbsNoiseTargetData.B, stAbsNoiseTargetData.E,
			stAbsNoiseTargetData.X, stAbsNoiseTargetData.Y, stAbsNoiseTargetData.Z);
#else	//XYZ 노이즈 생성
#if 1
		stAbsNoiseTargetData.X = stAbsTrueTargetData.X + 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.Y = stAbsTrueTargetData.Y + 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.Z = stAbsTrueTargetData.Z + 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
#else	//noise 제거 코드
		stAbsNoiseTargetData.X = stAbsTrueTargetData.X;// + 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.Y = stAbsTrueTargetData.Y;// + 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
		stAbsNoiseTargetData.Z = stAbsTrueTargetData.Z;//+ 2*m_cNoise.NoiseGauss(0, m_dblMeasureSigmaR);
#endif
		m_cAxisTransform.Cart2Sph(stAbsNoiseTargetData.X, stAbsNoiseTargetData.Y, stAbsNoiseTargetData.Z, 
			stAbsNoiseTargetData.R, stAbsNoiseTargetData.B, stAbsNoiseTargetData.E);

#endif
		

		stAbsNoiseTargetData.Time = nTimeIdx * SGTT.m_stt;
		stAbsNoiseTargetData.ValidateFlag = true;

		// Data 저장을 위하여...
		//XXX
		m_stAbsNoiseTargetData.push_back(stAbsNoiseTargetData);
		//m_stAbsNoiseTargetData.push_back(stAbsTrueTargetData);

		nTimeIdx++;
	}

	m_iTargetDataSize = m_stAbsNoiseTargetData.size();
}

void CSKF::ClearData()
{
	int i, j;

	for(i=0; i<3; i++)
	{
		m_dblResidual[i] = 0.;

		for(j=0; j<3; j++)
		{
			m_dblRS[i][j] = 0.;
		}
	}

	for(i=0; i<3; i++)
	{
		for(j=0; j<STATES; j++)
		{
			m_dblK[j][i] = 0.;
		}
	}

	for(i=0; i<9;i++)
	{
		m_dblXHat[i] = 0.;
		m_dblXBar[i] = 0.;

		for(j=0; j<9; j++)
		{
			m_dblPHat[i][j] = 0.;
			m_dblPBar[i][j] = 0.;
			m_dblSystemMatrix[i][j] = 0.;
			m_dblQ[i][j] = 0.;
		}
	}

	for(i=0; i<3; i++)
	{
		for(j=0; j<STATES; j++)
		{
			if(i!=j)
			{
				m_dblH[i][j] = 0.0;
			}
			else
			{
				m_dblH[i][j] = 1.0;
			}
		}
	}
}

void CSKF::_InitKalmanFilter(TargetData target_data)
{
	if(is_init_process == false)
	{
		is_init_process = true;
		double m_dblMeasureSigmaR = MEASUREMENT_NOISE_SIGMA_RANGE_MFR_AIR;
		double m_dblMeasureSigmaB = MEASUREMENT_NOISE_SIGMA_BEARING_MFR_AIR;
		double m_dblMeasureSigmaE = MEASUREMENT_NOISE_SIGMA_ELEVATION_MFR_AIR;

		double m_dblSmallQ;
		double m_dblProcessNoiseCVModelSigmaW = PROCESS_NOISE_CV_MODEL_SIGMA_W_MFR_AIR;

		m_dblDataSamplingRate = 0.1;

		// CV Model System Matrix
		m_dblSystemMatrix[0][0] = m_dblSystemMatrix[1][1] = m_dblSystemMatrix[2][2] = 1.0;
		m_dblSystemMatrix[3][3] = m_dblSystemMatrix[4][4] = m_dblSystemMatrix[5][5] = 1.0;
		m_dblSystemMatrix[0][3] = m_dblSystemMatrix[1][4] = m_dblSystemMatrix[2][5] = m_dblDataSamplingRate;

		m_dblRPol[0][0] = pow(m_dblMeasureSigmaR,2);	m_dblRPol[0][1] = 0.;								m_dblRPol[0][2] = 0.;
		m_dblRPol[1][0] = 0.;							m_dblRPol[1][1] = pow(m_dblMeasureSigmaB,2);		m_dblRPol[1][2] = 0.;
		m_dblRPol[2][0] = 0.;							m_dblRPol[2][1] = 0.;								m_dblRPol[2][2] = pow(m_dblMeasureSigmaE,2);

		m_dblSmallQ = pow(m_dblProcessNoiseCVModelSigmaW,2);

		m_dblQ[0][0] = m_dblSmallQ * pow(m_dblDataSamplingRate, 4) / 4.0;
		m_dblQ[1][1] = m_dblQ[2][2] = m_dblQ[0][0];
		m_dblQ[0][3] = m_dblSmallQ * pow(m_dblDataSamplingRate, 3) / 2.0;
		m_dblQ[1][4] = m_dblQ[2][5] = m_dblQ[0][3];
		m_dblQ[3][0] = m_dblQ[4][1] = m_dblQ[5][2] = m_dblQ[0][3];
		m_dblQ[3][3] = m_dblSmallQ * m_dblDataSamplingRate *m_dblDataSamplingRate;
		m_dblQ[4][4] = m_dblQ[5][5] = m_dblQ[3][3];

		InitDblR(target_data);
	}
}
void CSKF::ReSetValue()
{
	is_init_process = false;
	memset(&pre_target_data, 0, sizeof(pre_target_data));
	ClearData();
}

void CSKF::InitDblR(TargetData target_data)
{
	double dblRange, dblPsi, dblTheta;
	double dblJacobian[3][3] = {0.,}, dblJacobianTrans[3][3] = {0.,}, dblTempMatrix[3][3] = {0.,};
	double dblTmpMatrix1[3][3] = {{0.,},};
	m_cAxisTransform.Cart2Sph(target_data.X, target_data.Y, target_data.Z, dblRange, dblTheta, dblPsi);

	dblJacobian[0][0] = sin(dblTheta)*cos(dblPsi);
	dblJacobian[0][1] = dblRange*cos(dblTheta)*cos(dblPsi);
	dblJacobian[0][2] = -dblRange*sin(dblTheta)*sin(dblPsi);
	dblJacobian[1][0] = cos(dblTheta)*cos(dblPsi);
	dblJacobian[1][1] = -dblRange*sin(dblTheta)*cos(dblPsi);
	dblJacobian[1][2] = -dblRange*cos(dblTheta)*sin(dblPsi);
	dblJacobian[2][0] = sin(dblPsi);
	dblJacobian[2][1] = 0.0;
	dblJacobian[2][2] = dblRange*cos(dblPsi);

	m_cMat.transpose_33(dblJacobianTrans, dblJacobian);
	m_cMat.multi_33_33(dblTmpMatrix1, m_dblRPol, dblJacobianTrans);
	m_cMat.multi_33_33(m_dblR, dblJacobian, dblTmpMatrix1);
}

void CSKF::InitKalmanFilter(TargetData target_data)
{
	_InitKalmanFilter(target_data);
	double m_dblSimulationCurrentTime = 0;

	m_dblXHat[0] = target_data.X;
	m_dblXHat[1] = target_data.Y;
	m_dblXHat[2] = target_data.Z;
	m_dblXHat[3] = (target_data.X - pre_target_data.X)/(target_data.Time - pre_target_data.Time);
	m_dblXHat[4] = (target_data.Y - pre_target_data.Y)/(target_data.Time - pre_target_data.Time);
	m_dblXHat[5] = (target_data.Z - pre_target_data.Z)/(target_data.Time - pre_target_data.Time);
	m_dblXHat[6] = 0.;
	m_dblXHat[7] = 0.;
	m_dblXHat[8] = 0.;

	m_dblPHat[0][0]=m_dblR[0][0];
	m_dblPHat[1][1]=m_dblR[1][1];
	m_dblPHat[2][2]=m_dblR[2][2];
	m_dblPHat[0][3]=m_dblPHat[3][0]=m_dblR[0][0]/m_dblDataSamplingRate;
	m_dblPHat[1][4]=m_dblPHat[4][1]=m_dblR[1][1]/m_dblDataSamplingRate;
	m_dblPHat[2][5]=m_dblPHat[5][2]=m_dblR[2][2]/m_dblDataSamplingRate;
	m_dblPHat[3][3]=2.*m_dblR[0][0]/(m_dblDataSamplingRate*m_dblDataSamplingRate);
	m_dblPHat[4][4]=2.*m_dblR[1][1]/(m_dblDataSamplingRate*m_dblDataSamplingRate);
	m_dblPHat[5][5]=2.*m_dblR[2][2]/(m_dblDataSamplingRate*m_dblDataSamplingRate);
	m_dblPHat[6][6]=0.2;
	m_dblPHat[7][7]=0.2;
	m_dblPHat[8][8]=0.2;

	m_dblInputTargetPosX = target_data.X;
	m_dblInputTargetPosY = target_data.Y;
	m_dblInputTargetPosZ = target_data.Z;
	m_dblInputTargetVelX = (target_data.X - pre_target_data.X)/m_dblDataSamplingRate;
	m_dblInputTargetVelY = (target_data.Y - pre_target_data.Y)/m_dblDataSamplingRate;
	m_dblInputTargetVelZ = (target_data.Z - pre_target_data.Z)/m_dblDataSamplingRate;
	m_dblInputTargetRange = target_data.R;
	m_dblInputTargetBearing = target_data.B *(180./3.141592);
	m_dblInputTargetElevation = target_data.E *(180./3.141592);

	m_dblFilterOutputPosX = m_dblXHat[0];
	m_dblFilterOutputPosY = m_dblXHat[1];
	m_dblFilterOutputPosZ = m_dblXHat[2];
	m_dblFilterOutputVelX = m_dblXHat[3];
	m_dblFilterOutputVelY = m_dblXHat[4];
	m_dblFilterOutputVelZ = m_dblXHat[5];
	m_dblSimulationCurrentTime = target_data.Time;
}

void CSKF::RunStandardKalmanFilter(TargetData target_data)
{
	/*
	이함수내에서 수행하여 나온 결과값이 다음 호출때에 영향을 미치지는 않는것은 동기화 하지 않음
	--> 이 함수의 초기에 값을 설정하는 변수들은 동기화 하지 않음
	*/
	_InitKalmanFilter(target_data);

	double m_dblSimulationCurrentTime = 0;
	SKF_PredictionX();
	SKF_PredictionP();

	SKF_RHSetting();
	SKF_ResidualCV();

	SKF_SetKalmanFilterTargetData(target_data);

	SKF_Residual();
	SKF_KalmanGain();

	SKF_UpdateX();
	SKF_UpdateP();

	m_dblInputTargetPosX = target_data.X;
	m_dblInputTargetPosY = target_data.Y;
	m_dblInputTargetPosZ = target_data.Z;
	m_dblInputTargetVelX = (target_data.X - pre_target_data.X)/m_dblDataSamplingRate;
	m_dblInputTargetVelY = (target_data.Y - pre_target_data.Y)/m_dblDataSamplingRate;
	m_dblInputTargetVelZ = (target_data.Z - pre_target_data.Z)/m_dblDataSamplingRate;
	m_dblInputTargetRange = target_data.R;
	m_dblInputTargetBearing = target_data.B *(180./3.141592);
	m_dblInputTargetElevation = target_data.E *(180./3.141592);

	m_dblFilterOutputPosX = m_dblXHat[0];
	m_dblFilterOutputPosY = m_dblXHat[1];
	m_dblFilterOutputPosZ = m_dblXHat[2];
	m_dblFilterOutputVelX = m_dblXHat[3];
	m_dblFilterOutputVelY = m_dblXHat[4];
	m_dblFilterOutputVelZ = m_dblXHat[5];
	m_dblSimulationCurrentTime = target_data.Time;

	//	UpdateData(FALSE);
}

void CSKF::SKF_PredictionX()
{
	// mode #1
	m_cMat.multi_99_91(m_dblXBar, m_dblSystemMatrix, m_dblXHat);

}

void CSKF::SKF_PredictionP()
{
	double dblMat1[STATES][STATES], dblMat2[STATES][STATES], dblTrans[STATES][STATES];

	// mode #1
	m_cMat.transpose_99(dblTrans, m_dblSystemMatrix);
	m_cMat.multi_99_99(dblMat1, m_dblPHat, dblTrans);
	m_cMat.multi_99_99(dblMat2, m_dblSystemMatrix, dblMat1);
	m_cMat.addition_99(m_dblPBar, dblMat2, m_dblQ);
}


void CSKF::SKF_RHSetting()
{
	double dblRange, dblPsi, dblTheta;
	double dblJacobian[3][3] = {0.,}, dblJacobianTrans[3][3] = {0.,}, dblTempMatrix[3][3] = {0.,};

	// filter #1
	m_cAxisTransform.Cart2Sph(m_dblXBar[0], m_dblXBar[1], m_dblXBar[2], dblRange, dblTheta, dblPsi);

	dblJacobian[0][0] = sin(dblTheta)*cos(dblPsi);
	dblJacobian[0][1] = dblRange*cos(dblTheta)*cos(dblPsi);
	dblJacobian[0][2] = -dblRange*sin(dblTheta)*sin(dblPsi);
	dblJacobian[1][0] = cos(dblTheta)*cos(dblPsi);
	dblJacobian[1][1] = -dblRange*sin(dblTheta)*cos(dblPsi);
	dblJacobian[1][2] = -dblRange*cos(dblTheta)*sin(dblPsi);
	dblJacobian[2][0] = sin(dblPsi);
	dblJacobian[2][1] = 0.0;
	dblJacobian[2][2] = dblRange*cos(dblPsi);

	m_cMat.transpose_33(dblJacobianTrans, dblJacobian);
	m_cMat.multi_33_33(dblTempMatrix, m_dblRPol, dblJacobianTrans);
	m_cMat.multi_33_33(m_dblR, dblJacobian, dblTempMatrix);
}

void CSKF::SKF_ResidualCV()
{
	int i, j;
	double dblHP[3][STATES], dblHPH[3][3], dblTrans[STATES][3];

	// filter #1
	/// m_dblRS = H*m_dblPBar*TransH + m_dblR
	m_cMat.transpose_39( dblTrans, m_dblH );
	m_cMat.multi_39_99( dblHP, m_dblH, m_dblPBar );
	m_cMat.multi_39_93( dblHPH, dblHP, dblTrans );

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			m_dblRS[i][j] = dblHPH[i][j] + m_dblR[i][j];
		}
	}
}

void CSKF::SKF_SetKalmanFilterTargetData(TargetData stAbsNoiseTargetData)
{
	m_dblzRBE[0] = stAbsNoiseTargetData.R;
	m_dblzRBE[1] = stAbsNoiseTargetData.B;
	m_dblzRBE[2] = stAbsNoiseTargetData.E;
}

void CSKF::SKF_Residual()
{
	int i;
	double m_dblzXYZ[3];

	m_cAxisTransform.Sph2Cart(m_dblzRBE[0], m_dblzRBE[1], m_dblzRBE[2], m_dblzXYZ[0], m_dblzXYZ[1], m_dblzXYZ[2]);

	// CV Model
	for(i=0; i<3; i++)
	{
		m_dblResidual[i] = m_dblzXYZ[i] - m_dblXBar[i];
	}
}

void CSKF::SKF_KalmanGain()
{
	double dblDetS = 0.;
	double dblPH[STATES][3] = {{0.,},}, dblInv33[3][3] = {{0.,},}, dblInv22[2][2] = {{0.,},}, dblTrans[STATES][3] = {{0.,},};
	double dblTmpRS[2][2] = {{0.,},};

	// CV Model
	// m_dblK = m_dblPBar * TransH * InverseS
	m_cMat.transpose_39( dblTrans, m_dblH );
	m_cMat.multi_99_93( dblPH, m_dblPBar, dblTrans);
//*/
 	dblDetS =	 m_dblRS[0][0]*(m_dblRS[1][1]*m_dblRS[2][2]-m_dblRS[1][2]*m_dblRS[2][1])
				-m_dblRS[0][1]*(m_dblRS[1][0]*m_dblRS[2][2]-m_dblRS[1][2]*m_dblRS[2][0])
				+m_dblRS[0][2]*(m_dblRS[1][0]*m_dblRS[2][1]-m_dblRS[1][1]*m_dblRS[2][0]);

	if(fabs(dblDetS) < 0.5)
		dblDetS = 0.5;

	dblInv33[0][0] = (1./dblDetS) *(m_dblRS[1][1]*m_dblRS[2][2]-m_dblRS[1][2]*m_dblRS[2][1]);
	dblInv33[0][1] = (-1./dblDetS)*(m_dblRS[0][1]*m_dblRS[2][2]-m_dblRS[0][2]*m_dblRS[2][1]);
	dblInv33[0][2] = (1./dblDetS) *(m_dblRS[0][1]*m_dblRS[1][2]-m_dblRS[0][2]*m_dblRS[1][1]);
	dblInv33[1][0] = (-1./dblDetS)*(m_dblRS[1][0]*m_dblRS[2][2]-m_dblRS[1][2]*m_dblRS[2][0]);
	dblInv33[1][1] = (1./dblDetS) *(m_dblRS[0][0]*m_dblRS[2][2]-m_dblRS[0][2]*m_dblRS[2][0]);
	dblInv33[1][2] = (-1./dblDetS)*(m_dblRS[0][0]*m_dblRS[1][2]-m_dblRS[0][2]*m_dblRS[1][0]);
	dblInv33[2][0] = (1./dblDetS) *(m_dblRS[1][0]*m_dblRS[2][1]-m_dblRS[1][1]*m_dblRS[2][0]);
	dblInv33[2][1] = (-1./dblDetS)*(m_dblRS[0][0]*m_dblRS[2][1]-m_dblRS[0][1]*m_dblRS[2][0]);
	dblInv33[2][2] = (1./dblDetS) *(m_dblRS[0][0]*m_dblRS[1][1]-m_dblRS[0][1]*m_dblRS[1][0]);

	m_cMat.multi_93_33(m_dblK, dblPH, dblInv33 );
}

void CSKF::SKF_UpdateX()
{
	int i;
	double dblKZ[STATES];

	m_cMat.multi_93_31( dblKZ, m_dblK, m_dblResidual);


	for(i=0; i<STATES; i++)
	{
		m_dblXHat[i] = m_dblXBar[i] + dblKZ[i];

	}
}

void CSKF::SKF_UpdateP()
{
	int i, j;
	double m_dblI[STATES][STATES];
	double dblKH[STATES][STATES];
	double dblIKH[STATES][STATES];

	m_cMat.multi_93_39(dblKH, m_dblK, m_dblH);

	for(i=0; i<STATES; i++)
	{
		for(j=0; j<STATES; j++)
		{
			dblKH[i][j] = -1.*dblKH[i][j];
			if (i != j)
			{
				m_dblI[i][j] = 0.0;
			}
			else
			{
				m_dblI[i][j] = 1.0;
			}
		}
	}

	m_cMat.addition_99( dblIKH, m_dblI, dblKH);
	m_cMat.multi_99_99( m_dblPHat, dblIKH, m_dblPBar);
}

void CSKF::SetSyncData(const double sync_dblXHat[9], const double sync_dblPHat[9][9])
{
	memcpy(m_dblXHat, sync_dblXHat, sizeof(m_dblXHat));		
	memcpy(m_dblPHat, sync_dblPHat, sizeof(m_dblPHat));
}

void CSKF::GetSyncData(double sync_dblXHat[9], double sync_dblPHat[9][9])
{
	memcpy(sync_dblXHat, m_dblXHat, sizeof(m_dblXHat));		
	memcpy(sync_dblPHat, m_dblPHat, sizeof(m_dblPHat));
}