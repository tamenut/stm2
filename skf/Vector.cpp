// vector.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>
#include "Vector.h"
#include <string>
#include "assert.h"

#define SQR(a) ( (a) == 0. ? 0. : (a)*(a) )
#define SIGN(a,b) ((b) > 0. ? fabs(a) : -fabs(a))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVector::CVector()
{
	m_pData = NULL;
	m_nDimension = 0;
}

CVector::CVector(int nDimension)
{
	CreateVector(nDimension);
}

CVector::CVector(const CVector &aVector)
{
	m_nDimension = aVector.m_nDimension;
	m_pData = new double[m_nDimension];

	for(int i=0;i<m_nDimension;i++)
		m_pData[i] = aVector.m_pData[i];
}

CVector::~CVector()
{
	if(m_pData) delete[] m_pData;
}

CVector& CVector::operator=(const CVector &aVector)
{
	if(m_pData) delete [] m_pData;
	m_nDimension = aVector.m_nDimension;
	m_pData = new double[m_nDimension];
	
	memcpy(m_pData, aVector.m_pData, sizeof(double)*m_nDimension);

	return *this;
}

CVector CVector::operator*(double scalar) const
{
	CVector result_vector(m_nDimension);
	for(int i=0; i<m_nDimension; i++)
		result_vector.m_pData[i] = m_pData[i]*scalar;

	return result_vector;
}

double CVector::operator *(const CVector &aVector) const
{
	assert(m_nDimension == aVector.m_nDimension);

	double result = 0.0;
	for(int i=0; i<m_nDimension; i++)
		result += m_pData[i]*aVector.m_pData[i];

	return result;
}
/*
CMatrix CVector::operator ^(const CVector &aVector) const
{
	ASSERT(m_nDimension == aVector.m_nDimension);

	CMatrix result(m_nDimension, m_nDimension);
	for(int i=0; i<m_nDimension; i++)
		for(int j=0; j<m_nDimension; j++)
			result.m_pMatrix[i][j] = m_pData[i]*aVector.m_pData[j];

	return result;
}
*/
CVector CVector::operator +(const CVector &aVector) const
{
	assert(m_nDimension == aVector.m_nDimension);

	CVector result_vector(m_nDimension);
	for(int i=0;i<m_nDimension;i++)
		result_vector.m_pData[i] = m_pData[i]+aVector.m_pData[i];

	return result_vector;
}

CVector CVector::operator -(const CVector &aVector) const
{
	assert(m_nDimension == aVector.m_nDimension);

	CVector result_vector(m_nDimension);
	for(int i=0;i<m_nDimension;i++)
		result_vector.m_pData[i] = m_pData[i]-aVector.m_pData[i];

	return result_vector;
}

double CVector::operator [] (int row)
{
	assert( row >= 0 && row < m_nDimension );
	return m_pData[row];
}

void CVector::CreateVector(int nDimension)
{
	m_nDimension = nDimension;
	m_pData = new double[nDimension];
	memset(m_pData, 0, sizeof(double)*m_nDimension);
}
