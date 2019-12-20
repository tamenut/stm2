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
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CBCUPos
// ����          : CBCUPos Class�� ������.
//					�Էµ� ������ x, y, z���� �ʱ�ȭ �Ѵ�.
//					�Է°��� ���� ���� ������ �ʱⰪ���� 0�� �Էµȴ�.
// �Է°�        : [IN] i_dblX - ������ǥ�� x�� �ʱⰪ
//                 [IN] i_dblY - ������ǥ�� y�� �ʱⰪ
//                 [IN] i_dblZ - ������ǥ�� z�� �ʱⰪ
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/20 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos::CBCUPos(double i_dblX, double i_dblY, double i_dblZ)
{
	/// x,y,z�� i_dblX,i_dblX,i_dblX���� �ʱⰪ���� �Է��Ѵ�.
	x = i_dblX;
	y = i_dblY;
	z = i_dblZ;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ~CBCUPos
// ����          : �Ҹ���
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/20 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos::~CBCUPos()
{
	///���� ����
}


//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator+
// ����          : �ΰ��� ������ǥ���� ���� �ش�.
// �Է°�        : [IN] i_cPos - ������ ������ǥ��
// ���ϰ�        : ���� CBCUPos���� i_cPos���� ������ ������ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/20 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CBCUPos::operator+(CBCUPos i_cPos)
{
	/**
	x�� i_cPos�� x���� ���Ѵ�.
	y�� i_cPos�� y���� ���Ѵ�.
	z�� i_cPos�� z���� ���Ѵ�.
	������ ������ǥ ���� ��ȯ�Ѵ�.
	**/
	return CBCUPos(x + i_cPos.x,y + i_cPos.y,z + i_cPos.z);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator-
// ����          : �ΰ��� ������ǥ���� �� �ش�.
// �Է°�        : [IN] i_cPos - ���� ������ǥ��
// ���ϰ�        : CBCUPos���� i_cPos���� ���� ������ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/20 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CBCUPos::operator-(CBCUPos i_cPos)
{
	/**
	x�� i_cPos�� x���� ����.
	y�� i_cPos�� y���� ����.
	z�� i_cPos�� z���� ����.
	���� ������ǥ ���� ��ȯ�Ѵ�.
	**/
	return CBCUPos(x - i_cPos.x,y - i_cPos.y,z - i_cPos.z);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : x,y,z ���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_cPos������ �ʱ�ȭ�� ������ CBCUPos��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CBCUPos::operator=(CBCUPos i_cPos)
{
	/**
	x�� i_cPos�� x���� �ִ´�.
	y�� i_cPos�� y���� �ִ´�.
	z�� i_cPos�� z���� �ִ´�.
	�ʱ�ȭ�� ���� ��ǥ ���� ��ȯ�Ѵ�.
	**/

	x = i_cPos.x;
	y = i_cPos.y;
	z = i_cPos.z;
	return *this;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : x,y,z ���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_dplPos������ �ʱ�ȭ�� ������ CBCUPos��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CBCUPos::operator=(double i_dplPos)
{
	/**
	x,y,z���� i_dplPos���� �ִ´�.
	�ʱ�ȭ�� ����ǥ���� ��ȯ�Ѵ�.
	**/

	x = i_dplPos;
	y = i_dplPos;
	z = i_dplPos;
	return *this;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : x,y,z ���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_cPos������ �ʱ�ȭ�� ������ CBCUPos��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPos CBCUPos::operator=(CBCUPosENU i_cPos)
{
	/**
	x�� i_cPos�� east���� �ִ´�.
	y�� i_cPos�� north���� �ִ´�.
	z�� i_cPos�� up���� �ִ´�.
	�ʱ�ȭ�� ���� ��ǥ ���� ��ȯ�Ѵ�.
	**/

	x = i_cPos.east;
	y = i_cPos.north;
	z = i_cPos.up;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CBCUSphere

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CBCUSphere
// ����          : CBCUSphere Class�� ������.
//				   �Էµ� ������ bearing, elevation, range���� �ʱ�ȭ �Ѵ�.
//				   �Է°��� ���� ���� ������ �ʱⰪ���� 0�� �Էµȴ�.
// �Է°�        : [IN] i_dblRange - ����ǥ �߽ɿ����� �Ÿ���
//				   [IN] i_dblBearing - ����ǥ �߽ɿ����� ������
//				   [IN] i_dblElevation - ����ǥ �߽ɿ����� ��
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere::CBCUSphere(double i_dblRange, double i_dblBearing, double i_dblElevation)
{
	/**
	i_dblRange���� range���� �ִ´�.
	i_dblBearing���� bearing���� �ִ´�.
	i_dblElevation���� elevation���� �ִ´�.
	**/
	elevation = i_dblElevation;
	bearing = i_dblBearing;
	range = i_dblRange;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ~CBCUSphere
// ����          : CBCUSphere Class�� �Ҹ���.
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere::~CBCUSphere()
{
	///���� ����.
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator+
// ����          : �ΰ��� ����ǥ���� ���� �ش�.
// �Է°�        : [IN] i_cSphere - ������ ����ǥ��
// ���ϰ�        : ���� CBCUSphere���� i_cSphere���� ������ ����ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere CBCUSphere::operator+(CBCUSphere i_cSphere)
{
	/**
	elevation �� i_cSphere�� elevation���� ���Ѵ�.
	bearing�� i_cSphere�� bearing���� ���Ѵ�.
	range�� i_cSphere�� bearing���� ���Ѵ�.
	������ ����ǥ ���� ��ȯ�Ѵ�.
	**/

	return CBCUSphere(elevation + i_cSphere.elevation,bearing + i_cSphere.bearing,range + i_cSphere.range);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator-
// ����          : �ΰ��� ����ǥ���� �� �ش�.
// �Է°�        : [IN] i_cSphere - ���� ����ǥ��
// ���ϰ�        : ���� CBCUSphere���� i_cSphere���� ���� ����ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere CBCUSphere::operator-(CBCUSphere i_cSphere)
{
	/**
	elevation �� i_cSphere�� elevation���� ����.
	bearing�� i_cSphere�� bearing���� ����.
	range�� i_cSphere�� bearing���� ����.
	���� ����ǥ ���� ��ȯ�Ѵ�.
	**/

	return CBCUSphere(elevation - i_cSphere.elevation,bearing - i_cSphere.bearing,range - i_cSphere.range);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : �Էµ� ����ǥ ������ ������ ����ǥ ���� �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cSphere - ����ǥ�� �ʱ� ��
// ���ϰ�        : i_cSphere������ �ʱ�ȭ�� ������ CBCUSphere��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere CBCUSphere::operator=(CBCUSphere i_cSphere)
{
	/**
	elevation�� i_cSphere�� elevation���� �ִ´�.
	bearing�� i_cSphere�� bearing���� �ִ´�.
	range�� i_cSphere�� range���� �ִ´�.
	�ʱ�ȭ�� ����ǥ ���� ��ȯ�Ѵ�.
	**/

	elevation = i_cSphere.elevation;
	bearing = i_cSphere.bearing;
	range = i_cSphere.range;
	return *this;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : bearing, elevation, range���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_dblPos - ����ǥ�� �ʱ� ��
// ���ϰ�        : i_dblPos������ �ʱ�ȭ�� ������ CBCUSphere��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUSphere CBCUSphere::operator=(double i_dblPos)
{
	/**
	elevation, bearing, range���� dPos���� �ִ´�.
	�ʱ�ȭ�� ����ǥ���� ��ȯ�Ѵ�.
	**/

	elevation = i_dblPos;
	bearing = i_dblPos;
	range = i_dblPos;
	return *this;
}


//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CBCUPosENU
// ����          : CBCUPosENU Class�� ������.
//					�Էµ� ������ x, y, z���� �ʱ�ȭ �Ѵ�.
//					�Է°��� ���� ���� ������ �ʱⰪ���� 0�� �Էµȴ�.
// �Է°�        : [IN] i_dblEast - ������ǥ�� x�� �ʱⰪ
//                 [IN] i_dblNorth - ������ǥ�� y�� �ʱⰪ
//                 [IN] i_dblUp - ������ǥ�� z�� �ʱⰪ
// ���ϰ�        : �ش���� ����
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU::CBCUPosENU(double i_dblEast, double i_dblNorth, double i_dblUp)
{
	/// east,north,up�� i_dblEast,i_dblNorth,i_dblUp���� �ʱⰪ���� �Է��Ѵ�.
	east = i_dblEast;
	north = i_dblNorth;
	up = i_dblUp;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ~CBCUPosENU
// ����          : �Ҹ���
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU::~CBCUPosENU()
{
	///���� ����
}


//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator+
// ����          : �ΰ��� ������ǥ���� ���� �ش�.
// �Է°�        : [IN] i_cPos - ������ ������ǥ��
// ���ϰ�        : ���� CBCUPos���� i_cPos���� ������ ������ǥ��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU CBCUPosENU::operator+(CBCUPosENU i_cPos)
{
	/**
	east�� i_cPos�� east���� ���Ѵ�.
	north�� i_cPos�� north���� ���Ѵ�.
	up�� i_cPos�� up���� ���Ѵ�.
	������ ������ǥ ���� ��ȯ�Ѵ�.
	**/
	return CBCUPosENU(east + i_cPos.east,north + i_cPos.north,up + i_cPos.up);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator-
// ����          : �ΰ��� ������ǥ���� �� �ش�.
// �Է°�        : [IN] i_cPos - ���� ������ǥ��
// ���ϰ�        : CBCUPos���� i_cPos���� ���� ������ǥ��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU CBCUPosENU::operator-(CBCUPosENU i_cPos)
{
	/**
	east�� i_cPos�� east���� ����.
	north�� i_cPos�� north���� ����.
	up�� i_cPos�� up���� ����.
	���� ������ǥ ���� ��ȯ�Ѵ�.
	**/
	return CBCUPosENU(east - i_cPos.east,north - i_cPos.north,up - i_cPos.up);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : east,north,up ���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_cPos������ �ʱ�ȭ�� ������ CBCUPosENU��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU CBCUPosENU::operator=(CBCUPosENU i_cPos)
{
	/**
	east�� i_cPos�� east���� �ִ´�.
	north�� i_cPos�� north���� �ִ´�.
	up�� i_cPos�� up���� �ִ´�.
	�ʱ�ȭ�� ���� ��ǥ ���� ��ȯ�Ѵ�.
	**/

	east = i_cPos.east;
	north = i_cPos.north;
	up = i_cPos.up;
	return *this;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : east,north,up ���� �Էµ� ������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_dplPos������ �ʱ�ȭ�� ������ CBCUPosENU��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU CBCUPosENU::operator=(double i_dplPos)
{
	/**
	x,y,z���� i_dplPos���� �ִ´�.
	�ʱ�ȭ�� ����ǥ���� ��ȯ�Ѵ�.
	**/

	east = i_dplPos;
	north = i_dplPos;
	up = i_dplPos;
	return *this;
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : operator=
// ����          : east,north,up ���� �Էµ� x,y,z������ �ʱ�ȭ ��Ų��.
// �Է°�        : [IN] i_cPos - ������ǥ�� �ʱ�ȭ ��
// ���ϰ�        : i_cPos������ �ʱ�ȭ�� ������ CBCUPosENU��
// ���          : 2005/06/07 �ۼ�
//***************************************************************************
CBCUPosENU CBCUPosENU::operator =(CBCUPos i_cPos)
{
	/**
	east�� i_cPos�� x���� �ִ´�.
	north�� i_cPos�� y���� �ִ´�.
	up�� i_cPos�� z���� �ִ´�.
	�ʱ�ȭ�� ���� ��ǥ ���� ��ȯ�Ѵ�.
	**/

	east = i_cPos.x;
	north = i_cPos.y;
	up = i_cPos.z;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CAxisTransformations

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CAxisTransformations
// ����          : CAxisTransformations Class�� ������
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CAxisTransformations::CAxisTransformations()
{
	///���� ����.
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ~CAxisTransformations
// ����          : CAxisTransformations Class�� �Ҹ���
// �Է°�        : �ش���� ����
// ���ϰ�        : �ش���� ����
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CAxisTransformations::~CAxisTransformations()
{
	///���� ����.
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CoordinatesRotate
// ����          : ��ǥ ȸ���� �����Ѵ�.
// �Է°�        : [IN] i_cPos - ȸ���� ��ǥ
//				   [IN] i_iDir - ȸ�� ���� (1:PITCH, 2:ROLL, 3:YAW, 4:X_AXIS, 5:Y_AXIS, 6:Z_AXIS) : ��ȸ���� �ݽð� ����
//				   [IN] i_dblAngleDeg - ȸ�� ���� (���� : Deg)
//				   [IN] i_bFlag - ���� (true : ��ǥ �̵�, false : ��ǥ�� �̵�)
// ���ϰ�        : ȸ�� ������ ��ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
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
		case X_AXIS:	//X���� �������� ȸ��
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case PITCH:	//PITCH //X���� �������� - ȸ��
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = - sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case Y_AXIS:	//Y���� �������� ȸ��
		case ROLL:	//ROLL
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.y = i_cPos.y;
			cFinalPos.z = -sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.z;
			break;
		case Z_AXIS:	//Z���� �������� ȸ��
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x - sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		case YAW:	//YAW //Z���� �������� - ȸ��
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
		case X_AXIS:	//X���� �������� ȸ��
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y + sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = -sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case PITCH:	//PITCH //X���� �������� - ȸ��
			cFinalPos.x = i_cPos.x;
			cFinalPos.y = cos(dblAngleRad)*i_cPos.y - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.y + cos(dblAngleRad)*i_cPos.z;
			break;
		case Y_AXIS:	//Y���� �������� ȸ��
		case ROLL:	//ROLL
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x - sin(dblAngleRad)*i_cPos.z;
			cFinalPos.y = i_cPos.y;
			cFinalPos.z = sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.z;
			break;
		case Z_AXIS:	//Z���� �������� ȸ��
			cFinalPos.x = cos(dblAngleRad)*i_cPos.x + sin(dblAngleRad)*i_cPos.y;
			cFinalPos.y = -sin(dblAngleRad)*i_cPos.x + cos(dblAngleRad)*i_cPos.y;
			cFinalPos.z = i_cPos.z;
			break;
		case YAW:	//YAW //Z���� �������� - ȸ��
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
		case X_AXIS:	//X���� �������� ȸ��
			[A] =	1			0				0
					0	cos(ȸ�� ����)	-sin(ȸ�� ����)
					0	sin(ȸ�� ����)	cos(ȸ�� ����)
			break;
		case PITCH:	//PITCH //X���� �������� - ȸ��
			[A] =	1			0				0
					0	cos(ȸ�� ����)	sin(ȸ�� ����)
					0	-sin(ȸ�� ����)	cos(ȸ�� ����)
			break;
		case Y_AXIS:	//Y���� �������� ȸ��
		case ROLL:	//ROLL
			[A] =	cos(ȸ�� ����)	0	sin(ȸ�� ����)
					0			1			0
					-sin(ȸ�� ����)	0	cos(ȸ�� ����)
			break;
		case Z_AXIS:	//Z���� �������� ȸ��
			[A] =	cos(ȸ�� ����)	-sin(ȸ�� ����)	0
					sin(ȸ�� ����)	cos(ȸ�� ����)	0
					0				1			0
			break;
		case YAW:	//YAW //Z���� �������� - ȸ��
		[A] =	cos(ȸ�� ����)	sin(ȸ�� ����)	0
				-sin(ȸ�� ����)	cos(ȸ�� ����)	0
				0				1			0
			break;
		default:
			i_cPos�� ��ȯ �Ѵ�.
			break;
		}
	}
	else
	{
		switch(i_iDir)
		{
			case X_AXIS:	//X���� �������� ȸ��
				[A] =	1			0				0
						0	cos(ȸ�� ����)	sin(ȸ�� ����)
						0	-sin(ȸ�� ����)	cos(ȸ�� ����)
				break;
			case PITCH:	//PITCH //X���� �������� - ȸ��
				[A] =	1			0				0
						0	cos(ȸ�� ����)	-sin(ȸ�� ����)
						0	sin(ȸ�� ����)	cos(ȸ�� ����)
				break;
			case Y_AXIS:	//Y���� �������� ȸ��
			case ROLL:	//ROLL
				[A] =	cos(ȸ�� ����)	0	-sin(ȸ�� ����)
						0			1			0
						sin(ȸ�� ����)	0	cos(ȸ�� ����)
				break;
			case Z_AXIS:	//Z���� �������� ȸ��
				[A] =	cos(ȸ�� ����)	sin(ȸ�� ����)	0
						-sin(ȸ�� ����)	cos(ȸ�� ����)	0
						0				1			0
				break;
			case YAW:	//YAW //Z���� �������� - ȸ��
				[A] =	cos(ȸ�� ����)	-sin(ȸ�� ����)	0
						sin(ȸ�� ����)	cos(ȸ�� ����)	0
						0				1			0
				break;
			default:
				i_cPos�� ��ȯ �Ѵ�.
				break;
		}
	}
	[B] = i_cPos.x
		  i_cPos.y
		  i_cPos.z

	[A][B] ���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : CoordinatesMove
// ����          : ��ǥ �̵��� �����Ѵ�.
// �Է°�        : [IN] i_cPos - �̵��� ��ǥ
//				   [IN] i_cDir - �̵��ҰŸ� �� ����
//				   [IN] i_bFlag - ����(true : ��ǥ �̵�, false : ��ǥ�� �̵�)
// ���ϰ�        : �̵��� ������ ��ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CAxisTransformations::CoordinatesMove(CBCUPos i_cPos, CBCUPos i_cDir, bool i_bFlag)
{
	/**
	if(i_bFlag)
	{
		i_cPos�� i_cDir�� ���Ѱ��� ��ȯ�Ѵ�.
	}
	else
	{
		i_cPos�� i_cDir�� ������ ��ȯ�Ѵ�.
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
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : Sphere2Orthogonal
// ����          : �� ��ǥ�� ���� ��ǥ�� ��ȯ
// �Է°�        : [IN] i_cSphere - ��ȯ�� ����ǥ��
// ���ϰ�        : ����ǥ���� ��ȯ�� ������ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
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
	��ȯ��.x = i_cSphere.range * cos(i_cSphere.elevation��) * sin(i_cSphere.bearing��);
	��ȯ��.y = i_cSphere.range * cos(i_cSphere.elevation��) * cos(i_cSphere.bearing��);
	��ȯ��.z = i_cSphere.range * sin(i_cSphere.elevation��);
	���� ���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : Orthogonal2Sphere
// ����          : ���� ��ǥ�� ����ǥ�� ��ȯ�Ѵ�.
// �Է°�        : [IN] i_cPos - ����ǥ�� ��ȯ�� ������ǥ ��ġ
// ���ϰ�        : ������ǥ���� ��ȯ�� ����ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
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
	Bearing�� i_cPos.x�� i_cPos.y�� �̷�� ����
	Elevation�� i_cPos.z�� sqrt(i_cPos.x^2 + i_cPos.y^2)�� �̷�� ����
	Range�� sqrt(i_cPos.x^2 + i_cPos.y^2 + i_cPos.z^2)
	���� �� ��ǥ���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : GetTilt
// ����          : ������ ���� �������� ȸ���Ҷ� Roll,Pitch���� ���� ȸ�� ���� ���Ѵ�
// �Է°�        : [IN] i_dblRollDeg - ���� Tilt���� Roll�� (���� : Deg)
//				   [IN] i_dblPitchDeg - ���� Tilt���� Pitch �� (���� : Deg)
// ���ϰ�        : ���� �������� ȸ���� �� (���� : Deg)
// ���          : OpenGL���� glRotatef(GetTilt(dRoll,dPitch),dPitch,dRoll,0);
//				   �ر����� ����ϴ� Roll,Pitch ��� ��
//					Roll = Tilt * sin(BHP);
//					Pitch = Tilt * cos(BHP);
//				   2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
double CAxisTransformations::GetTilt(double i_dblRollDeg, double i_dblPitchDeg)
{
	double dblBHPDeg;
	double dblTILT;
	double dblBHPRad;

	dblBHPRad = atan2(i_dblRollDeg,i_dblPitchDeg);
	dblBHPDeg = dblBHPRad * RAD2DEG;	//���� BHP��
	/*
	if((-180. <= dblBHPDeg) && (180. >= dblBHPDeg) && (0. != i_dblRollDeg))	//dRoll�� 0�� �ƴϸ� dBHP�� 0�� �ƴ�
	{
		dblTILT = i_dblRollDeg / sin(dblBHPRad);	//��� �ִ� ������ Ǯ�� ��
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
	dblBHPDeg = atan2(i_dblPitchDeg,i_dblRollDeg) * RAD2DEG;	//BHP�� 90���� ��

	return dblTILT;
	/**
	PHP : Roll���� Pitch���� �̷�� ��
	Roll = Tilt * sin(BHP)
	Pitch = Tilt * cos(BHP)
	�� ���� �̿��Ͽ� Tilt���� ���Ѵ�.
	������ Tilt���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : Quaternions
// ����          : ���� �������� ��ǥ�� ȸ�� ��Ų��.
// �Է°�        : [IN] i_cPos - �̵��� ��ǥ��
//				   [IN] i_cAxis - ���� ��
//				   [IN] i_dblAngleDeg - �̵��� ���� (���� : Deg)
// ���ϰ�        : ���� �������� ȸ���� ��ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
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

	/*	//�Ʒ� ������ ����ϸ� ���� ������ �� ������ ��귮�� ����
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
	��ȯ��.x = i_cPos.x*q11 + i_cPos.y*q21 + i_cPos.z*q31;
	��ȯ��.y = i_cPos.x*q12 + i_cPos.y*q22 + i_cPos.z*q32;
	��ȯ��.z = i_cPos.x*q13 + i_cPos.y*q23 + i_cPos.z*q33;

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

	���� ������� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : GetPositionAfterRollPitchYaw
// ����          : Roll, Pitch, Yaw�� ���� ������ ��ǥ�� ����Ѵ�.
// �Է°�        : [IN] i_dblRollDeg - ������ Roll�� (���� : Deg)
//				   [IN] i_dblPitchDeg - ������ Pitch �� (���� : Deg)
//				   [IN] i_dblYawDeg - ������ Yaw �� (���� : Deg)
//				   [IN] i_cPos - ��ȯ�� ��ǥ
// ���ϰ�        : Roll,Pitch,Yaw�� ���� ������ ��ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CAxisTransformations::GetPositionAfterRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	i_cPos = CoordinatesRotate(i_cPos, ROLL, i_dblRollDeg);
	i_cPos = CoordinatesRotate(i_cPos, PITCH, i_dblPitchDeg);
	i_cPos = CoordinatesRotate(i_cPos, YAW, i_dblYawDeg);

	return i_cPos;
	/**
	Roll�� ���� ��ǥ ȸ���� �����Ѵ�.
	Pitch�� ���� ��ǥ ȸ���� �����Ѵ�.
	Yaw�� ���� ��ǥ ȸ���� �����Ѵ�.
	��ȯ�� ���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/12/21
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : GetPositionBeforeRollPitchYaw
// ����          : Roll, Pitch, Yaw�� �߻��ϱ� ������ ��ǥ�� ����Ѵ�.
// �Է°�        : [IN] i_dblRollDeg - ������ Roll�� (���� : Deg)
//				   [IN] i_dblPitchDeg - ������ Pitch �� (���� : Deg)
//				   [IN] i_dblYawDeg - ������ Yaw �� (���� : Deg)
//				   [IN] i_cPos - ��ȯ�� ��ǥ
// ���ϰ�        : Roll,Pitch,Yaw�� �߻��ϱ� ������ ��ǥ��
// ���          : 2004/12/21 �ۼ�
//				   2005/05/23 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
CBCUPos CAxisTransformations::GetPositionBeforeRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	i_cPos = CoordinatesRotate(i_cPos, YAW, i_dblYawDeg, false);
	i_cPos = CoordinatesRotate(i_cPos, PITCH, i_dblPitchDeg, false);
	i_cPos = CoordinatesRotate(i_cPos, ROLL, i_dblRollDeg, false) ;

	return i_cPos;
	/**
	Yaw�� ���� ��ǥ ȸ���� �����Ѵ� - �ݴ� ����.
	Pitch�� ���� ��ǥ ȸ���� �����Ѵ� - �ݴ� ����.
	Roll�� ���� ��ǥ ȸ���� �����Ѵ� - �ݴ� ����.
	��ȯ�� ���� ��ȯ�Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2006/04/20
// ������ ������ : 2006/04/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ChangePosNorthToDeck
// ����          : ���� ������ ��ǥ�� Deck ������ ��ȯ �Ѵ�.
// �Է°�        : [IN] i_dblRollDeg - ������ Roll�� (���� : Deg)
//				   [IN] i_dblPitchDeg - ������ Pitch �� (���� : Deg)
//				   [IN] i_dblYawDeg - ������ Yaw �� (���� : Deg)
//				   [IN] i_cPos - ���� ������ ��ġ
// ���ϰ�        : Deck ������ ��ġ
// ���          :
//***************************************************************************
CBCUPos CAxisTransformations::ChangePosNorthToDeck(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	return GetPositionBeforeRollPitchYaw(i_dblRollDeg,i_dblPitchDeg,i_dblYawDeg,i_cPos);
	/**
	GetPositionBeforeRollPitchYaw �Լ��� �����Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2006/04/20
// ������ ������ : 2006/04/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : ChangePosDeckToNorth
// ����          : Deck ������ ��ǥ�� ���� ������ ��ǥ�� ��ȯ �Ѵ�.
// �Է°�        : [IN] i_dblRollDeg - ������ Roll�� (���� : Deg)
//				   [IN] i_dblPitchDeg - ������ Pitch �� (���� : Deg)
//				   [IN] i_dblYawDeg - ������ Yaw �� (���� : Deg)
//				   [IN] i_cPos - Deck ������ ��ġ
// ���ϰ�        : ���� ������ ��ġ
// ���          :
//***************************************************************************
CBCUPos CAxisTransformations::ChangePosDeckToNorth(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos)
{
	return GetPositionAfterRollPitchYaw(i_dblRollDeg,i_dblPitchDeg,i_dblYawDeg,i_cPos);
	/**
	GetPositionAfterRollPitchYaw �Լ��� �����Ѵ�.
	**/
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2006/04/20
// ������ ������ : 2006/04/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// �Լ���        : RotateToRangeDirection
// ����          : ǥ���� ���� Range �������� ��ǥ�� ��ȯ�Ѵ�.
// �Է°�        : [IN] i_cTargetPos - ǥ���� ��ġ
//				   [IN] i_cPos - ���� ��ġ
// ���ϰ�        : ǥ���� Range ���⿡ ���� ǥ���� ��ǥ
// ���          :
//***************************************************************************
CBCUSphere CAxisTransformations::RotateToRangeDirection(CBCUPos i_cTargetPos, CBCUPos i_cPos)
{
	CBCUSphere cSphere;

	cSphere = Orthogonal2Sphere(i_cTargetPos);	//Pos

	///Z�� �������� ǥ���� Bearing ��ŭ ȸ�� �Ѵ�.
	i_cPos = CoordinatesRotate(i_cPos,Z_AXIS,cSphere.bearing);
	///X�� �������� ǥ���� -Elevation ��ŭ ȸ�� �Ѵ�.
	i_cPos = CoordinatesRotate(i_cPos,X_AXIS,-cSphere.elevation);

	///����ǥ�� ��ȯ�Ͽ� ��ȯ�Ѵ�.
	return CBCUSphere(i_cPos.y,fabs(atan2(i_cPos.x,cSphere.range)) * RAD2DEG,fabs(atan2(i_cPos.z,cSphere.range)) * RAD2DEG);
}

//***************************************************************************
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/03/05
// ������ ������ : 2006/12/13
// ���� �ۼ���   : ����ü�� System �׷�/M&S ������
// ������ ������ : ����ü�� System �׷�/M&S ������
// �Լ���        : Sph2Cart
// ����          : ����ǥ���� ���� ���� ��ǥ��� ����(Elevation, Z ���)
// �Է°�        : [IN] RX - Range data
//                 [IN] BY - Bearing data
//                 [IN] EZ - Elevation data
//                 [IN] XX - X axis data
//                 [IN] YY - Y axis data
//                 [IN] ZZ - Z axis data
// ���ϰ�        : �ش���� ����
// ���          : 2009/01/16 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
void CAxisTransformations::Sph2Cart(double RX, double BY, double EZ, double &XX, double &YY, double &ZZ)
{
/// Range, Bearing, Elevation ���� �̿��� X, Y, Z data�� ���
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
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/03/05
// ������ ������ : 2006/12/13
// ���� �ۼ���   : ����ü�� System �׷�/M&S ������
// ������ ������ : ����ü�� System �׷�/M&S ������
// �Լ���        : Pol2Cart
// ����          : ����ǥ���� ���� ���� ��ǥ��� ����(Elevation, Z ����)
// �Է°�        : [IN] RX - Range data
//                 [IN] BY - Bearing data
//                 [IN] EZ - Elevation data
//                 [IN] XX - X axis data
//                 [IN] YY - Y axis data
//                 [IN] ZZ - Z axis data
// ���ϰ�        : �ش���� ����
// ���          : 2009/01/16 ���� ��� ���� �� �ּ� �߰�
//*************************************************************************** 
void CAxisTransformations::Pol2Cart(double RX, double BY, double EZ, double &XX, double &YY, double &ZZ)
{
/// Range, Bearing, ���� �̿��� X, Y data�� ���
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
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/03/05
// ������ ������ : 2006/12/13
// ���� �ۼ���   : ����ü�� System �׷�/M&S ������
// ������ ������ : ����ü�� System �׷�/M&S ������
// �Լ���        : Cart2Sph
// ����          : ������ǥ���� ���� ����ǥ��� ����(Elevation, Z ���)
// �Է°�        : [IN] X - X axis data
//                 [IN] Y - Y axis data
//                 [IN] Z - Z axis data
//                 [IN] R - Range data
//                 [IN] B - Bearing data
//                 [IN] E - Elevation data
// ���ϰ�        : �ش���� ����
// ���          : 2009/01/16 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
void CAxisTransformations::Cart2Sph(double X, double Y, double Z, double &R, double &B, double &E)
{
/**
X, Y, Z data�� �̿��� Range, Bearing, Elevation ���
**/
	double dblTmpB1, dblTmpB2, dblTmpB3;

	R = sqrt(pow(X,2.0) + pow(Y,2.0) + pow(Z,2.0));

	if((R>0.)||(R<0.))
	{
		E = asin(Z/R);
		
		// bearing�� ���Ѵ�.
		dblTmpB1 = atan2(Y, X);
		// +, - ���� �ٲ۴�.
		dblTmpB2 = dblTmpB1 * -1.;
		// ���°��� 90���� �ջ��Ͽ� y�� �������� �ٲ۴�.
		dblTmpB3 = dblTmpB2 + PI/2.;
		// ������� +pi Ȥ�� -pi���� ũ�� �ٽ� ����ؼ� -pi < ����� < pi ���� �����Ѵ�.
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
// ���ϸ�        : CAxisTransformations.cpp
// ���� �ۼ���   : 2004/03/05
// ������ ������ : 2006/12/13
// ���� �ۼ���   : ����ü�� System �׷�/M&S ������
// ������ ������ : ����ü�� System �׷�/M&S ������
// �Լ���        : Cart2Pol
// ����          : ������ǥ���� ���� ����ǥ��� ����(Elevation, Z ����)
// �Է°�        : [IN] X - X axis data
//                 [IN] Y - Y axis data
//                 [IN] Z - Z axis data
//                 [IN] R - Range data
//                 [IN] B - Bearing data
//                 [IN] E - Elevation data
// ���ϰ�        : �ش���� ����
// ���          : 2009/01/16 ���� ��� ���� �� �ּ� �߰�
//***************************************************************************
void CAxisTransformations::Cart2Pol(double X, double Y, double Z, double &R, double &B, double &E)
{
/**
X, Y data�� �̿��� Range, Bearing ���
**/
	double dblTmpB1, dblTmpB2, dblTmpB3;

	E = 0.;

	R = sqrt( pow(X,2.0) + pow(Y,2.0) );
	
	if((R>0.)||(R<0.))
	{
		// bearing�� ���Ѵ�.
		dblTmpB1 = atan2(Y, X);
		// +, - ���� �ٲ۴�.
		dblTmpB2 = dblTmpB1 * -1.;
		// ���°��� 90���� �ջ��Ͽ� y�� �������� �ٲ۴�.
		dblTmpB3 = dblTmpB2 + PI/2.;

		// ������� +pi Ȥ�� -pi���� ũ�� �ٽ� ����ؼ� -pi < ����� < pi ���� �����Ѵ�.
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