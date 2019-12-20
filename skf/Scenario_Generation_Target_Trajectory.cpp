// Scenario_Generation_Target_Trajectory.cpp: implementation of the Scenario_Generation_Target_Trajectory class.
//
//////////////////////////////////////////////////////////////////////
#include "Scenario_Generation_Target_Trajectory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Scenario_Generation_Target_Trajectory::Scenario_Generation_Target_Trajectory() 
{

}

Scenario_Generation_Target_Trajectory::~Scenario_Generation_Target_Trajectory()
{

}

void Scenario_Generation_Target_Trajectory::Scenario_Input_Target_Trajectory()
{
	
	cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■ " <<endl;
	cout << " ■              표적  시나리오 설정                   ■ " <<endl; 
	cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■ " <<endl;
	
	cout << " 0. 표적구분 대함표적 '1' , 대공표적 '2' : " ;
	cin >> OUTPUT_m_TARGET_IDENTIFICATION ;
		
	switch(OUTPUT_m_TARGET_IDENTIFICATION)
	{
	
	case 1:
		cout << " 1. 표적의 위치 East North[m]" ;
		cin  >> m_Target_input_Position[0] >> m_Target_input_Position[1] ;
		m_Target_input_Position[2] = 0. ;
		cout <<endl;
		
		cout << " 2. 표적의 속도 [m/sec] " ;
		cin  >> m_TVELI ;
		cout <<endl;

		cout << " 3. 표적의 초기자세 Yaw [degree]" ;
		cin  >> m_Target_input_EulerAngle[2] ;
		m_Target_input_EulerAngle[0] = 0. ;
		m_Target_input_EulerAngle[1] = 0. ;
		cout <<endl;
			
		cout << " 4. 표적모델 가속도 운동 모델(원운동 포함) '3' or 등속도 운동 모델 '1' " ;
		cin  >> OUTPUT_m_Target_Model ;
		cout <<endl;

		if ( OUTPUT_m_Target_Model == 1 )
		{
			m_TACCI         = 0. ;
			m_TBETAI        = 0. ;
			m_TEPSILI       = 0. ;
		}

		else
		{
			cout << " 5. 표적의 가속도 [m/sec^2] " ;
			cin  >>m_TACCI;
			cout <<endl;
			
			cout << " 6. 표적의 가속도 방향 설정 Beta[degree]"<<endl;
			cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"<<endl;
			cout << " ■ Beta = +-90 이면 등속 원운동                                                     ■"<<endl;
			cout << " ■ Beta = 0. or +-180 이면 가감속 운동                                              ■"<<endl;
			cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"<<endl;
			cin  >> m_TBETAI ;
			m_TEPSILI = 0. ;
			cout <<endl;
		}
		break;
		
	case 2:
		cout << " 1. 표적의 위치 East Norht UP[m]" ;
		cin  >> m_Target_input_Position[0] >> m_Target_input_Position[1] >> m_Target_input_Position[2] ;
		cout <<endl;
		
		cout << " 2. 표적의 속도 [m/sec] " ;
		cin  >> m_TVELI ;
		cout <<endl;
		
		cout << " 3. 표적의 초기자세 roll, pitch, Yaw [degree]" ;
		cin  >> m_Target_input_EulerAngle[0] >> m_Target_input_EulerAngle[1] >> m_Target_input_EulerAngle[2];
		m_Target_input_EulerAngle[0] = m_Target_input_EulerAngle[0] * 0. ;
		cout <<endl;

		cout << " 4. 표적모델 가속도 운동 모델(coordinated turn 포함) '3' or 등속도 운동 모델 '1' " ;
		cin  >> OUTPUT_m_Target_Model ;
		cout <<endl;
		
		if ( OUTPUT_m_Target_Model == 1 )
		{
			m_TACCI         = 0. ;
			m_TBETAI        = 0. ;
			m_TEPSILI       = 0. ;
		}

		else
		{
			cout << " 5. 표적의 가속도 [m/sec^2] " ;
			cin  >>m_TACCI;
			cout <<endl;
			
			cout << " 6. 표적의 가속도 방향 설정 Beta, Epsil[degree]"<<endl;
			cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"<<endl;
			cout << " ■ Beta = +-90 이면 등속 원운동                                                     ■"<<endl;
			cout << " ■ Beta = 0. or +-180 이면 가감속 운동                                              ■"<<endl;
			cout << " ■ 이외의 값을 입력시 원운동과 가감속 운동을 동시에 실시한다.                       ■"<<endl;
			cout << " ■ Epsil은 현재 속도방향에 대해서 가속도 방향을 위,아래로 줄 수 있는 값을 나타낸다. ■"<<endl;
			cout << " ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"<<endl;
			cin  >> m_TBETAI >> m_TEPSILI ;
			cout <<endl;
		}
		break;
	
	default:
		break;
	}
	
}

