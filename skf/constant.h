#ifndef _CONSTANT_
#define _CONSTANT_

#include <math.h>
#ifdef WIN32
	#include  "float.h"	
#else
	#include "private/mathP.h"
#endif

//회전축
#define PITCH			1
#define ROLL			2
#define YAW				3
#define X_AXIS			4
#define Y_AXIS			5
#define Z_AXIS			6
#define ALL_AXIS		7

// 단위 변환
#define PI 						3.1415926535897932384626
#define RAD2DEG					(180.0/PI)
#define DEG2RAD					(PI/180.0)		
#define	CONST_INITIAL_GRAVITY	9.80665			// 중력가속도
#define KT2MS					(1852./3600.)
#define GRAVITY2ACC				9.81

// 표적 성질
#define ENV_NONE 		0
#define ENV_AIR 		1
#define ENV_SURFACE 	2
#define ENV_NSFS 		3

// 센서 할당
#define EOTS_AIR 1
#define EOTS_SURFACE 2
#define TWS_SURFACE 3
#define TRS_AIR 4
#define TRS_SURFACE 5

// 다기능레이더 구역 할당
#define MFR_AIR			1
#define MFR_SURFACE		2

// 시나리오 생성 Sampling Rate
#define TRACKING				0
#define SCANING					1

// 시스템 다이나믹
#define CV_MODEL				0
#define SONG_MODEL				1
#define CTR_MODEL				2


// 측정치 생성용 Sigma
#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_EOTS_AIR				1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_EOTS_AIR			0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_EOTS_AIR			0.0002

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_EOTS_SURFACE			1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_EOTS_SURFACE		0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_EOTS_SURFACE		0.0

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_MFR_AIR				1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_MFR_AIR				0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_MFR_AIR			0.0002

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_MFR_SURFACE			1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_MFR_SURFACE			0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_MFR_SURFACE		0.0

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_TWS_SURFACE			4.0
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_TWS_SURFACE			0.003
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_TWS_SURFACE		0.0

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_TRS_AIR				1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_TRS_AIR				0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_TRS_AIR			0.0002

#define MEASUREMENT_NOISE_GEN_SIGMA_RANGE_TRS_SURFACE			1.5
#define MEASUREMENT_NOISE_GEN_SIGMA_BEARING_TRS_SURFACE			0.0002
#define MEASUREMENT_NOISE_GEN_SIGMA_ELEVATION_TRS_SURFACE		0.0002

// Measurement Noise 생성용 Sigma
#define	MEASUREMENT_NOISE_SIGMA_RANGE_EOTS_AIR					7.5		// deviation of Range		// 07.02.08 ADD 5->1.5 * 5
#define	MEASUREMENT_NOISE_SIGMA_BEARING_EOTS_AIR				0.001	// deviation of Bearing		// 07.02.08 ADD 0.0007->0.0002
#define	MEASUREMENT_NOISE_SIGMA_ELEVATION_EOTS_AIR				0.001	// deviation of Elevation	// 07.02.08 ADD 0.0007->0.0002

#define	MEASUREMENT_NOISE_SIGMA_RANGE_EOTS_SURFACE				7.5		// deviation of Range		// 07.02.08 ADD 5->1.5 * 5
#define	MEASUREMENT_NOISE_SIGMA_BEARING_EOTS_SURFACE			0.001	// deviation of Bearing		// 07.02.08 ADD 0.0007->0.0002
#define	MEASUREMENT_NOISE_SIGMA_ELEVATION_EOTS_SURFACE			0.0		// deviation of Elevation

#define	MEASUREMENT_NOISE_SIGMA_RANGE_MFR_AIR					7.5		// deviation of Range		
#define	MEASUREMENT_NOISE_SIGMA_BEARING_MFR_AIR					0.001	// deviation of Bearing		
#define	MEASUREMENT_NOISE_SIGMA_ELEVATION_MFR_AIR				0.001	// deviation of Elevation	

#define	MEASUREMENT_NOISE_SIGMA_RANGE_MFR_SURFACE				7.5		// deviation of Range		
#define	MEASUREMENT_NOISE_SIGMA_BEARING_MFR_SURFACE				0.001	// deviation of Bearing		
#define	MEASUREMENT_NOISE_SIGMA_ELEVATION_MFR_SURFACE			0.0		// deviation of Elevation

#define	MEASUREMENT_NOISE_SIGMA_RANGE_TWS_SURFACE				20.0	// deviation of Range		// 07.02.08 ADD 20->4 * 5
#define	MEASUREMENT_NOISE_SIGMA_BEARING_TWS_SURFACE				0.015	// deviation of Bearing		// 07.02.08 ADD 0.003->0.0012
#define	MEASUREMENT_NOISE_SIGMA_ELEVATION_TWS_SURFACE			0.0		// deviation of Elevation

