// CMatUtil.cpp: implementation of the CMatUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "CMatUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatUtil::CMatUtil()
{

}

CMatUtil::~CMatUtil()
{

}

void CMatUtil::addition_31(double out_mat[3], double add_one[3], double add_two[3])
{
	int i;

	double temp_1[3], temp_2[3];

	equal_31(temp_1, add_one);
	equal_31(temp_2, add_two);

	for(i = 0; i < 3; i++)
	{
		out_mat[i] = 0.0;
		out_mat[i] = temp_1[i] + temp_2[i];
	}
}

void CMatUtil::addition_66(double out_mat[][6], double add_one[][6], double add_two[][6])
{
	int i, j;

	double temp_1[6][6], temp_2[6][6];

	equal_66(temp_1, add_one);
	equal_66(temp_2, add_two);

	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 6; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp_1[i][j] + temp_2[i][j];
		}
	}
}

void CMatUtil::addition_99(double out_mat[][9], double add_one[][9], double add_two[][9])
{
	int i, j;

	double temp_1[9][9], temp_2[9][9];

	equal_99(temp_1, add_one);
	equal_99(temp_2, add_two);

	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp_1[i][j] + temp_2[i][j];
		}
	}
}

void CMatUtil::det_22(double &out, double in[2][2])
{
	out = in[0][0]*in[1][1] - in[0][1]*in[1][0];
}

void CMatUtil::equal_22(double out[][2], double in[][2])
{
	int i,j;
	for(i=0;i<2;i++)
		for(j=0;j<2;j++)
			out[i][j]=in[i][j];
}

void CMatUtil::equal_26(double out[][6], double in[][6])
{
	int i, j;

	for(i = 0; i < 2; i++)
		for(j = 0; j < 6; j++)
			out[i][j] = in[i][j];
}

void CMatUtil::equal_31(double out[3], double in[1])
{
	int i;
	for(i = 0; i < 3; i++)
		out[i] = in[i];
}

void CMatUtil::equal_33(double out[3][3], double in[3][3])
{
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			out[i][j]=in[i][j];
}

void CMatUtil::equal_39(double out[][9], double in[][9])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 9; j++)
			out[i][j] = in[i][j];
}

void CMatUtil::equal_61(double out[], double in[])
{
	int i;
	for(i = 0; i < 6; i++)
		out[i] = in[i];
}

void CMatUtil::equal_62(double out[][2], double in[][2])
{
	int i, j;

	for(i = 0; i < 6; i++)
		for(j = 0; j < 2; j++)
			out[i][j] = in[i][j];
}

void CMatUtil::equal_66(double out[][6], double in[][6])
{
	int i, j;
	for(i = 0; i < 6; i++)
		for(j = 0; j < 6; j++)
			out[i][j] = in[i][j];
}

void CMatUtil::equal_91(double out[9], double in[9])
{
	int i;
	for(i = 0; i < 9; i++)
		out[i] = in[i];
}

void CMatUtil::equal_93(double out[][3], double in[][3])
{
	int i, j;

	for(i = 0; i < 9; i++)
		for(j = 0; j < 3; j++)
			out[i][j] = in[i][j];	
}

void CMatUtil::equal_99(double out[][9], double in[][9])
{
	int i, j;
	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
			out[i][j] = in[i][j];
}

void CMatUtil::Generation_33_Pitch_Axis_Transformation_Attitude(double out_mat[][3], double Pitch)
{
	initialize_33(out_mat);

	out_mat[1][1] = 1.;
	out_mat[0][0] = cos(Pitch);
	out_mat[2][2] = out_mat[0][0];
	out_mat[2][0] = sin(Pitch);
	out_mat[0][2] = -out_mat[2][0];
}

void CMatUtil::Generation_33_Roll_Axis_Transformation_Attitude(double out_mat[][3], double Roll)
{
	initialize_33(out_mat);

	out_mat[0][0] = 1.;
	out_mat[1][1] = cos(Roll);
	out_mat[1][2] = sin(Roll);
	out_mat[2][1] = -out_mat[1][2];
	out_mat[2][2] = out_mat[1][1];
}

void CMatUtil::Generation_33_Yaw_Axis_Transformation_Attitude(double out_mat[][3], double Yaw)
{
	initialize_33(out_mat);

	out_mat[0][0] = cos(Yaw);
	out_mat[0][1] = sin(Yaw);
	out_mat[2][2] = 1.;
	out_mat[1][0] = -out_mat[0][1];
	out_mat[1][1] = out_mat[0][0];
}