void Scenario_Generation_Target_Trajectory::InputScenarioParam(double dPosNorth, double dPosEast, double dPosUp,
															   double dVel,
															   double dRoll, double dPitch, double dYaw,
															   double dAcc, double dBeta, double dEpsil)
{	
	// 1. 표적의 위치 East Norht UP[m]	
	m_Target_input_Position[0]=dPosNorth;
	m_Target_input_Position[1]=dPosEast;
	m_Target_input_Position[2]=-1.*dPosUp;
	
	// 2. 표적의 속도 [m/sec]
	m_TVELI = dVel;
	
	// 3. 표적의 초기자세 roll, pitch, Yaw [degree]
	m_Target_input_EulerAngle[0]=dRoll;
	m_Target_input_EulerAngle[1]=dPitch;
	m_Target_input_EulerAngle[2]=dYaw;
	//m_Target_input_EulerAngle[0] = m_Target_input_EulerAngle[0] * 0. ;
	
	// 4. 표적의 가속도 [m/sec^2] 
	m_TACCI = dAcc;
	
	// 5. 표적의 가속도 방향 설정 Beta, Epsil[degree]"<<endl;
	m_TBETAI =dBeta;
	m_TEPSILI =dEpsil;
}

void Scenario_Generation_Target_Trajectory::runge_kutta_Target()
{
	int i ;
	double RKT[4][g_TA] = {0.,} ;
	double t_d[g_TA]    = {0.,} ;

	for( i = 0 ; i < g_TA ; i++ )
	{
		t_d[i] = m_T[i] ;
	}

	derivative_Target();

	for( i = 0 ; i < g_TA ; i++ )
	{
		RKT[0][i] = m_dt[i] ;  //전단계 미분계산값 갱신
		m_T[i] = t_d[i] + 0.5 * m_stt * RKT[0][i] ;
	}
	
	OUTPUT_m_TOF_T += 0.5 * m_stt ;

	derivative_Target();

	for( i = 0 ; i < g_TA ; i++ )
	{
		RKT[1][i] = m_dt[i] ;  //전단계 미분계산값 갱신
		m_T[i] = t_d[i] + 0.5 * m_stt * RKT[1][i] ;
	}

	derivative_Target();

	for( i = 0 ; i < g_TA ; i++ )
	{
		RKT[2][i] = m_dt[i] ;  //전단계 미분계산값 갱신
		m_T[i] = t_d[i] + m_stt * RKT[2][i] ;
	}
	
	OUTPUT_m_TOF_T += 0.5 * m_stt ;
	
	derivative_Target();

	for( i = 0 ; i < g_TA ; i++ )
	{
		RKT[3][i] = m_dt[i] ;  //전단계 미분계산값 갱신
		m_T[i] = t_d[i] + (m_stt * 0.1666666667)*( RKT[0][i] + 2. * RKT[1][i] + 2. * RKT[2][i] + RKT[3][i] ) ;		
	}

	for(i=0 ; i<3 ; i++)
	{
		OUTPUT_m_Target_Velocity_LI[i]     = m_T[i]    ;
		OUTPUT_m_Target_Position_LI[i]     = m_T[i+3]  ;
		OUTPUT_m_Target_EulerAngle_LI[i]   = m_T[i+6]  ;
		OUTPUT_m_Target_Acceleration_LI[i] = m_dt[i]   ;		
	}

	if(sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) == 0.)
		OUTPUT_m_TOMEGA = 0. ;
	else
		OUTPUT_m_TOMEGA = sqrt(m_Acceleration_Target_Body[1]*m_Acceleration_Target_Body[1]+m_Acceleration_Target_Body[2]*m_Acceleration_Target_Body[2])
		                / sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) ;	
}

