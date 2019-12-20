// CAxisTransformations.h: interface for the CAxisTransformations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAXISTRANSFORMATIONS_H__35C66458_F4DB_48A7_9FC6_57E205C55D10__INCLUDED_)
#define AFX_CAXISTRANSFORMATIONS_H__35C66458_F4DB_48A7_9FC6_57E205C55D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBCUPosENU;
//***************************************************************************
// 파일명        : CAxisTransformations.h
// 프로젝트명    : PKX BCU - 76mm,40mm 축변환
// 최초 작성일   : 2003/12/05
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 클래스명      : CBCUPos
// 클래스 설명   : 직교좌표를 나타내는 Class로 x축, y축, z축 값을 한꺼번에 더하거나 빼거나 초기화 시키는 기능을 수행한다.
// 비고          :
//***************************************************************************
class CBCUPos
{
//함수
public:
	CBCUPos(double i_dblX = 0., double i_dblY = 0., double i_dblZ = 0.);	//생성자
	~CBCUPos();	//소멸자
	CBCUPos operator+(CBCUPos i_cPos);	//CBCUPos 값을 더해 준다.
	CBCUPos operator-(CBCUPos i_cPos);	//CBCUPos 값을 빼 준다.
	CBCUPos operator=(CBCUPos i_cPos);	//CBCUPos 값을 받아서 그값으로 수정한다.
	CBCUPos operator=(double i_dblPos);	//double 값을 받아서 그값으로 초기화 한다.
	CBCUPos operator=(CBCUPosENU i_cPos);	//CBCUPosENU 값을 받아서 그값으로 수정한다.

//변수
public:
	double x;	//X 방향 위치값
	double y;	//Y 방향 위치값
	double z;	//Z 방향 위치값
};

//***************************************************************************
// 파일명        : CAxisTransformations.h
// 프로젝트명    : PKX BCU - 76mm,40mm 축변환
// 최초 작성일   : 2003/12/05
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 클래스명      : CBCUSphere
// 클래스 설명   : 구좌표를 나타내는 Class로 Bearing, Elevation, Range 값을 한꺼번에 더하거나 빼거나 초기화 시키는 기능을 수행한다.
// 비고          : Bearing, Elevation값은 Deg값이다.
//***************************************************************************
class CBCUSphere
{
//함수
public:
	CBCUSphere(double i_dblRange = 0., double i_dblBearing = 0., double i_dblElevation = 0.);	//생성자.
	~CBCUSphere();	//소멸자.
	CBCUSphere operator+(CBCUSphere i_cSphere);	//CBCUSphere 값을 더해 준다.
	CBCUSphere operator-(CBCUSphere i_cSphere);	//CBCUSphere 값을 빼 준다.
	CBCUSphere operator=(CBCUSphere i_cSphere);	//CBCUSphere 값을 받아서 그값으로 수정한다.
	CBCUSphere operator=(double i_dblPos);	//double 값을 받아서 그값으로 초기화 한다.

//변수
public:
	double elevation;	//방위각 위치값
	double bearing;		//고각 위치값 - 진북 기준 시계 방향 각도
	double range;		//거리
};


//***************************************************************************
// 파일명        : CAxisTransformations.h
// 프로젝트명    : PKX BCU - 76mm,40mm 축변환
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 클래스명      : CBCUPosENU
// 클래스 설명   : 직교좌표를 나타내는 Class로 east축, north축, up축 값을 한꺼번에 더하거나 빼거나 초기화 시키는 기능을 수행한다.
// 비고          :
//***************************************************************************
class CBCUPosENU
{
//함수
public:
	CBCUPosENU(double i_dblEast = 0., double i_dblNorth = 0., double i_dblUp = 0.);	//생성자.
	~CBCUPosENU();	//소멸자.
	CBCUPosENU operator+(CBCUPosENU i_cPos);	//CBCUPosENU 값을 더해 준다.
	CBCUPosENU operator-(CBCUPosENU i_cPos);	//CBCUPosENU 값을 빼 준다.
	CBCUPosENU operator=(CBCUPosENU i_cPos);	//CBCUPosENU 값을 받아서 그값으로 수정한다.
	CBCUPosENU operator=(double i_dblPos);	//double 값을 받아서 그값으로 초기화 한다.
	CBCUPosENU operator=(CBCUPos i_cPos);	//CBCUPos 값을 받아서 그값으로 수정한다.

//변수
public:
	double east;	//X 방향 위치값
	double north;	//Y 방향 위치값
	double up;	//Z 방향 위치값
};


