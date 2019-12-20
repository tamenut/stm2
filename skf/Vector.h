// vector.h: interface for the CMatrix class.
// coded by hs.you
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VECTOR_H_)
#define _VECTOR_H_

class CVector
{
public:
	CVector();
	CVector(int nDimension);
	CVector(const CVector& aVector);
	virtual ~CVector();

// member functions
public:
	CVector  operator*(double scalar) const;
	double	 operator*(const CVector& aVector) const;
	CVector  operator-(const CVector& aVector) const;
	CVector& operator=(const CVector& aVector);
	CVector  operator+(const CVector& aVector) const;
//	CMatrix	 operator^(const CVector& aVector) const;
	double	 operator [] (int row);

	void CreateVector(int nDimension);	
	int GetDimension(){ return m_nDimension; };

protected:


// member variables
public:
	double* m_pData;
	
protected:
	int m_nDimension;
};

#endif // !defined(_VECTOR_H_)