void Scenario_Generation_Target_Trajectory::derivative_Target()
{

	double p, q, r ;
	
	m_Acceleration_Target_Body[0] = OUTPUT_m_TACC * cos(OUTPUT_m_TEPSIL) * cos(OUTPUT_m_TBETA);
	m_Acceleration_Target_Body[1] = OUTPUT_m_TACC * cos(OUTPUT_m_TEPSIL) * sin(OUTPUT_m_TBETA);
	m_Acceleration_Target_Body[2] = -1.*OUTPUT_m_TACC	 * sin(OUTPUT_m_TEPSIL)					;		
	
	if (OUTPUT_m_Target_Model >= 3) {
		m_Acceleration_Target_Body[0] =  m_Missile_ACC[0];
		m_Acceleration_Target_Body[1] =  m_Missile_ACC[1];
		m_Acceleration_Target_Body[2] =  m_Missile_ACC[2];
	}

	p =  0. ;
	
	if(sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) == 0.)
	{
		q = 0. ;
		r = 0. ;
	}
	else
	{
		q = -m_Acceleration_Target_Body[2] / sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) ;
		r =  m_Acceleration_Target_Body[1] / sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) ;
	}

	m_dt[0] = m_Acceleration_Target_Body[0] * ( cos(m_T[7])*cos(m_T[8]) ) + 
		      m_Acceleration_Target_Body[1] * ( sin(m_T[6])*sin(m_T[7])*cos(m_T[8])-1.*cos(m_T[6])*sin(m_T[8]) ) +
			  m_Acceleration_Target_Body[2] * ( cos(m_T[6])*sin(m_T[7])*cos(m_T[8])+sin(m_T[6])*sin(m_T[8]) ) ;

	m_dt[1] = m_Acceleration_Target_Body[0] * ( cos(m_T[7])*sin(m_T[8]) ) + 
		      m_Acceleration_Target_Body[1] * ( sin(m_T[6])*sin(m_T[7])*sin(m_T[8])+cos(m_T[6])*cos(m_T[8]) ) +
			  m_Acceleration_Target_Body[2] * ( cos(m_T[6])*sin(m_T[7])*sin(m_T[8])-1.*sin(m_T[6])*cos(m_T[8]) ) ;
	
	m_dt[2] = m_Acceleration_Target_Body[0] * ( -1.* sin(m_T[7]) ) + 
		      m_Acceleration_Target_Body[1] * ( sin(m_T[6])*cos(m_T[7]) ) +
			  m_Acceleration_Target_Body[2] * ( cos(m_T[6])*cos(m_T[7]) ) ;
	
	m_dt[3] = m_T[0] ;
	
	m_dt[4] = m_T[1] ;
	
	m_dt[5] = m_T[2] ;
		
	m_dt[6] = p + q*sin(m_T[6])*tan(m_T[7]) + r*cos(m_T[6])*tan(m_T[7]);
	
	m_dt[7] = q*cos(m_T[6]) - r*sin(m_T[6]);							
	
	m_dt[8] = q*sin(m_T[6])/cos(m_T[7]) + r*cos(m_T[6])/cos(m_T[7]);

}