//***************************************************************************
// 파일명        : CAxisTransformations.h
// 프로젝트명    : PKX BCU - 76mm, 40mm 축변환
// 최초 작성일   : 2003/12/05
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 클래스명      : CBCUAxisTransformations
// 클래스 설명   : 좌표 변환을 담당하는 Class로 원통좌표에서 직교좌표로의 변환, 직교좌표에서 원통좌표로의 변환 및 직교좌표의 이동 및 회전하는 기능을 수행하는 함수 이다.
// 비고          : 좌표계는 ENU좌표계 - East(X)North(Y)UP(Z)
//                 Heading - 진북을 기준으로 함수가 오른쪽으로 기울어진값
//                 Pitch - 선수가 Heaging값을 가진 상태에서 선수가 수평면 아래로 기울어진 값
//                 Roll - Heading,Pitch된 상태에서 우현이 아래로 기울어진 값
//				   회전방향 - 반시계 방향
//***************************************************************************
class CAxisTransformations
{

//함수
public:

	CAxisTransformations();	//생성자.
	virtual ~CAxisTransformations();	//소멸자.
	CBCUPos ChangePosDeckToNorth(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Deck 기준의 좌표를 진북 기준의 좌표로 변환 한다.
	CBCUPos ChangePosNorthToDeck(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//진북 기준의 좌표를 Deck 기준의 변환 한다.
	CBCUPos CoordinatesMove(CBCUPos i_cPos, CBCUPos i_cDir, bool i_bFlag = true);	//좌표 이동을 수행한다.
	CBCUPos CoordinatesRotate(CBCUPos i_cPos, int i_iDir, double i_dblDeg , bool i_bFlag = true);	//좌표 회전을 수행한다.
	CBCUPos GetPositionAfterRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Roll, Pitch, Yaw가 끝난 시점의 좌표를 계산한다.
	CBCUPos GetPositionBeforeRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Roll, Pitch, Yaw가 일어나기 이전 시점의 좌표를 계산한다.
	double GetTilt(double i_dblRoll, double i_dblPitch);	//임의의 축을 기준으로 회전할때 Roll,Pitch값에 대한 회전 각을 구한다.
	CBCUSphere Orthogonal2Sphere(CBCUPos i_cPos);	//직교 좌표를 구좌표로 변환한다.
	CBCUPos Quaternions(CBCUPos i_cPos, CBCUPos i_cAxis, double i_dblAngle);	//축을 기준으로 좌표를 회전 시킨다.
	CBCUSphere RotateToRangeDirection(CBCUPos i_cTargetPos, CBCUPos i_cPos);	//표적에 대한 Range 방향으로 좌표를 변환한다.
	CBCUPos Sphere2Orthogonal(CBCUSphere i_cSphere);	//구 좌표를 직교 좌표로 변환한다.

	void Cart2Sph(double ,double ,double ,double& ,double& ,double& );
	void Cart2Pol(double ,double ,double ,double& ,double& ,double& );

	void Sph2Cart(double ,double ,double ,double& ,double& ,double& );
	void Pol2Cart(double ,double ,double ,double& ,double& ,double& ); 
};

#endif // !defined(AFX_CAXISTRANSFORMATIONS_H__35C66458_F4DB_48A7_9FC6_57E205C55D10__INCLUDED_)