#define MEASUREMENT_NOISE_SIGMA_RANGE_TRS_AIR					1.5
#define MEASUREMENT_NOISE_SIGMA_BEARING_TRS_AIR					0.0001
#define MEASUREMENT_NOISE_SIGMA_ELEVATION_TRS_AIR				0.0001

#define MEASUREMENT_NOISE_SIGMA_RANGE_TRS_SURFACE				1.5
#define MEASUREMENT_NOISE_SIGMA_BEARING_TRS_SURFACE				0.0001
#define MEASUREMENT_NOISE_SIGMA_ELEVATION_TRS_SURFACE			0.0001

// Prefilter - Process Noise 생성용 Sigma
#define	PROCESS_NOISE_PREFILTER_SIGMA_W_EOTS_AIR				4.0		// Prefilter Model Sigma
#define	PROCESS_NOISE_PREFILTER_TAU_EOTS_AIR					8.0		// Prefilter Correlation time

#define	PROCESS_NOISE_PREFILTER_SIGMA_W_EOTS_SURFACE			1.5		// Prefilter Model Sigma
#define	PROCESS_NOISE_PREFILTER_TAU_EOTS_SURFACE				5.0		// Prefilter Correlation time

#define	PROCESS_NOISE_PREFILTER_SIGMA_W_TWS_SURFACE				0.1		// Prefilter Model Sigma
#define	PROCESS_NOISE_PREFILTER_TAU_TWS_SURFACE					5.0		// Prefilter Correlation time

#define	PROCESS_NOISE_PREFILTER_SIGMA_W_TRS_AIR					4.0		// Prefilter Model Sigma
#define	PROCESS_NOISE_PREFILTER_TAU_TRS_AIR						8.0		// Prefilter Correlation time
	
#define	PROCESS_NOISE_PREFILTER_SIGMA_W_TRS_SURFACE				1.5		// Prefilter Model Sigma
#define	PROCESS_NOISE_PREFILTER_TAU_TRS_SURFACE					5.0		// Prefilter Correlation time

// Estimation Filter - Process Noise 생성용 Sigma
#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_EOTS_AIR					0.1		// CV model의 sigma_w,				// 0.1
#define	PROCESS_NOISE_CV_MODEL_TAU_EOTS_AIR						0.0		// CV model의 Tau
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_EOTS_AIR				1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_EOTS_AIR					0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_EOTS_AIR				10.0	// deviation of time acceleration	//10.0
#define	PROCESS_NOISE_SONG_MODEL_TAU_EOTS_AIR					2.0		// correlation time, Song model		// 2.0
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_EOTS_AIR				0.5		// CTR model의 sigma_wk				// 0.5
#define	PROCESS_NOISE_CTR_MODEL_TAU_EOTS_AIR					0.0		// CTR model의 tau				// 0.5
#define PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_EOTS_AIR			0.0

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_EOTS_SURFACE				0.1		// CV model의 sigma_w
#define	PROCESS_NOISE_CV_MODEL_TAU_EOTS_SURFACE					0.0		// CV model의 TAU
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_EOTS_SURFACE			1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_EOTS_SURFACE				0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_EOTS_SURFACE			1.0		// deviation of time acceleration, mode 2
#define	PROCESS_NOISE_SONG_MODEL_TAU_EOTS_SURFACE				2.0		// correlation time, Song model, mode 2
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_EOTS_SURFACE			0.3		// CTR model의 sigma_w
#define	PROCESS_NOISE_CTR_MODEL_TAU_EOTS_SURFACE				0.0		// CTR model의 TAU
#define	PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_EOTS_SURFACE		0.01

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_MFR_AIR					0.1		// CV model의 sigma_w,				// 0.1
#define	PROCESS_NOISE_CV_MODEL_TAU_MFR_AIR						0.0		// CV model의 Tau
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_MFR_AIR				1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_MFR_AIR					0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_MFR_AIR				10.0	// deviation of time acceleration	//10.0
#define	PROCESS_NOISE_SONG_MODEL_TAU_MFR_AIR					2.0		// correlation time, Song model		// 2.0
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_MFR_AIR					0.5		// CTR model의 sigma_wk				// 0.5
#define	PROCESS_NOISE_CTR_MODEL_TAU_MFR_AIR						0.0		// CTR model의 tau				// 0.5
#define PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_MFR_AIR			0.0

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_MFR_SURFACE				0.1		// CV model의 sigma_w
#define	PROCESS_NOISE_CV_MODEL_TAU_MFR_SURFACE					0.0		// CV model의 TAU
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_MFR_SURFACE			1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_MFR_SURFACE				0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_MFR_SURFACE			1.0		// deviation of time acceleration, mode 2
#define	PROCESS_NOISE_SONG_MODEL_TAU_MFR_SURFACE				2.0		// correlation time, Song model, mode 2
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_MFR_SURFACE				0.3		// CTR model의 sigma_w
#define	PROCESS_NOISE_CTR_MODEL_TAU_MFR_SURFACE					0.0		// CTR model의 TAU
#define	PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_MFR_SURFACE		0.01

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_TWS_SURFACE				0.01	// CV model의 sigma_w
#define	PROCESS_NOISE_CV_MODEL_TAU_TWS_SURFACE					0.0		// CV model의 tau
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_TWS_SURFACE			1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_TWS_SURFACE				0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_TWS_SURFACE			0.5		// deviation of time acceleration, mode 2
#define	PROCESS_NOISE_SONG_MODEL_TAU_TWS_SURFACE				2.0		// correlation time, Song model, mode 2
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_TWS_SURFACE				0.3		// CTR model의 sigma_w
#define	PROCESS_NOISE_CTR_MODEL_TAU_TWS_SURFACE					0.0		// CTR model의 tau
#define	PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_TWS_SURFACE		0.1

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_TRS_AIR					0.1		// CV model의 sigma_w,				// 0.1
#define	PROCESS_NOISE_CV_MODEL_TAU_TRS_AIR						0.0		// CV model의 Tau
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_TRS_AIR				1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_TRS_AIR					0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_TRS_AIR				10.0	// deviation of time acceleration	//10.0
#define	PROCESS_NOISE_SONG_MODEL_TAU_TRS_AIR					2.0		// correlation time, Song model		// 2.0
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_TRS_AIR					0.5		// CTR model의 sigma_wk				// 0.5
#define	PROCESS_NOISE_CTR_MODEL_TAU_TRS_AIR						0.0		// CTR model의 tau				// 0.5
#define PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_TRS_AIR			0.0