void Scenario_Generation_Target_Trajectory::Scenario_Initialize_Target_Trajectory(int nTracker)
{
	int i ;
	
	for(i=0 ; i < g_TA ; i++)
	{
		m_T[i]   =0.;
		m_dt[i]  =0.;
	}

	m_Missile_ACC[0] = 0.0 ;
	m_Missile_ACC[1] = 0.0 ;
	m_Missile_ACC[2] = 0.0 ;
	
	if(nTracker==0)	//EOTS
		m_stt = 0.1 ;
	else if(nTracker==1) //TWS
		m_stt = 2.0 ;	
	else //Simulation for PHP predict
		m_stt = 0.01;
	
	OUTPUT_m_TOF_T = 0.    ;	

	OUTPUT_m_TVEL   = m_TVELI ;
	//Edited by EJ KIM in ADD
	UpdateTargetTrajectory(m_TACCI,m_TBETAI,m_TEPSILI);
	/*OUTPUT_m_TACC   = m_TACCI ;
	OUTPUT_m_TBETA  = m_TBETAI  * 0.01745329252 ;
	OUTPUT_m_TEPSIL = m_TEPSILI * 0.01745329252 ;*/
		
	m_T[3] = m_Target_input_Position[0];
	m_T[4] = m_Target_input_Position[1];
	m_T[5] = m_Target_input_Position[2];

	m_T[6] = m_Target_input_EulerAngle[0] * 0.01745329252 ;
	m_T[7] = m_Target_input_EulerAngle[1] * 0.01745329252 ;
	m_T[8] = m_Target_input_EulerAngle[2] * 0.01745329252 ;

	m_T[0] =		m_TVELI * cos(m_T[7]) * cos(m_T[8]);
	m_T[1] =		m_TVELI * cos(m_T[7]) * sin(m_T[8]);
	m_T[2] =  -1. * m_TVELI * sin(m_T[7])              ;		

	for(i=0 ; i<3 ; i++)
	{
		OUTPUT_m_Target_Velocity_LI[i]     = m_T[i]    ;
		OUTPUT_m_Target_Position_LI[i]     = m_T[i+3]  ;
		OUTPUT_m_Target_EulerAngle_LI[i]   = m_T[i+6]  ;
		OUTPUT_m_Target_Acceleration_LI[i] = m_dt[i]   ;		
	}

	if(sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) == 0.)
		OUTPUT_m_TOMEGA = 0. ;
	else
		OUTPUT_m_TOMEGA = sqrt(m_Acceleration_Target_Body[1]*m_Acceleration_Target_Body[1]+m_Acceleration_Target_Body[2]*m_Acceleration_Target_Body[2])
		                / sqrt(m_T[0]*m_T[0]+m_T[1]*m_T[1]+m_T[2]*m_T[2]) ;
}

void Scenario_Generation_Target_Trajectory::UpdateTargetTrajectory(double dAcc, double dBeta, double dEpsil)
{	
	//Edited by EJ KIM in ADD
	OUTPUT_m_TACC   = dAcc ;
	OUTPUT_m_TBETA  = dBeta  * 0.01745329252;	
	OUTPUT_m_TEPSIL = dEpsil * 0.01745329252;
	
}

