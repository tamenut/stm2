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
// ���ϸ�        : CAxisTransformations.h
// ������Ʈ��    : PKX BCU - 76mm,40mm �ຯȯ
// ���� �ۼ���   : 2003/12/05
// ������ ������ : 2005/05/23
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// Ŭ������      : CBCUPos
// Ŭ���� ����   : ������ǥ�� ��Ÿ���� Class�� x��, y��, z�� ���� �Ѳ����� ���ϰų� ���ų� �ʱ�ȭ ��Ű�� ����� �����Ѵ�.
// ���          :
//***************************************************************************
class CBCUPos
{
//�Լ�
public:
	CBCUPos(double i_dblX = 0., double i_dblY = 0., double i_dblZ = 0.);	//������
	~CBCUPos();	//�Ҹ���
	CBCUPos operator+(CBCUPos i_cPos);	//CBCUPos ���� ���� �ش�.
	CBCUPos operator-(CBCUPos i_cPos);	//CBCUPos ���� �� �ش�.
	CBCUPos operator=(CBCUPos i_cPos);	//CBCUPos ���� �޾Ƽ� �װ����� �����Ѵ�.
	CBCUPos operator=(double i_dblPos);	//double ���� �޾Ƽ� �װ����� �ʱ�ȭ �Ѵ�.
	CBCUPos operator=(CBCUPosENU i_cPos);	//CBCUPosENU ���� �޾Ƽ� �װ����� �����Ѵ�.

//����
public:
	double x;	//X ���� ��ġ��
	double y;	//Y ���� ��ġ��
	double z;	//Z ���� ��ġ��
};

//***************************************************************************
// ���ϸ�        : CAxisTransformations.h
// ������Ʈ��    : PKX BCU - 76mm,40mm �ຯȯ
// ���� �ۼ���   : 2003/12/05
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// Ŭ������      : CBCUSphere
// Ŭ���� ����   : ����ǥ�� ��Ÿ���� Class�� Bearing, Elevation, Range ���� �Ѳ����� ���ϰų� ���ų� �ʱ�ȭ ��Ű�� ����� �����Ѵ�.
// ���          : Bearing, Elevation���� Deg���̴�.
//***************************************************************************
class CBCUSphere
{
//�Լ�
public:
	CBCUSphere(double i_dblRange = 0., double i_dblBearing = 0., double i_dblElevation = 0.);	//������.
	~CBCUSphere();	//�Ҹ���.
	CBCUSphere operator+(CBCUSphere i_cSphere);	//CBCUSphere ���� ���� �ش�.
	CBCUSphere operator-(CBCUSphere i_cSphere);	//CBCUSphere ���� �� �ش�.
	CBCUSphere operator=(CBCUSphere i_cSphere);	//CBCUSphere ���� �޾Ƽ� �װ����� �����Ѵ�.
	CBCUSphere operator=(double i_dblPos);	//double ���� �޾Ƽ� �װ����� �ʱ�ȭ �Ѵ�.

//����
public:
	double elevation;	//������ ��ġ��
	double bearing;		//�� ��ġ�� - ���� ���� �ð� ���� ����
	double range;		//�Ÿ�
};


//***************************************************************************
// ���ϸ�        : CAxisTransformations.h
// ������Ʈ��    : PKX BCU - 76mm,40mm �ຯȯ
// ���� �ۼ���   : 2005/06/07
// ������ ������ : 2005/06/07
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// Ŭ������      : CBCUPosENU
// Ŭ���� ����   : ������ǥ�� ��Ÿ���� Class�� east��, north��, up�� ���� �Ѳ����� ���ϰų� ���ų� �ʱ�ȭ ��Ű�� ����� �����Ѵ�.
// ���          :
//***************************************************************************
class CBCUPosENU
{
//�Լ�
public:
	CBCUPosENU(double i_dblEast = 0., double i_dblNorth = 0., double i_dblUp = 0.);	//������.
	~CBCUPosENU();	//�Ҹ���.
	CBCUPosENU operator+(CBCUPosENU i_cPos);	//CBCUPosENU ���� ���� �ش�.
	CBCUPosENU operator-(CBCUPosENU i_cPos);	//CBCUPosENU ���� �� �ش�.
	CBCUPosENU operator=(CBCUPosENU i_cPos);	//CBCUPosENU ���� �޾Ƽ� �װ����� �����Ѵ�.
	CBCUPosENU operator=(double i_dblPos);	//double ���� �޾Ƽ� �װ����� �ʱ�ȭ �Ѵ�.
	CBCUPosENU operator=(CBCUPos i_cPos);	//CBCUPos ���� �޾Ƽ� �װ����� �����Ѵ�.

//����
public:
	double east;	//X ���� ��ġ��
	double north;	//Y ���� ��ġ��
	double up;	//Z ���� ��ġ��
};