#define	PROCESS_NOISE_CV_MODEL_SIGMA_W_TRS_SURFACE				0.1		// CV model의 sigma_w
#define	PROCESS_NOISE_CV_MODEL_TAU_TRS_SURFACE					0.0		// CV model의 TAU
#define PROCESS_NOISE_SINGER_MODEL_SIGMA_W_TRS_SURFACE			1.0		// Singer model의 sigma_w
#define PROCESS_NOISE_SINGER_MODEL_TAU_TRS_SURFACE				0.2		// correlation time, Singer model
#define	PROCESS_NOISE_SONG_MODEL_SIGMA_W_TRS_SURFACE			1.0		// deviation of time acceleration, mode 2
#define	PROCESS_NOISE_SONG_MODEL_TAU_TRS_SURFACE				2.0		// correlation time, Song model, mode 2
#define	PROCESS_NOISE_CTR_MODEL_SIGMA_W_TRS_SURFACE				0.3		// CTR model의 sigma_w
#define	PROCESS_NOISE_CTR_MODEL_TAU_TRS_SURFACE					0.0		// CTR model의 TAU
#define	PROCESS_NOISE_CA_NOISE_MODEL_SMALL_Q_TRS_SURFACE		0.01


// Prefilter Window
#define PREFILTER_WINDOW_MEDIAN 3
#define PREFILTER_WINDOW_AVERAGE 3

// Prefilter Model - Tracking Filter Model
#define PREFILTER_MODEL_MEDIAN				1	// Prefilter - Median Filter
#define PREFILTER_MODEL_AVERAGE			2	// Prefilter - Average Filter

// Estimation Filter - Tracking Filter Model
#define	FILTER_CV_AIR							1
#define	FILTER_SINGER_AIR						2
#define FILTER_SONG_AIR							3
#define FILTER_CTR_AIR							4
#define FILTER_CA_AIR							5
#define FILTER_CA_NOISE_AIR						6
#define FILTER_CV_SURF							7
#define	FILTER_SINGER_SURF						8
#define FILTER_SONG_SURF						9
#define FILTER_CTR_SURF							10
#define FILTER_CA_SURF							11
#define FILTER_CA_NOISE_SURF					12



	




// Filter States
#define	AIR_STATES			9
#define	SURFACE_STATES		6
#define OWNSHIP_STATES		10

// PHP Calculation Method
#define	PHP_CALC_METHOD_MAX				1		// 1 : Max
#define	PHP_CALC_METHOD_COMBI			2		// 2 : Combination
#define	PHP_CALC_METHOD_SIGMOID			3		// 3 : Sigmoid

// Spec 상에서 존재하는 정해진 delay time
#define DELAY_TIME_SPEC_EOTS_AIR		0.1
#define DELAY_TIME_SPEC_EOTS_SURFACE	0.1
#define DELAY_TIME_SPEC_TWS_SURFACE		0.1
#define DELAY_TIME_SPEC_TRS_AIR			0.1
#define DELAY_TIME_SPEC_TRS_SURFACE		0.1
#define DELAY_TIME_SPEC_MFR_AIR			0.1
#define DELAY_TIME_SPEC_MFR_SURFACE		0.1