void Scenario_Generation_Target_Trajectory::PNG_ACC_CALCULATION(double Ownship_Pos[3], double Ownship_Vel[3])
{
	double RMT_LI[3]    = {0., };
	double RMT_MBODY[3] = {0., };
	double RMT_LOS[3]   = {0., };

	double VMT_LI[3]  = {0., };
	double VMT_LOS[3] = {0., };
	double V_LOS[3]   = {0., };
	
	double T_MISSILE_B_LI[3][3] = {0.0,};
	double T_MISSILE_LI_B[3][3] = {0.0,};
	double T_MISSILE_LOS_B[3][3] = {0.0,};
	double T_MISSILE_B_LOS[3][3] = {0.0,};
	double T_MISSILE_LOS_LI[3][3] = {0.0,};
	double T_MISSILE_LI_LOS[3][3] = {0.0,};

	double Theta_g = 0. , Pusi_g = 0. ;
	double LOS_Rate[3] = {0.,};

	double Navigation_constant = 3.0 ;

	int i = 0;

	for (i=0;i<3;i++) {
		RMT_LI[i] = Ownship_Pos[i] - OUTPUT_m_Target_Position_LI[i];
	}

	for (i=0;i<3;i++) {
		VMT_LI[i] = Ownship_Vel[i] - OUTPUT_m_Target_Velocity_LI[i];
	}

	//■■■  Body -> Li ■■■ //
	T_MISSILE_LI_B[0][0] = cos(m_T[7])*cos(m_T[8]);
	T_MISSILE_LI_B[0][1] = sin(m_T[6])*sin(m_T[7])*cos(m_T[8])-cos(m_T[6])*sin(m_T[8]);
	T_MISSILE_LI_B[0][2] = cos(m_T[6])*sin(m_T[7])*cos(m_T[8])+sin(m_T[6])*sin(m_T[8]);

	T_MISSILE_LI_B[1][0] = cos(m_T[7])*sin(m_T[8]);
	T_MISSILE_LI_B[1][1] = sin(m_T[6])*sin(m_T[7])*sin(m_T[8])+cos(m_T[6])*cos(m_T[8]);
	T_MISSILE_LI_B[1][2] = cos(m_T[6])*sin(m_T[7])*sin(m_T[8])-sin(m_T[6])*cos(m_T[8]);

	T_MISSILE_LI_B[2][0] = -1.*sin(m_T[7]);
	T_MISSILE_LI_B[2][1] = sin(m_T[6])*cos(m_T[7]);
	T_MISSILE_LI_B[2][2] = cos(m_T[6])*cos(m_T[7]);

	transpose_33(T_MISSILE_B_LI,T_MISSILE_LI_B) ;
	multi_33_31(RMT_MBODY,T_MISSILE_B_LI,RMT_LI);

	Pusi_g	= atan2(RMT_MBODY[1], RMT_MBODY[0]);
	Theta_g = atan2(-1.*RMT_MBODY[2], (sqrt(pow(RMT_MBODY[0],2.0) + pow(RMT_MBODY[1],2.0))));

	T_MISSILE_LOS_B[0][0] =  cos(Theta_g)*cos(Pusi_g);
	T_MISSILE_LOS_B[0][1] =  cos(Theta_g)*sin(Pusi_g);
	T_MISSILE_LOS_B[0][2] = -1.*sin(Theta_g)		 ;

	T_MISSILE_LOS_B[1][0] = -1.*sin(Pusi_g)			 ;
	T_MISSILE_LOS_B[1][1] =  cos(Pusi_g)			 ;
	T_MISSILE_LOS_B[1][2] =  0.					 ;

	T_MISSILE_LOS_B[2][0] =  sin(Theta_g)*cos(Pusi_g);
	T_MISSILE_LOS_B[2][1] =  sin(Theta_g)*sin(Pusi_g);
	T_MISSILE_LOS_B[2][2] =  cos(Theta_g)			 ;

	transpose_33(T_MISSILE_B_LOS,T_MISSILE_LOS_B) ;
	multi_33_33(T_MISSILE_LOS_LI,T_MISSILE_LOS_B,T_MISSILE_B_LI);
	transpose_33(T_MISSILE_LI_LOS,T_MISSILE_LOS_LI) ;

	multi_33_31(RMT_LOS,T_MISSILE_LOS_LI,RMT_LI);	
	multi_33_31(VMT_LOS,T_MISSILE_LOS_LI,VMT_LI);

	for (i=0;i<3;i++) {
		V_LOS[i] = -1.0*VMT_LOS[i];
	}

	LOS_Rate[0] = (RMT_LOS[2]*V_LOS[1] - RMT_LOS[1]*V_LOS[2])/(pow(RMT_LOS[0], 2.0) + pow(RMT_LOS[1], 2.0) + pow(RMT_LOS[2], 2.0));
	LOS_Rate[1] = (RMT_LOS[0]*V_LOS[2] - RMT_LOS[2]*V_LOS[0])/(pow(RMT_LOS[0], 2.0) + pow(RMT_LOS[1], 2.0) + pow(RMT_LOS[2], 2.0));
	LOS_Rate[2] = (RMT_LOS[1]*V_LOS[0] - RMT_LOS[0]*V_LOS[1])/(pow(RMT_LOS[0], 2.0) + pow(RMT_LOS[1], 2.0) + pow(RMT_LOS[2], 2.0));

	m_Missile_ACC[0] = 0.0 ;
	m_Missile_ACC[1] = Navigation_constant * V_LOS[0] * LOS_Rate[2] / cos(Pusi_g);
	m_Missile_ACC[2] = -1.0 * Navigation_constant * V_LOS[0] * LOS_Rate[1] / cos(Theta_g);

	if (fabs(m_Missile_ACC[1]) >= 50.0) {
		m_Missile_ACC[1] = 50.0 * m_Missile_ACC[1]/fabs(m_Missile_ACC[1]) ;
	}
	if (fabs(m_Missile_ACC[0]) >= 50.0) {
		m_Missile_ACC[0] = 50.0 * m_Missile_ACC[0]/fabs(m_Missile_ACC[0]) ;
	}
}
void Scenario_Generation_Target_Trajectory:: multi_33_33(double out_mat[3][3], double multi_one[3][3],  double multi_two[3][3])
{
	int i, j, k;
	
	for(j=0;j<3;j++)
		for(k=0;k<3;k++)
		{
			out_mat[j][k]=0;
			for(i=0;i<3;i++)
				out_mat[j][k] += multi_one[j][i] * multi_two[i][k];
		}
}