void CMatUtil::I_matrix(double iden[][3])
{
	int i, j;

	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			if (i==j)
				iden[i][j] = 1. ;
			
			else
				iden[i][j] = 0. ;
		}
	}
}

void CMatUtil::initialize_31(double ini[3])
{
	int i;
	for(i=0;i<3;i++)
		ini[i]=0.0;
}

void CMatUtil::initialize_33(double ini[][3])
{
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			ini[i][j]=0.0;
}

void CMatUtil::inverse_22(double out_mat[2][2], double in_mat[2][2])
{
	double det;

	det=in_mat[0][0]*in_mat[1][1]-in_mat[0][1]*in_mat[1][0];

	out_mat[0][0]=in_mat[1][1]/det;
	out_mat[0][1]=-1.f*in_mat[0][1]/det;
	out_mat[1][0]=-1.f*in_mat[1][0]/det;
	out_mat[1][1]=in_mat[0][0]/det;	
}

void CMatUtil::inverse_33(double out_mat[][3], double inverse_in[][3])
{
	double u[3][3] = {{0.,},};
	double l[3][3] = {{0.,},};

	mat_equal(inverse_in, u);			// matrix inverse_in ==> matrix u
	lu_decomp(l,u);				// matrix u 를 L-part 와 U-part로 나눔
	inverse_mat(l, u, out_mat);		// matrix a의 inverse matrix를 구한다.	
}

void CMatUtil::inverse_mat(double l[][3], double u[][3], double out_mat[][3])
{
	int		i,j;	
	double	b[3]={0.,};
	double  s[3]={0.,};
	double	iden[3][3];

	I_matrix(iden);

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			b[j]= iden[i][j];
			substitution(l, u, b, s);
		}
		for(j=0; j<3; j++)
			out_mat[j][i]=s[j];
	}	
}

void CMatUtil::lu_decomp(double l[][3], double u[][3])
{
	int	i,j,k;
	double m[3][3] = {{0.,},};

	for(i=0; i<2; i++)		//---------Gaussian Elimination( Upper triangular)
	{
		if(u[i][i]==0.)	
				break ;
		
		for(k=i+1; k<3; k++)
		{
			m[k][i] = u[k][i]/u[i][i] ;

			for(j=i;j<3;j++)
				u[k][j] = u[k][j] - m[k][i]*u[i][j] ;
		}
	}	
		
	
	for(i=0; i<3; i++)		//---------Lower triangular
	{
		for(k=0; k<3; k++)
		{
				if(i==k)		l[i][k] = 1. ;
				else if(k>i)	l[i][k] = 0. ;
				else			l[i][k] = m[i][k] ;
		}
	}		
}

void CMatUtil::mat_equal(double inverse_in[][3], double u[][3])
{
	int	i, j;
	
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
			u[i][j]=inverse_in[i][j];
	}
}