/*
// IMM Filter Mode
#define	FILTER_MODE1		0		// CV Model
#define	FILTER_MODE2		1		// Song Model
#define	FILTER_MODE3		2		// CTR Model

// FilterSet : 필터에 적용하는 Sigma 값의 배수
#define FILTER_EOTS_AIR_RANGE_SIGMA_MULTIPLE_CONSTANT		5.	// EOTS-Air Range Measurement Sigma의 배수
#define FILTER_EOTS_AIR_BEARING_SIGMA_MULTIPLE_CONSTANT		5.	// EOTS-Air Bearing Measurement Sigma의 배수
#define FILTER_EOTS_AIR_ELEVATION_SIGMA_MULTIPLE_CONSTANT	5.	// EOTS-Air Elevation Measurement Sigma의 배수

#define FILTER_EOTS_SURF_RANGE_SIGMA_MULTIPLE_CONSTANT		7.	// EOTS-Sur Range Measurement Sigma의 배수
#define FILTER_EOTS_SURF_BEARING_SIGMA_MULTIPLE_CONSTANT	7.	// EOTS-Sur Bearing Measurement Sigma의 배수
#define FILTER_EOTS_SURF_ELEVATION_SIGMA_MULTIPLE_CONSTANT	7.	// EOTS-Sur Elevation Measurement Sigma의 배수

#define FILTER_TWS_SURF_RANGE_SIGMA_MULTIPLE_CONSTANT		5.	// TWS-Sur Range Measurmenet Sigma의 배수
#define FILTER_TWS_SURF_BEARING_SIGMA_MULTIPLE_CONSTANT		10.	// TWS-Sur Bearing Measurmenet Sigma의 배수
#define FILTER_TWS_SURF_ELEVATION_SIGMA_MULTIPLE_CONSTANT	5.	// TWS-Sur Elevation Measurement Sigma의 배수

// PHP 계산에 Acc 를 적용시키기 위한 상한/하한 값
#define PHP_EOTS_AIR_ACC_START_LIMIT_BAND					5.
#define PHP_EOTS_AIR_ACC_END_LIMIT_BAND						1.
#define PHP_EOTS_SURFACE_ACC_START_LIMIT_BAND				0.3
#define PHP_EOTS_SURFACE_ACC_END_LIMIT_BAND					0.3
#define PHP_TWS_SURFACE_ACC_START_LIMIT_BAND				0.3
#define PHP_TWS_SURFACE_ACC_END_LIMIT_BAND					0.3

// Prefilter에서 EOTS Air/Surface 측정치의 Range Step을 판단하는 지표(Range Sigma)에 곱해질 배수
#define EOTS_AIR_RANGE_STEP_SAMPLE							10
#define EOTS_AIR_RANGE_STEP_CRITERIA_MULTIPLE_CONSTANT		3	// EOTS Air Range Step 판단 배수
#define EOTS_SURFACE_RANGE_STEP_SAMPLE						10
#define	EOTS_SURFACE_RANGE_STEP_CRITERIA_MULTIPLE_CONSTANT	10	// EOTS Surface Range Step 판단 배수
#define TWS_SURFACE_NORM_STEP_CRITERIA						200	// TWS Surface Norm Step 판단 지수




// PHP 초기 CV Model 제한 상수
#define	PHP_EOTS_AIR_CV_LIMIT_CONSTANT	10	// EOTS_Air PHP 계산시 초기 CV로 강제 설정하는 sample 수
#define	PHP_EOTS_SURF_CV_LIMIT_CONSTANT	10	// EOTS_Surf PHP 계산시 초기 CV로 강제 설정하는 sample 수
#define	PHP_TWS_SURF_CV_LIMIT_CONSTANT	10	// TWS_Surf PHP 계산시 초기 CV로 강제 설정하는 sample 수

// PHP MODE Change 적용 판단 상수
#define	PHP_EOTS_AIR_MODE_CHANGE_LIMIT_CONSTANT		3
#define	PHP_EOTS_SURF_MODE_CHANGE_LIMIT_CONSTANT	3
#define	PHP_TWS_SURF_MODE_CHANGE_LIMIT_CONSTANT		3

// PHP TRS Acceleration 적용 판단 상수
#define	PHP_TRS_AIR_ACC_APPLY_LIMIT_CONSTANT		3
#define	PHP_TRS_SURF_ACC_APPLY_LIMIT_CONSTANT		3


*/
// Measurement Type
#define IDEAL_MEASURE	1		// Noise 를 섞어야 하는 Ideal Trajectory
#define REAL_MEASURE	2		// Noise 가 섞여있다고 가정된 일종의 센싱된 궤적

#endif