void Scenario_Generation_Target_Trajectory:: transpose_33(double out_mat[3][3],double transfer_in[3][3])
{
	int i,j;
	
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
		{
			out_mat[i][j]=0.0;
			out_mat[i][j]=transfer_in[j][i];
		}
	
}

void Scenario_Generation_Target_Trajectory:: multi_33_31( double out_mat[3], double multi_one[3][3],double multi_two[3] )
{
	int i, j;

	for(j=0;j<3;j++)
	{
		out_mat[j]=0;
		for(i=0;i<3;i++)
			out_mat[j] += multi_one[j][i] * multi_two[i];			
	}
}

void Scenario_Generation_Target_Trajectory::Pure_Pursuit_ACC_CALCULATION(double Ownship_Pos[3], double Ownship_Vel[3])
{
	double RMT_LI[3]    = {0., };
	
	double V_M = 0. ;
	
	double Theta_Los = 0., Pusi_Los = 0. ;


	int i = 0;

	for (i=0;i<3;i++) {
		RMT_LI[i] = Ownship_Pos[i] - OUTPUT_m_Target_Position_LI[i];
		V_M = V_M + pow(OUTPUT_m_Target_Velocity_LI[i],2.);
	}

	V_M = sqrt(V_M);

	Theta_Los = atan2(-1.*RMT_LI[2], (sqrt(pow(RMT_LI[0],2.0) + pow(RMT_LI[1],2.0))));
	Pusi_Los  = atan2(RMT_LI[1],RMT_LI[0]);

	m_Missile_ACC[0] = 0.0 ;
	m_Missile_ACC[1] =   10.0 * V_M * (Pusi_Los -m_T[8]); // 8보다 크게 잡을 것
	m_Missile_ACC[2] =  -1.0 * V_M * (Theta_Los-m_T[7]);

	if (fabs(m_Missile_ACC[1]) >= 50.0) {
		m_Missile_ACC[1] = 50.0 * m_Missile_ACC[1]/fabs(m_Missile_ACC[1]) ;
	}
	if (fabs(m_Missile_ACC[0]) >= 50.0) {
		m_Missile_ACC[0] = 50.0 * m_Missile_ACC[0]/fabs(m_Missile_ACC[0]) ;
	}
}