void CMatUtil::multi_22_22(double out_mat[][2], double multi_one[][2], double multi_two[][2])
{
	int i, j, k;

	double temp_1[2][2], temp_2[2][2];

	equal_22(temp_1, multi_one);
	equal_22(temp_2, multi_two);
	
	for(j = 0; j < 2; j++)
	{
		for(k = 0; k < 2; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 2; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::multi_26_62(double out_mat[][2], double multi_one[][6], double multi_two[][2])
{
	int i, j, k;

	double temp_1[2][6], temp_2[6][2];

	equal_26(temp_1, multi_one);
	equal_62(temp_2, multi_two);
	
	for(j = 0; j < 2; j++)
	{
		for(k = 0; k < 2; k++)
		{
			out_mat[j][k] = 0.0;

			for(i = 0; i < 6; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::multi_26_66(double out_mat[][6], double multi_one[][6], double multi_two[][6])
{
	int i, j, k;

	double temp_one[2][6];

	equal_26(temp_one, multi_one);
	
	for(j = 0; j < 2; j++)
	{
		for(k = 0; k < 6; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 6; i++)
				out_mat[j][k] += temp_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_33_31(double out_mat[3], double multi_one[3][3], double multi_two[3])
{
	int i, j;

	double temp[3];

	equal_31(temp, multi_two);

	for(j = 0; j < 3; j++)
	{
		out_mat[j] = 0.;
		for(i = 0; i < 3; i++)
			out_mat[j] += multi_one[j][i] * temp[i];			
	}
}

void CMatUtil::multi_33_33(double out_mat[][3], double multi_one[][3], double multi_two[][3])
{
	int i, j, k;

	double temp_1[3][3], temp_2[3][3];

	equal_33(temp_1, multi_one);
	equal_33(temp_2, multi_two);
	
	for(j = 0; j < 3; j++)
	{
		for(k = 0; k < 3; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 3; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::multi_39_93(double out_mat[][3], double multi_one[][9], double multi_two[][3])
{
	int i, j, k;

	double temp_1[3][9], temp_2[9][3];

	equal_39(temp_1, multi_one);
	equal_93(temp_2, multi_two);
	
	for(j = 0; j < 3; j++)
	{
		for(k = 0; k < 3; k++)
		{
			out_mat[j][k] = 0.0;

			for(i = 0; i < 9; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::multi_39_99(double out_mat[3][9], double multi_one[3][9], double multi_two[9][9])
{
	int i, j, k;

	double temp_one[3][9];

	equal_39(temp_one, multi_one);
	
	for(j = 0; j < 3; j++)
	{
		for(k = 0; k < 9; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 9; i++)
				out_mat[j][k] += temp_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_62_21(double out_mat[], double multi_one[][2], double multi_two[])
{
	int i, j;

	for(j = 0; j < 6; j++)
	{
		out_mat[j] = 0.;
		for(i = 0; i < 2; i++)
			out_mat[j] += multi_one[j][i] * multi_two[i];
	}
}

void CMatUtil::multi_62_22(double out_mat[][2], double multi_one[][2], double multi_two[][2])
{
	int i, j, k;

	double temp_one[6][2];

	equal_62(temp_one, multi_one);
	
	for(j = 0; j < 6; j++)
	{
		for(k = 0; k < 2; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 2; i++)
				out_mat[j][k] += temp_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_62_26(double out_mat[][6], double multi_one[][2], double multi_two[][6])
{
	int i, j, k;
	
	for(j = 0; j < 6; j++)
	{
		for(k = 0; k < 6; k++)
		{
			out_mat[j][k] = 0.0;
			for(i = 0; i < 2; i++)
				out_mat[j][k] += multi_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_66_61(double out_mat[], double multi_one[][6], double multi_two[])
{
	int i, j;

	double temp[6];

	equal_61(temp, multi_two);

	for(j = 0; j < 6; j++)
	{
		out_mat[j] = 0.;
		for(i = 0; i < 6; i++)
			out_mat[j] += multi_one[j][i] * temp[i];			
	}
}

void CMatUtil::multi_66_62(double out_mat[][2], double multi_one[][6], double multi_two[][2])
{
	int i, j, k;
	double temp_two[6][2];

	equal_62(temp_two, multi_two);

	for(j = 0; j < 6; j++)
	{
		for(k = 0; k < 2; k++)
		{
			out_mat[j][k] = 0.;

			for(i = 0; i < 6; i++)
				out_mat[j][k] += multi_one[j][i] * temp_two[i][k];
		}
	}
}

void CMatUtil::multi_66_66(double out_mat[][6], double multi_one[][6], double multi_two[][6])
{
	int i, j, k;

	double temp_1[6][6], temp_2[6][6];

	equal_66(temp_1, multi_one);
	equal_66(temp_2, multi_two);
	
	for(j = 0; j < 6; j++)
	{
		for(k = 0; k < 6; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 6; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::multi_93_31(double out_mat[9], double multi_one[9][3], double multi_two[3])
{
	int i, j;

	for(j = 0; j < 9; j++)
	{
		out_mat[j] = 0.;
		for(i = 0; i < 3; i++)
			out_mat[j] += multi_one[j][i] * multi_two[i];
	}	
}

void CMatUtil::multi_93_33(double out_mat[][3], double multi_one[][3], double multi_two[][3])
{
	int i, j, k;

	double temp_one[9][3];

	equal_93(temp_one, multi_one);
	
	for(j = 0; j < 9; j++)
	{
		for(k = 0; k < 3; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 3; i++)
				out_mat[j][k] += temp_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_93_39(double out_mat[9][9], double multi_one[9][3], double multi_two[3][9])
{
	int i, j, k;
	
	for(j = 0; j < 9; j++)
	{
		for(k = 0; k < 9; k++)
		{
			out_mat[j][k] = 0.0;
			for(i = 0; i < 3; i++)
				out_mat[j][k] += multi_one[j][i] * multi_two[i][k];
		}
	}
}

void CMatUtil::multi_99_91(double out_mat[9], double multi_one[9][9], double multi_two[9])
{
	int i, j;

	double temp[9];

	equal_91(temp, multi_two);

	for(j = 0; j < 9; j++)
	{
		out_mat[j] = 0.;
		for(i = 0; i < 9; i++)
			out_mat[j] += multi_one[j][i] * temp[i];			
	}
}

void CMatUtil::multi_99_93(double out_mat[][3], double multi_one[][9], double multi_two[][3])
{
	int i, j, k;
	double temp_two[9][3];

	equal_93(temp_two, multi_two);

	for(j = 0; j < 9; j++)
	{
		for(k = 0; k < 3; k++)
		{
			out_mat[j][k] = 0.;

			for(i = 0; i < 9; i++)
				out_mat[j][k] += multi_one[j][i] * temp_two[i][k];
		}
	}
}

void CMatUtil::multi_99_99(double out_mat[][9], double multi_one[][9], double multi_two[][9])
{
	int i, j, k;

	double temp_1[9][9], temp_2[9][9];

	equal_99(temp_1, multi_one);
	equal_99(temp_2, multi_two);
	
	for(j = 0; j < 9; j++)
	{
		for(k = 0; k < 9; k++)
		{
			out_mat[j][k] = 0.;
			for(i = 0; i < 9; i++)
				out_mat[j][k] += temp_1[j][i] * temp_2[i][k];
		}
	}
}

void CMatUtil::substitution(double l[][3], double u[][3], double b[], double s[])
{
	int	i,k;
	double sum;
	double y[3]={0.,};
	

	for(i=0; i<3; i++)				//forward_substitution
	{
		sum=0.;
		
		for(k=0; k<i; k++)
			sum += l[i][k]*y[k] ;
		
		y[i] = b[i] - sum ;

	}	
	
	
	for(i=2; i>=0; i--)			//back_substitution	
	{
		sum=0.;
		
		if(u[i][i]==0.)
		{ 
			break;
		}
		
		for(k=i+1; k<3; k++)
			sum += u[i][k]*s[k] ;
		
		s[i] = (y[i] - sum)/u[i][i] ;
	}
}

void CMatUtil::subtraction_31(double out_mat[], double sub_one[], double sub_two[])
{
	int i;

	double temp_1[3], temp_2[3];

	equal_31(temp_1, sub_one);
	equal_31(temp_2, sub_two);
	
	for(i = 0; i < 3; i++)
	{
		out_mat[i] = 0.0;
		out_mat[i] = temp_1[i] - temp_2[i];
	}
}

void CMatUtil::transpose_22(double out_mat[][2], double transfer_in[][2])
{
	int i , j;

	double temp[2][2];

	equal_22(temp, transfer_in);
	
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 2; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp[j][i];
		}
	}
}

void CMatUtil::transpose_26(double out_mat[6][2], double transfer_in[2][6])
{
	int i, j;

	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 2; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = transfer_in[j][i];
		}
	}
}

void CMatUtil::transpose_33(double out_mat[][3], double transfer_in[][3])
{
	int i , j;

	double temp[3][3];

	equal_33(temp, transfer_in);
	
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp[j][i];
		}
	}
}

void CMatUtil::transpose_39(double out_mat[9][3], double transfer_in[3][9])
{
	int i, j;

	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 3; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = transfer_in[j][i];
		}
	}
}

void CMatUtil::transpose_62(double out_mat[][6], double transfer_in[][2])
{
	int i, j;

	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 6; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = transfer_in[j][i];
		}
	}
}

void CMatUtil::transpose_66(double out_mat[][6], double transfer_in[][6])
{
	int i, j;

	double temp[6][6];

	equal_66(temp, transfer_in);
	
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j < 6; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp[j][i];
		}
	}
}

void CMatUtil::transpose_93(double out_mat[3][9], double transfer_in[9][3])
{
	int i, j;

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 9; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = transfer_in[j][i];
		}
	}
}

void CMatUtil::transpose_99(double out_mat[][9], double transfer_in[][9])
{
	int i, j;

	double temp[9][9];

	equal_99(temp, transfer_in);
	
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			out_mat[i][j] = 0.0;
			out_mat[i][j] = temp[j][i];
		}
	}
}

void CMatUtil::vector61_cross_vector61_T(double out[][6], double vect[])
{
	int i, j;

	for(i=0; i<6; i++)
	{
		for(j=0; j<6; j++) 
		{
			out[i][j] = vect[i]*vect[j];
		}
	}
}

void CMatUtil::vector91_cross_vector91_T(double out[][9], double vect[])
{
	int i, j;

	for(i=0; i<9; i++)
	{
		for(j=0; j<9; j++) 
		{
			out[i][j] = vect[i]*vect[j];
		}
	}
}