// Scenario_Generation_Target_Trajectory.h: interface for the Scenario_Generation_Target_Trajectory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENARIO_GENERATION_TARGET_TRAJECTORY_H__F99ED9B4_3D75_474D_80BC_9C7A218C2564__INCLUDED_)
#define AFX_SCENARIO_GENERATION_TARGET_TRAJECTORY_H__F99ED9B4_3D75_474D_80BC_9C7A218C2564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <Math.h>

using namespace std;

const int g_TA = 9; // 표적의 상태변수 개수

class Scenario_Generation_Target_Trajectory  
{

private:	

	double m_TACCI	       ;
	double m_TBETAI		   ;
	double m_TEPSILI       ;
	double m_TVELI		   ;
	double m_T[g_TA]         ;
	double m_dt[g_TA]        ;
	double m_Acceleration_Target_Body[3] ;
	double m_Target_input_EulerAngle[3]  ;

	void derivative_Target(void);
	void multi_33_31(double out_mat[3], double multi_one[3][3],double multi_two[3]);
	void multi_33_33(double out_mat[3][3], double multi_one[3][3],  double multi_two[3][3]);
	void transpose_33(double out_mat[3][3],double transfer_in[3][3]);
	
public:
		double m_stt           ;
	Scenario_Generation_Target_Trajectory();
	virtual ~Scenario_Generation_Target_Trajectory();
	void PNG_ACC_CALCULATION(double Ownship_Pos[3], double Ownship_Vel[3]);
	void Pure_Pursuit_ACC_CALCULATION(double Ownship_Pos[3], double Ownship_Vel[3]);

	
	double m_Target_input_Position[3]           ;
	int    OUTPUT_m_Target_Model	            ;
	int    OUTPUT_m_TARGET_IDENTIFICATION       ;
	double OUTPUT_m_TOMEGA		                ;
	double OUTPUT_m_TACC	                    ;
	double OUTPUT_m_TBETA		                ;
	double OUTPUT_m_TEPSIL		                ;
	double OUTPUT_m_TVEL		                ;
	double OUTPUT_m_TOF_T		                ;
	double OUTPUT_m_Target_Acceleration_LI[3]	;
	double OUTPUT_m_Target_Velocity_LI[3]       ;
	double OUTPUT_m_Target_Position_LI[3]	    ;
	double OUTPUT_m_Target_EulerAngle_LI[3]	    ;
	double m_Missile_ACC[3]  ;

	void Scenario_Input_Target_Trajectory(void)      ;
	void Scenario_Initialize_Target_Trajectory(int nTracker) ;//nTracker : 0=EOTS 1=TWS
	void UpdateTargetTrajectory(double dAcc, double dBeta, double dEpsil);
	void runge_kutta_Target(void)                    ;
	void InputScenarioParam(double dPosNorth, double dPosEast, double dPosUp,
							double dVel,
							double dRoll, double dPitch, double dYaw,
							double dAcc, double dBeta, double dEpsil);
};

#endif // !defined(AFX_SCENARIO_GENERATION_TARGET_TRAJECTORY_H__F99ED9B4_3D75_474D_80BC_9C7A218C2564__INCLUDED_)