//***************************************************************************
// ���ϸ�        : CAxisTransformations.h
// ������Ʈ��    : PKX BCU - 76mm, 40mm �ຯȯ
// ���� �ۼ���   : 2003/12/05
// ������ ������ : 2005/05/20
// ���� �ۼ���   : SW�� SW2�׷�/PKX SW ��õȯ
// ������ ������ : SW�� SW2�׷�/PKX SW ��õȯ
// Ŭ������      : CBCUAxisTransformations
// Ŭ���� ����   : ��ǥ ��ȯ�� ����ϴ� Class�� ������ǥ���� ������ǥ���� ��ȯ, ������ǥ���� ������ǥ���� ��ȯ �� ������ǥ�� �̵� �� ȸ���ϴ� ����� �����ϴ� �Լ� �̴�.
// ���          : ��ǥ��� ENU��ǥ�� - East(X)North(Y)UP(Z)
//                 Heading - ������ �������� �Լ��� ���������� ��������
//                 Pitch - ������ Heaging���� ���� ���¿��� ������ ����� �Ʒ��� ������ ��
//                 Roll - Heading,Pitch�� ���¿��� ������ �Ʒ��� ������ ��
//				   ȸ������ - �ݽð� ����
//***************************************************************************
class CAxisTransformations
{

//�Լ�
public:

	CAxisTransformations();	//������.
	virtual ~CAxisTransformations();	//�Ҹ���.
	CBCUPos ChangePosDeckToNorth(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Deck ������ ��ǥ�� ���� ������ ��ǥ�� ��ȯ �Ѵ�.
	CBCUPos ChangePosNorthToDeck(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//���� ������ ��ǥ�� Deck ������ ��ȯ �Ѵ�.
	CBCUPos CoordinatesMove(CBCUPos i_cPos, CBCUPos i_cDir, bool i_bFlag = true);	//��ǥ �̵��� �����Ѵ�.
	CBCUPos CoordinatesRotate(CBCUPos i_cPos, int i_iDir, double i_dblDeg , bool i_bFlag = true);	//��ǥ ȸ���� �����Ѵ�.
	CBCUPos GetPositionAfterRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Roll, Pitch, Yaw�� ���� ������ ��ǥ�� ����Ѵ�.
	CBCUPos GetPositionBeforeRollPitchYaw(double i_dblRollDeg, double i_dblPitchDeg, double i_dblYawDeg ,CBCUPos i_cPos);	//Roll, Pitch, Yaw�� �Ͼ�� ���� ������ ��ǥ�� ����Ѵ�.
	double GetTilt(double i_dblRoll, double i_dblPitch);	//������ ���� �������� ȸ���Ҷ� Roll,Pitch���� ���� ȸ�� ���� ���Ѵ�.
	CBCUSphere Orthogonal2Sphere(CBCUPos i_cPos);	//���� ��ǥ�� ����ǥ�� ��ȯ�Ѵ�.
	CBCUPos Quaternions(CBCUPos i_cPos, CBCUPos i_cAxis, double i_dblAngle);	//���� �������� ��ǥ�� ȸ�� ��Ų��.
	CBCUSphere RotateToRangeDirection(CBCUPos i_cTargetPos, CBCUPos i_cPos);	//ǥ���� ���� Range �������� ��ǥ�� ��ȯ�Ѵ�.
	CBCUPos Sphere2Orthogonal(CBCUSphere i_cSphere);	//�� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.

	void Cart2Sph(double ,double ,double ,double& ,double& ,double& );
	void Cart2Pol(double ,double ,double ,double& ,double& ,double& );

	void Sph2Cart(double ,double ,double ,double& ,double& ,double& );
	void Pol2Cart(double ,double ,double ,double& ,double& ,double& ); 
};

#endif // !defined(AFX_CAXISTRANSFORMATIONS_H__35C66458_F4DB_48A7_9FC6_57E205C55D10__INCLUDED_)

