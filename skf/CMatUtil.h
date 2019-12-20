// CMatUtil.h: interface for the CMatUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMATUTIL_H__2EDC4D28_2504_48F3_9E84_E908B51DFEAB__INCLUDED_)
#define AFX_CMATUTIL_H__2EDC4D28_2504_48F3_9E84_E908B51DFEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

class CMatUtil  
{
public:
	void substitution(double l[3][3], double u[3][3], double b[3], double s[3]);
	void I_matrix(double iden[3][3]);
	void inverse_mat(double l[3][3], double u[3][3], double out_mat[3][3]);
	void lu_decomp(double l[3][3],double u[3][3]);
	void mat_equal(double inverse_in[3][3],double u[3][3]);
	void multi_93_33(double out_mat[9][3], double multi_one[9][3],  double multi_two[3][3]);
	void inverse_33(double out_mat[3][3], double inverse_in[3][3]);
	void equal_93(double out[9][3], double in[9][3]);
	void equal_39(double out[3][9], double in[3][9]);
	void equal_66(double out[6][6], double in[6][6]);
	void multi_99_93(double out_mat[9][3], double multi_one[9][9],  double multi_two[9][3]);
	void multi_39_93(double out_mat[3][3], double multi_one[3][9],  double multi_two[9][3]);
	void multi_39_99(double out_mat[3][9], double multi_one[3][9],  double multi_two[9][9]);
	void transpose_39(double out_mat[9][3],double transfer_in[3][9]);
	void addition_31(double out_mat[3], double add_one[3], double add_two[3]);
	void initialize_31(double ini[3]);
	void multi_33_33(double out_mat[3][3], double multi_one[3][3],  double multi_two[3][3]);
	void Generation_33_Yaw_Axis_Transformation_Attitude(double out_mat[3][3], double Yaw);
	void subtraction_31(double out_mat[3], double sub_one[3], double sub_two[3]);
	void Generation_33_Pitch_Axis_Transformation_Attitude(double out_mat[3][3], double Pitch);
	void initialize_33(double ini[3][3]);
	void Generation_33_Roll_Axis_Transformation_Attitude(double out_mat[3][3], double Roll);
	void equal_31(double out[3], double in[3]);
	void equal_33(double out[3][3],double in[3][3]);
	void equal_91(double out[9],double in[9]);
	void equal_99(double out[9][9], double in[9][9]);
	void multi_99_99(double out_mat[9][9], double multi_one[9][9], double multi_two[9][9]);
	void transpose_99(double out_mat[9][9], double transfer_in[9][9]);
	void multi_93_31(double out_mat[9], double multi_one[9][3], double multi_twop3[3]);
	void multi_33_31(double out_mat[3], double multi_one[3][3], double multi_two[3]);
	void transpose_33(double out_mat[3][3], double transfer_in[3][3]);
	void multi_99_91(double out_mat[9], double multi_one[9][9], double multi_two[9]);
	void addition_99(double out_mat[9][9], double add_one[9][9], double add_two[9][9]);
	void vector91_cross_vector91_T(double out[9][9], double vect[9]);
	void multi_93_39(double out_mat[9][9], double multi_one[9][3], double multi_two[3][9]);
	void transpose_93(double out_mat[3][9], double transfer_in[9][3]);
public:
	void multi_62_21(double out_mat[6], double multi_one[6][2], double multi_two[2]);
	void multi_62_22(double out_mat[6][2], double multi_one[6][2], double multi_two[2][2]);
	void multi_66_62(double out_mat[6][2], double multi_one[6][6], double multi_two[6][2]);
	void equal_62(double out[6][2], double in[6][2]);
	void multi_26_62(double out_mat[2][2], double multi_one[2][6], double multi_two[6][2]);
	void equal_26(double out[2][6], double in[2][6]);
	void multi_26_66(double out_mat[2][6], double multi_one[2][6], double multi_two[6][6]);
	void transpose_26(double out_mat[6][2], double transfer_in[2][6]);
	void multi_66_66(double out_mat[6][6], double multi_one[6][6], double multi_two[6][6]);
	void transpose_66(double out_mat[6][6], double transfer_in[6][6]);
	void equal_61(double out[6], double in[1]);
	void multi_66_61(double out_mat[6], double multi_one[6][6], double multi_two[6]);
	void equal_22(double out[2][2], double in[2][2]);
	void transpose_22(double out_mat[2][2], double transfer_in[2][2]);
	void multi_22_22(double out_mat[2][2], double multi_one[2][2], double multi_two[2][2]);
	void multi_62_26(double out_mat[6][6], double multi_one[6][2], double multi_two[2][6]);
	void transpose_62(double out_mat[2][6], double transfer_in[6][2]);
	void addition_66(double out_mat[6][6], double add_one[6][6], double add_two[6][6]);
	void vector61_cross_vector61_T(double out[6][6], double vect[6]);
	void det_22(double& out,double in[2][2]);
	void inverse_22(double out_mat[2][2], double in_mat[2][2]);
	CMatUtil();
	virtual ~CMatUtil();

};

#endif // !defined(AFX_CMATUTIL_H__2EDC4D28_2504_48F3_9E84_E908B51DFEAB__INCLUDED_)
