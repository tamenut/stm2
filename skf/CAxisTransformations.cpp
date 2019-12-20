// CAxisTransformations.cpp: implementation of the CAxisTransformations class.
//
//////////////////////////////////////////////////////////////////////

#include "CAxisTransformations.h"
#include <math.h>
#include "constant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CBCUPos
// 설명          : CBCUPos Class의 생성자.
//					입력된 값으로 x, y, z값을 초기화 한다.
//					입력값이 정의 되지 않으면 초기값으로 0이 입력된다.
// 입력값        : [IN] i_dblX - 직교좌표의 x축 초기값
//                 [IN] i_dblY - 직교좌표의 y축 초기값
//                 [IN] i_dblZ - 직교좌표의 z축 초기값
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/20 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos::CBCUPos(double i_dblX, double i_dblY, double i_dblZ)
{
	/// x,y,z에 i_dblX,i_dblX,i_dblX값을 초기값으로 입력한다.
	x = i_dblX;
	y = i_dblY;
	z = i_dblZ;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ~CBCUPos
// 설명          : 소멸자
// 입력값        : 해당사항 없음
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/20 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos::~CBCUPos()
{
	///내용 없음
}


//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator+
// 설명          : 두개의 직교좌표값을 더해 준다.
// 입력값        : [IN] i_cPos - 더해질 직교좌표값
// 리턴값        : 기존 CBCUPos값에 i_cPos값이 더해진 직교좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/20 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CBCUPos::operator+(CBCUPos i_cPos)
{
	/**
	x에 i_cPos의 x값을 더한다.
	y에 i_cPos의 y값을 더한다.
	z에 i_cPos의 z값을 더한다.
	더해진 직교좌표 값을 반환한다.
	**/
	return CBCUPos(x + i_cPos.x,y + i_cPos.y,z + i_cPos.z);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator-
// 설명          : 두개의 직교좌표값을 빼 준다.
// 입력값        : [IN] i_cPos - 빼질 직교좌표값
// 리턴값        : CBCUPos값에 i_cPos값이 빼진 직교좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/20 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CBCUPos::operator-(CBCUPos i_cPos)
{
	/**
	x에 i_cPos의 x값을 뺀다.
	y에 i_cPos의 y값을 뺀다.
	z에 i_cPos의 z값을 뺀다.
	빼진 직교좌표 값을 반환한다.
	**/
	return CBCUPos(x - i_cPos.x,y - i_cPos.y,z - i_cPos.z);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : x,y,z 값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_cPos값으로 초기화된 기존의 CBCUPos값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CBCUPos::operator=(CBCUPos i_cPos)
{
	/**
	x에 i_cPos의 x값을 넣는다.
	y에 i_cPos의 y값을 넣는다.
	z에 i_cPos의 z값을 넣는다.
	초기화된 직교 좌표 값을 반환한다.
	**/

	x = i_cPos.x;
	y = i_cPos.y;
	z = i_cPos.z;
	return *this;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : x,y,z 값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_dplPos값으로 초기화된 기존의 CBCUPos값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CBCUPos::operator=(double i_dplPos)
{
	/**
	x,y,z값에 i_dplPos값을 넣는다.
	초기화된 구좌표값을 반환한다.
	**/

	x = i_dplPos;
	y = i_dplPos;
	z = i_dplPos;
	return *this;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : x,y,z 값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_cPos값으로 초기화된 기존의 CBCUPos값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPos CBCUPos::operator=(CBCUPosENU i_cPos)
{
	/**
	x에 i_cPos의 east값을 넣는다.
	y에 i_cPos의 north값을 넣는다.
	z에 i_cPos의 up값을 넣는다.
	초기화된 직교 좌표 값을 반환한다.
	**/

	x = i_cPos.east;
	y = i_cPos.north;
	z = i_cPos.up;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CBCUSphere

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CBCUSphere
// 설명          : CBCUSphere Class의 생성자.
//				   입력된 값으로 bearing, elevation, range값을 초기화 한다.
//				   입력값이 정의 되지 않으면 초기값으로 0이 입력된다.
// 입력값        : [IN] i_dblRange - 구좌표 중심에서의 거리값
//				   [IN] i_dblBearing - 구좌표 중심에서의 방위각
//				   [IN] i_dblElevation - 구좌표 중심에서의 고각
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere::CBCUSphere(double i_dblRange, double i_dblBearing, double i_dblElevation)
{
	/**
	i_dblRange값을 range값에 넣는다.
	i_dblBearing값을 bearing값에 넣는다.
	i_dblElevation값을 elevation값에 넣는다.
	**/
	elevation = i_dblElevation;
	bearing = i_dblBearing;
	range = i_dblRange;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ~CBCUSphere
// 설명          : CBCUSphere Class의 소멸자.
// 입력값        : 해당사항 없음
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere::~CBCUSphere()
{
	///내용 없음.
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator+
// 설명          : 두개의 구좌표값을 더해 준다.
// 입력값        : [IN] i_cSphere - 더해질 구좌표값
// 리턴값        : 기존 CBCUSphere값에 i_cSphere값이 더해진 구좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere CBCUSphere::operator+(CBCUSphere i_cSphere)
{
	/**
	elevation 에 i_cSphere의 elevation값을 더한다.
	bearing에 i_cSphere의 bearing값을 더한다.
	range에 i_cSphere의 bearing값을 더한다.
	더해진 구좌표 값을 반환한다.
	**/

	return CBCUSphere(elevation + i_cSphere.elevation,bearing + i_cSphere.bearing,range + i_cSphere.range);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator-
// 설명          : 두개의 구좌표값을 빼 준다.
// 입력값        : [IN] i_cSphere - 빼질 구좌표값
// 리턴값        : 기존 CBCUSphere값에 i_cSphere값이 빼진 구좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere CBCUSphere::operator-(CBCUSphere i_cSphere)
{
	/**
	elevation 에 i_cSphere의 elevation값을 뺀다.
	bearing에 i_cSphere의 bearing값을 뺀다.
	range에 i_cSphere의 bearing값을 뺀다.
	빼진 구좌표 값을 반환한다.
	**/

	return CBCUSphere(elevation - i_cSphere.elevation,bearing - i_cSphere.bearing,range - i_cSphere.range);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : 입력된 구좌표 값으로 기존의 구좌표 값을 초기화 시킨다.
// 입력값        : [IN] i_cSphere - 구좌표의 초기 값
// 리턴값        : i_cSphere값으로 초기화된 기존의 CBCUSphere값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere CBCUSphere::operator=(CBCUSphere i_cSphere)
{
	/**
	elevation에 i_cSphere의 elevation값을 넣는다.
	bearing에 i_cSphere의 bearing값을 넣는다.
	range에 i_cSphere의 range값을 넣는다.
	초기화된 구좌표 값을 반환한다.
	**/

	elevation = i_cSphere.elevation;
	bearing = i_cSphere.bearing;
	range = i_cSphere.range;
	return *this;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : bearing, elevation, range값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_dblPos - 구좌표의 초기 값
// 리턴값        : i_dblPos값으로 초기화된 기존의 CBCUSphere값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere CBCUSphere::operator=(double i_dblPos)
{
	/**
	elevation, bearing, range값에 dPos값을 넣는다.
	초기화된 구좌표값을 반환한다.
	**/

	elevation = i_dblPos;
	bearing = i_dblPos;
	range = i_dblPos;
	return *this;
}


//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CBCUPosENU
// 설명          : CBCUPosENU Class의 생성자.
//					입력된 값으로 x, y, z값을 초기화 한다.
//					입력값이 정의 되지 않으면 초기값으로 0이 입력된다.
// 입력값        : [IN] i_dblEast - 직교좌표의 x축 초기값
//                 [IN] i_dblNorth - 직교좌표의 y축 초기값
//                 [IN] i_dblUp - 직교좌표의 z축 초기값
// 리턴값        : 해당사항 없음
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU::CBCUPosENU(double i_dblEast, double i_dblNorth, double i_dblUp)
{
	/// east,north,up에 i_dblEast,i_dblNorth,i_dblUp값을 초기값으로 입력한다.
	east = i_dblEast;
	north = i_dblNorth;
	up = i_dblUp;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ~CBCUPosENU
// 설명          : 소멸자
// 입력값        : 해당사항 없음
// 리턴값        : 해당사항 없음
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU::~CBCUPosENU()
{
	///내용 없음
}


//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator+
// 설명          : 두개의 직교좌표값을 더해 준다.
// 입력값        : [IN] i_cPos - 더해질 직교좌표값
// 리턴값        : 기존 CBCUPos값에 i_cPos값이 더해진 직교좌표값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU CBCUPosENU::operator+(CBCUPosENU i_cPos)
{
	/**
	east에 i_cPos의 east값을 더한다.
	north에 i_cPos의 north값을 더한다.
	up에 i_cPos의 up값을 더한다.
	더해진 직교좌표 값을 반환한다.
	**/
	return CBCUPosENU(east + i_cPos.east,north + i_cPos.north,up + i_cPos.up);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator-
// 설명          : 두개의 직교좌표값을 빼 준다.
// 입력값        : [IN] i_cPos - 빼질 직교좌표값
// 리턴값        : CBCUPos값에 i_cPos값이 빼진 직교좌표값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU CBCUPosENU::operator-(CBCUPosENU i_cPos)
{
	/**
	east에 i_cPos의 east값을 뺀다.
	north에 i_cPos의 north값을 뺀다.
	up에 i_cPos의 up값을 뺀다.
	빼진 직교좌표 값을 반환한다.
	**/
	return CBCUPosENU(east - i_cPos.east,north - i_cPos.north,up - i_cPos.up);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : east,north,up 값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_cPos값으로 초기화된 기존의 CBCUPosENU값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU CBCUPosENU::operator=(CBCUPosENU i_cPos)
{
	/**
	east에 i_cPos의 east값을 넣는다.
	north에 i_cPos의 north값을 넣는다.
	up에 i_cPos의 up값을 넣는다.
	초기화된 직교 좌표 값을 반환한다.
	**/

	east = i_cPos.east;
	north = i_cPos.north;
	up = i_cPos.up;
	return *this;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : east,north,up 값을 입력된 값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_dplPos값으로 초기화된 기존의 CBCUPosENU값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU CBCUPosENU::operator=(double i_dplPos)
{
	/**
	x,y,z값에 i_dplPos값을 넣는다.
	초기화된 구좌표값을 반환한다.
	**/

	east = i_dplPos;
	north = i_dplPos;
	up = i_dplPos;
	return *this;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2005/06/07
// 마지막 수정일 : 2005/06/07
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : operator=
// 설명          : east,north,up 값을 입력된 x,y,z값으로 초기화 시킨다.
// 입력값        : [IN] i_cPos - 직교좌표의 초기화 값
// 리턴값        : i_cPos값으로 초기화된 기존의 CBCUPosENU값
// 비고          : 2005/06/07 작성
//***************************************************************************
CBCUPosENU CBCUPosENU::operator =(CBCUPos i_cPos)
{
	/**
	east에 i_cPos의 x값을 넣는다.
	north에 i_cPos의 y값을 넣는다.
	up에 i_cPos의 z값을 넣는다.
	초기화된 직교 좌표 값을 반환한다.
	**/

	east = i_cPos.x;
	north = i_cPos.y;
	up = i_cPos.z;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CAxisTransformations

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CAxisTransformations
// 설명          : CAxisTransformations Class의 생성자
// 입력값        : 해당사항 없음
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CAxisTransformations::CAxisTransformations()
{
	///내용 없음.
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ~CAxisTransformations
// 설명          : CAxisTransformations Class의 소멸자
// 입력값        : 해당사항 없음
// 리턴값        : 해당사항 없음
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CAxisTransformations::~CAxisTransformations()
{
	///내용 없음.
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CoordinatesRotate
// 설명          : 좌표 회전을 수행한다.
// 입력값        : [IN] i_cPos - 회전할 좌표
//				   [IN] i_iDir - 회전 방향 (1:PITCH, 2:ROLL, 3:YAW, 4:X_AXIS, 5:Y_AXIS, 6:Z_AXIS) : 축회전은 반시계 방향
//				   [IN] i_dblAngleDeg - 회전 각도 (단위 : Deg)
//				   [IN] i_bFlag - 기준 (true : 좌표 이동, false : 좌표계 이동)
// 리턴값        : 회전 이후의 좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::CoordinatesRotate(CBCUPos i_cPos, int i_iDir, double i_dblAngleDeg , bool i_bFlag)
{
	CBCUPos cFinalPos;
	double dblAngleRad;

	dblAngleRad = i_dblAngleDeg * DEG2RAD;

	if(i_bFlag)
	{
		switch(i_iDir)
		{
		case X_AXIS:	//X축을 기준으로 회전
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case PITCH:	//PITCH //X축을 기준으로 - 회전
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = - sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case Y_AXIS:	//Y축을 기준으로 회전
		case ROLL:	//ROLL
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.y = i_cPos.y;
			cFinalPos.z = -sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.z;
			break;
		case Z_AXIS:	//Z축을 기준으로 회전
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x - sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		case YAW:	//YAW //Z축을 기준으로 - 회전
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x + sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = -sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		default:
			cFinalPos = i_cPos;
			break;
		}
	}
	else
	{
		switch(i_iDir)
		{
		case X_AXIS:	//X축을 기준으로 회전
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = -sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case PITCH:	//PITCH //X축을 기준으로 - 회전
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case Y_AXIS:	//Y축을 기준으로 회전
		case ROLL:	//ROLL
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.y = i_cPos.y;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.z;
			break;
		case Z_AXIS:	//Z축을 기준으로 회전
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x + sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = -sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		case YAW:	//YAW //Z축을 기준으로 - 회전
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x - sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		default:
			cFinalPos = i_cPos;
			break;
		}
	}

	return cFinalPos;

	/**
	if(i_bFlag)
	{
		switch(i_iDir)
		{
		case X_AXIS:	//X축을 기준으로 회전
			[A] =	1			0				0
					0	cos(회전 각도)	-sin(회전 각도)
					0	sin(회전 각도)	cos(회전 각도)
			break;
		case PITCH:	//PITCH //X축을 기준으로 - 회전
			[A] =	1			0				0
					0	cos(회전 각도)	sin(회전 각도)
					0	-sin(회전 각도)	cos(회전 각도)
			break;
		case Y_AXIS:	//Y축을 기준으로 회전
		case ROLL:	//ROLL
			[A] =	cos(회전 각도)	0	sin(회전 각도)
					0			1			0
					-sin(회전 각도)	0	cos(회전 각도)
			break;
		case Z_AXIS:	//Z축을 기준으로 회전
			[A] =	cos(회전 각도)	-sin(회전 각도)	0
					sin(회전 각도)	cos(회전 각도)	0
					0				1			0
			break;
		case YAW:	//YAW //Z축을 기준으로 - 회전
		[A] =	cos(회전 각도)	sin(회전 각도)	0
				-sin(회전 각도)	cos(회전 각도)	0
				0				1			0
			break;
		default:
			i_cPos을 반환 한다.
			break;
		}
	}
	else
	{
		switch(i_iDir)
		{
			case X_AXIS:	//X축을 기준으로 회전
				[A] =	1			0				0
						0	cos(회전 각도)	sin(회전 각도)
						0	-sin(회전 각도)	cos(회전 각도)
				break;
			case PITCH:	//PITCH //X축을 기준으로 - 회전
				[A] =	1			0				0
						0	cos(회전 각도)	-sin(회전 각도)
						0	sin(회전 각도)	cos(회전 각도)
				break;
			case Y_AXIS:	//Y축을 기준으로 회전
			case ROLL:	//ROLL
				[A] =	cos(회전 각도)	0	-sin(회전 각도)
						0			1			0
						sin(회전 각도)	0	cos(회전 각도)
				break;
			case Z_AXIS:	//Z축을 기준으로 회전
				[A] =	cos(회전 각도)	sin(회전 각도)	0
						-sin(회전 각도)	cos(회전 각도)	0
						0				1			0
				break;
			case YAW:	//YAW //Z축을 기준으로 - 회전
				[A] =	cos(회전 각도)	-sin(회전 각도)	0
						sin(회전 각도)	cos(회전 각도)	0
						0				1			0
				break;
			default:
				i_cPos을 반환 한다.
				break;
		}
	}
	[B] = i_cPos.x
		  i_cPos.y
		  i_cPos.z

	[A][B] 값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : CoordinatesMove
// 설명          : 좌표 이동을 수행한다.
// 입력값        : [IN] i_cPos - 이동할 좌표
//				   [IN] i_cDir - 이동할거리 및 방향
//				   [IN] i_bFlag - 기준(true : 좌표 이동, false : 좌표계 이동)
// 리턴값        : 이동된 이후의 좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::CoordinatesMove(CBCUPos i_cPos, CBCUPos i_cDir, bool i_bFlag)
{
	/**
	if(i_bFlag)
	{
		i_cPos에 i_cDir를 더한값을 반환한다.
	}
	else
	{
		i_cPos에 i_cDir를 뺀값을 반환한다.
	}
	**/

	CBCUPos cFinalPos;
	if(i_bFlag)
	{
		cFinalPos = i_cPos + i_cDir;
	}
	else
	{
		cFinalPos = i_cPos - i_cDir;
	}
	return cFinalPos;
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : Sphere2Orthogonal
// 설명          : 구 좌표를 직교 좌표로 변환
// 입력값        : [IN] i_cSphere - 변환할 구좌표값
// 리턴값        : 구좌표에서 변환된 직교좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::Sphere2Orthogonal(CBCUSphere i_cSphere)
{

	CBCUPos cFinalPos;
	double dblElevationRad;
	double dblBearingRad;
	dblElevationRad = i_cSphere.elevation * DEG2RAD;
	dblBearingRad = i_cSphere.bearing * DEG2RAD;

	cFinalPos.x = i_cSphere.range * cos(dblElevationRad) * sin(dblBearingRad);
	cFinalPos.y = i_cSphere.range * cos(dblElevationRad) * cos(dblBearingRad);
	cFinalPos.z = i_cSphere.range * sin(dblElevationRad);

	return cFinalPos;
	/**
	반환값.x = i_cSphere.range * cos(i_cSphere.elevation값) * sin(i_cSphere.bearing값);
	반환값.y = i_cSphere.range * cos(i_cSphere.elevation값) * cos(i_cSphere.bearing값);
	반환값.z = i_cSphere.range * sin(i_cSphere.elevation값);
	계산된 값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : Orthogonal2Sphere
// 설명          : 직교 좌표를 구좌표로 변환한다.
// 입력값        : [IN] i_cPos - 구좌표로 변환할 직교좌표 위치
// 리턴값        : 직교좌표에서 변환된 구좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUSphere CAxisTransformations::Orthogonal2Sphere(CBCUPos i_cPos)
{
	CBCUSphere cFinalPos;
	double dblTemp;
	dblTemp = i_cPos.x * i_cPos.x + i_cPos.y * i_cPos.y;

	cFinalPos.elevation = atan2(i_cPos.z,sqrt(dblTemp)) * RAD2DEG;
	cFinalPos.bearing = atan2(i_cPos.x,i_cPos.y) * RAD2DEG;
	cFinalPos.range = sqrt(dblTemp + i_cPos.z * i_cPos.z);

	return cFinalPos;

	/**
	Bearing은 i_cPos.x와 i_cPos.y가 이루는 각도
	Elevation은 i_cPos.z와 sqrt(i_cPos.x^2 + i_cPos.y^2)이 이루는 각도
	Range는 sqrt(i_cPos.x^2 + i_cPos.y^2 + i_cPos.z^2)
	계산된 구 좌표값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : GetTilt
// 설명          : 임의의 축을 기준으로 회전할때 Roll,Pitch값에 대한 회전 각을 구한다
// 입력값        : [IN] i_dblRollDeg - 함포 Tilt값의 Roll값 (단위 : Deg)
//				   [IN] i_dblPitchDeg - 함포 Tilt값의 Pitch 값 (단위 : Deg)
// 리턴값        : 축을 기준으로 회전할 값 (단위 : Deg)
// 비고          : OpenGL에서 glRotatef(GetTilt(dRoll,dPitch),dPitch,dRoll,0);
//				   해군에서 사용하는 Roll,Pitch 계산 법
//					Roll = Tilt * sin(BHP);
//					Pitch = Tilt * cos(BHP);
//				   2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
double CAxisTransformations::GetTilt(double i_dblRollDeg, double i_dblPitchDeg)
{
	double dblBHPDeg;
	double dblTILT;
	double dblBHPRad;

	dblBHPRad = atan2(i_dblRollDeg,i_dblPitchDeg);
	dblBHPDeg = dblBHPRad * RAD2DEG;	//실제 BHP값
	/*
	if((-180. <= dblBHPDeg) && (180. >= dblBHPDeg) && (0. != i_dblRollDeg))	//dRoll이 0이 아니면 dBHP도 0이 아님
	{
		dblTILT = i_dblRollDeg / sin(dblBHPRad);	//비고에 있는 내용을 풀면 됨
	}
	else
	{
		dblTILT = i_dblPitchDeg / cos(dblBHPRad);
	}
	*/
	if(i_dblPitchDeg>i_dblRollDeg)
	{
		dblTILT = i_dblPitchDeg / cos(dblBHPRad);
	}
	else
	{
		if(i_dblRollDeg == 0)
		{
			dblTILT = i_dblPitchDeg / cos(dblBHPRad);
		}
		else
		{
			dblTILT = i_dblRollDeg / sin(dblBHPRad);
		}
	}
	dblBHPDeg = atan2(i_dblPitchDeg,i_dblRollDeg) * RAD2DEG;	//BHP에 90도인 값

	return dblTILT;
	/**
	PHP : Roll값과 Pitch값이 이루는 값
	Roll = Tilt * sin(BHP)
	Pitch = Tilt * cos(BHP)
	위 식을 이용하여 Tilt값을 구한다.
	구해진 Tilt값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : Quaternions
// 설명          : 축을 기준으로 좌표를 회전 시킨다.
// 입력값        : [IN] i_cPos - 이동할 좌표값
//				   [IN] i_cAxis - 기준 축
//				   [IN] i_dblAngleDeg - 이동할 각도 (단위 : Deg)
// 리턴값        : 축을 기준으로 회전된 좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::Quaternions(CBCUPos i_cPos, CBCUPos i_cAxis, double i_dblAngleDeg)
{
	CBCUPos cResult;
	double dblThetaRad;
	double dblUnit;
	double c,s,t;
	double q11,q12,q13,q21,q22,q23,q31,q32,q33;

	dblThetaRad = i_dblAngleDeg * DEG2RAD;
	dblUnit = sqrt(i_cAxis.x*i_cAxis.x + i_cAxis.y*i_cAxis.y + i_cAxis.z*i_cAxis.z);

	if(0 == dblUnit)
	{
		return i_cPos;
	}

	i_cAxis.x = i_cAxis.x/dblUnit;
	i_cAxis.y = i_cAxis.y/dblUnit;
	i_cAxis.z = i_cAxis.z/dblUnit;
	c = cos(dblThetaRad);
	s = sin(dblThetaRad);
	t = 1-cos(dblThetaRad);

	q11 = t*i_cAxis.x*i_cAxis.x + c;
	q12 = t*i_cAxis.x*i_cAxis.y + s*i_cAxis.z;
	q13 = t*i_cAxis.x*i_cAxis.z - s*i_cAxis.y;
	q21 = t*i_cAxis.x*i_cAxis.y - s*i_cAxis.z;
	q22 = t*i_cAxis.y*i_cAxis.y + c;
	q23 = t*i_cAxis.y*i_cAxis.z + s*i_cAxis.x;
	q31 = t*i_cAxis.x*i_cAxis.z + s*i_cAxis.y;
	q32 = t*i_cAxis.y*i_cAxis.z - s*i_cAxis.x;
	q33 = t*i_cAxis.z*i_cAxis.z + c;

	cResult.x = i_cPos.x*q11 + i_cPos.y*q21 + i_cPos.z*q31;
	cResult.y = i_cPos.x*q12 + i_cPos.y*q22 + i_cPos.z*q32;
	cResult.z = i_cPos.x*q13 + i_cPos.y*q23 + i_cPos.z*q33;

	return cResult;

	/*	//아래 내용을 요약하면 위의 내용이 됨 위쪽이 계산량이 적음
	CBCUPos result;
	double dTheta;
	double unit;
	double x,y,z,w;
	double q11,q12,q13,q21,q22,q23,q31,q32,q33;

	dTheta = (dAngle)/2.;
	unit = sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
	axis.x = axis.x/unit;
	axis.y = axis.y/unit;
	axis.z = axis.z/unit;

	w = cos(dTheta);
	x = sin(dTheta) * axis.x;
	y = sin(dTheta) * axis.y;
	z = sin(dTheta) * axis.z;

	q11 = w*w + x*x - y*y - z*z;
	q12 = 2*x*y + 2*w*z;
	q13 = 2*x*z - 2*w*y;
	q21 = 2*x*y - 2*w*z;
	q22 = w*w - x*x + y*y - z*z;
	q23 = 2*y*z + 2*w*x;
	q31 = 2*x*z + 2*w*y;
	q32 = 2*y*z - 2*w*x;
	q33 = w*w - x*x - y*y + z*z;

	result.x = pos.x*q11 + pos.y*q21 + pos.z*q31;
	result.y = pos.x*q12 + pos.y*q22 + pos.z*q32;
	result.z = pos.x*q13 + pos.y*q23 + pos.z*q33;

	return result;
	*/

	/**
	반환값.x = i_cPos.x*q11 + i_cPos.y*q21 + i_cPos.z*q31;
	반환값.y = i_cPos.x*q12 + i_cPos.y*q22 + i_cPos.z*q32;
	반환값.z = i_cPos.x*q13 + i_cPos.y*q23 + i_cPos.z*q33;

	c = cos(dAngle);
	s = sin(dAngle);
	t = 1-cos(dAngle);

	q11 = t*axis.x*axis.x + c;
	q12 = t*axis.x*axis.y + s*axis.z;
	q13 = t*axis.x*axis.z - s*axis.y;
	q21 = t*axis.x*axis.y - s*axis.z;
	q22 = t*axis.y*axis.y + c;
	q23 = t*axis.y*axis.z + s*axis.x;
	q31 = t*axis.x*axis.z + s*axis.y;
	q32 = t*axis.y*axis.z - s*axis.x;
	q33 = t*axis.z*axis.z + c;

	계산된 결과값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : GetPositionAfterRollPitchYaw
// 설명          : Roll, Pitch, Yaw가 생긴 이후의 좌표를 계산한다.
// 입력값        : [IN] i_dblRollDeg - 자함의 Roll값 (단위 : Deg)
//				   [IN] i_dblPitchDeg - 자함의 Pitch 값 (단위 : Deg)
//				   [IN] i_dblYawDeg - 자함의 Yaw 값 (단위 : Deg)
//				   [IN] i_cPos - 변환할 좌표
// 리턴값        : Roll,Pitch,Yaw가 끝난 이후의 좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::GetPositionAfterRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	i_cPos = CoordinatesRotate(i_cPos, ROLL, i_dblRollDeg);
	i_cPos = CoordinatesRotate(i_cPos, PITCH, i_dblPitchDeg);
	i_cPos = CoordinatesRotate(i_cPos, YAW, i_dblYawDeg);

	return i_cPos;
	/**
	Roll에 대한 좌표 회전을 수행한다.
	Pitch에 대한 좌표 회전을 수행한다.
	Yaw에 대한 좌표 회전을 수행한다.
	변환된 값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/12/21
// 마지막 수정일 : 2005/05/23
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : GetPositionBeforeRollPitchYaw
// 설명          : Roll, Pitch, Yaw가 발생하기 이전의 좌표를 계산한다.
// 입력값        : [IN] i_dblRollDeg - 자함의 Roll값 (단위 : Deg)
//				   [IN] i_dblPitchDeg - 자함의 Pitch 값 (단위 : Deg)
//				   [IN] i_dblYawDeg - 자함의 Yaw 값 (단위 : Deg)
//				   [IN] i_cPos - 변환할 좌표
// 리턴값        : Roll,Pitch,Yaw가 발생하기 이전의 좌표값
// 비고          : 2004/12/21 작성
//				   2005/05/23 설명 헤더 변경 및 주석 추가
//***************************************************************************
CBCUPos CAxisTransformations::GetPositionBeforeRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	i_cPos = CoordinatesRotate(i_cPos, YAW, i_dblYawDeg, false);
	i_cPos = CoordinatesRotate(i_cPos, PITCH, i_dblPitchDeg, false);
	i_cPos = CoordinatesRotate(i_cPos, ROLL, i_dblRollDeg, false) ;

	return i_cPos;
	/**
	Yaw에 대한 좌표 회전을 수행한다 - 반대 방향.
	Pitch에 대한 좌표 회전을 수행한다 - 반대 방향.
	Roll에 대한 좌표 회전을 수행한다 - 반대 방향.
	변환된 값을 반환한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2006/04/20
// 마지막 수정일 : 2006/04/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ChangePosNorthToDeck
// 설명          : 진북 기준의 좌표를 Deck 기준의 변환 한다.
// 입력값        : [IN] i_dblRollDeg - 자함의 Roll값 (단위 : Deg)
//				   [IN] i_dblPitchDeg - 자함의 Pitch 값 (단위 : Deg)
//				   [IN] i_dblYawDeg - 자함의 Yaw 값 (단위 : Deg)
//				   [IN] i_cPos - 진북 기준의 위치
// 리턴값        : Deck 기준의 위치
// 비고          :
//***************************************************************************
CBCUPos CAxisTransformations::ChangePosNorthToDeck(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	return GetPositionBeforeRollPitchYaw(i_dblRollDeg,i_dblPitchDeg,i_dblYawDeg,i_cPos);
	/**
	GetPositionBeforeRollPitchYaw 함수를 수행한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2006/04/20
// 마지막 수정일 : 2006/04/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : ChangePosDeckToNorth
// 설명          : Deck 기준의 좌표를 진북 기준의 좌표로 변환 한다.
// 입력값        : [IN] i_dblRollDeg - 자함의 Roll값 (단위 : Deg)
//				   [IN] i_dblPitchDeg - 자함의 Pitch 값 (단위 : Deg)
//				   [IN] i_dblYawDeg - 자함의 Yaw 값 (단위 : Deg)
//				   [IN] i_cPos - Deck 기준의 위치
// 리턴값        : 진북 기준의 위치
// 비고          :
//***************************************************************************
CBCUPos CAxisTransformations::ChangePosDeckToNorth(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	return GetPositionAfterRollPitchYaw(i_dblRollDeg,i_dblPitchDeg,i_dblYawDeg,i_cPos);
	/**
	GetPositionAfterRollPitchYaw 함수를 수행한다.
	**/
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2006/04/20
// 마지막 수정일 : 2006/04/20
// 최초 작성자   : SW팀 SW2그룹/PKX SW 김천환
// 마지막 수정자 : SW팀 SW2그룹/PKX SW 김천환
// 함수명        : RotateToRangeDirection
// 설명          : 표적에 대한 Range 방향으로 좌표를 변환한다.
// 입력값        : [IN] i_cTargetPos - 표적의 위치
//				   [IN] i_cPos - 기준 위치
// 리턴값        : 표적의 Range 뱡향에 대한 표적의 좌표
// 비고          :
//***************************************************************************
CBCUSphere CAxisTransformations::RotateToRangeDirection(CBCUPos i_cTargetPos, CBCUPos i_cPos)
{
	CBCUSphere cSphere;

	cSphere = Orthogonal2Sphere(i_cTargetPos);	//Pos

	///Z축 방향으로 표적의 Bearing 만큼 회전 한다.
	i_cPos = CoordinatesRotate(i_cPos,Z_AXIS,cSphere.bearing);
	///X축 방향으로 표적의 -Elevation 만큼 회전 한다.
	i_cPos = CoordinatesRotate(i_cPos,X_AXIS,-cSphere.elevation);

	///구좌표로 변환하여 반환한다.
	return CBCUSphere(i_cPos.y,fabs(atan2(i_cPos.x,cSphere.range)) * RAD2DEG,fabs(atan2(i_cPos.z,cSphere.range)) * RAD2DEG);
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/03/05
// 마지막 수정일 : 2006/12/13
// 최초 작성자   : 전투체계 System 그룹/M&S 윤동식
// 마지막 수정자 : 전투체계 System 그룹/M&S 윤동식
// 함수명        : Sph2Cart
// 설명          : 구좌표계의 값을 직교 좌표계로 변경(Elevation, Z 고려)
// 입력값        : [IN] RX - Range data
//                 [IN] BY - Bearing data
//                 [IN] EZ - Elevation data
//                 [IN] XX - X axis data
//                 [IN] YY - Y axis data
//                 [IN] ZZ - Z axis data
// 리턴값        : 해당사항 없음
// 비고          : 2009/01/16 설명 헤더 변경 및 주석 추가
//***************************************************************************
void CAxisTransformations::Sph2Cart(double RX, double BY, double EZ, double &XX, double &YY, double &ZZ)
{
/// Range, Bearing, Elevation 값을 이용해 X, Y, Z data를 계산
	double rt;

	if((RX>0.)||(RX<0.))
	{
		ZZ = RX*sin(EZ);
		rt = RX*cos(EZ);
		
		XX = rt*sin(BY);
		YY = rt*cos(BY);
	}
	else
	{
		ZZ = 0.;
		XX = 0.;
		YY = 0.;
	}
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/03/05
// 마지막 수정일 : 2006/12/13
// 최초 작성자   : 전투체계 System 그룹/M&S 윤동식
// 마지막 수정자 : 전투체계 System 그룹/M&S 윤동식
// 함수명        : Pol2Cart
// 설명          : 구좌표계의 값을 직교 좌표계로 변경(Elevation, Z 무시)
// 입력값        : [IN] RX - Range data
//                 [IN] BY - Bearing data
//                 [IN] EZ - Elevation data
//                 [IN] XX - X axis data
//                 [IN] YY - Y axis data
//                 [IN] ZZ - Z axis data
// 리턴값        : 해당사항 없음
// 비고          : 2009/01/16 설명 헤더 변경 및 주석 추가
//*************************************************************************** 
void CAxisTransformations::Pol2Cart(double RX, double BY, double EZ, double &XX, double &YY, double &ZZ)
{
/// Range, Bearing, 값을 이용해 X, Y data를 계산
	ZZ = 0.;

	if((RX>0.)||(RX<0.))
	{
		XX = RX*sin(BY);
		YY = RX*cos(BY);
	}
	else
	{
		XX = 0.;
		YY = 0.;
	}
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/03/05
// 마지막 수정일 : 2006/12/13
// 최초 작성자   : 전투체계 System 그룹/M&S 윤동식
// 마지막 수정자 : 전투체계 System 그룹/M&S 윤동식
// 함수명        : Cart2Sph
// 설명          : 직교좌표계의 값을 구좌표계로 변경(Elevation, Z 고려)
// 입력값        : [IN] X - X axis data
//                 [IN] Y - Y axis data
//                 [IN] Z - Z axis data
//                 [IN] R - Range data
//                 [IN] B - Bearing data
//                 [IN] E - Elevation data
// 리턴값        : 해당사항 없음
// 비고          : 2009/01/16 설명 헤더 변경 및 주석 추가
//***************************************************************************
void CAxisTransformations::Cart2Sph(double X, double Y, double Z, double &R, double &B, double &E)
{
/**
X, Y, Z data를 이용해 Range, Bearing, Elevation 계산
**/
	double dblTmpB1, dblTmpB2, dblTmpB3;

	R = sqrt(pow(X,2.0) + pow(Y,2.0) + pow(Z,2.0));

	if((R>0.)||(R<0.))
	{
		E = asin(Z/R);
		
		// bearing을 구한다.
		dblTmpB1 = atan2(Y, X);
		// +, - 값을 바꾼다.
		dblTmpB2 = dblTmpB1 * -1.;
		// 나온값에 90도를 합산하여 y축 기준으로 바꾼다.
		dblTmpB3 = dblTmpB2 + PI/2.;
		// 결과값이 +pi 혹은 -pi보다 크면 다시 계산해서 -pi < 결과값 < pi 으로 조절한다.
		if(fabs(dblTmpB3) > PI )
		{
			if(dblTmpB3>0.)
				B = -2.*PI + dblTmpB3;
			else
				B = 2.*PI + dblTmpB3;
		}
		else
		{
			B = dblTmpB3;
		}
	}
	else
	{
		E = 0.;
		B = 0.;
	}
}

//***************************************************************************
// 파일명        : CAxisTransformations.cpp
// 최초 작성일   : 2004/03/05
// 마지막 수정일 : 2006/12/13
// 최초 작성자   : 전투체계 System 그룹/M&S 윤동식
// 마지막 수정자 : 전투체계 System 그룹/M&S 윤동식
// 함수명        : Cart2Pol
// 설명          : 직교좌표계의 값을 구좌표계로 변경(Elevation, Z 무시)
// 입력값        : [IN] X - X axis data
//                 [IN] Y - Y axis data
//                 [IN] Z - Z axis data
//                 [IN] R - Range data
//                 [IN] B - Bearing data
//                 [IN] E - Elevation data
// 리턴값        : 해당사항 없음
// 비고          : 2009/01/16 설명 헤더 변경 및 주석 추가
//***************************************************************************
void CAxisTransformations::Cart2Pol(double X, double Y, double Z, double &R, double &B, double &E)
{
/**
X, Y data를 이용해 Range, Bearing 계산
**/
	double dblTmpB1, dblTmpB2, dblTmpB3;

	E = 0.;

	R = sqrt( pow(X,2.0) + pow(Y,2.0) );
	
	if((R>0.)||(R<0.))
	{
		// bearing을 구한다.
		dblTmpB1 = atan2(Y, X);
		// +, - 값을 바꾼다.
		dblTmpB2 = dblTmpB1 * -1.;
		// 나온값에 90도를 합산하여 y축 기준으로 바꾼다.
		dblTmpB3 = dblTmpB2 + PI/2.;

		// 결과값이 +pi 혹은 -pi보다 크면 다시 계산해서 -pi < 결과값 < pi 으로 조절한다.
		if(fabs(dblTmpB3) > PI )
		{
			if(dblTmpB3>0.)
				B = -2.*PI + dblTmpB3;
			else
				B = 2.*PI + dblTmpB3;
		}
		else
			B = dblTmpB3;
	}
	else
	{
		B = 0.;
	}
}