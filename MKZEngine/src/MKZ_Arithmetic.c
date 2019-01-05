#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "MKZ_Geometry.h"
#include "MKZ_Arithmetic.h"

void MKZ_ARITHMETIC_to_cameraMatrix(float* transform_mat , float* camera_mat){

	MKZ_ARITHMETIC_identityMatrix(camera_mat);

	int i = 0;
	for (i = 0; i < 9 ; i++){
		camera_mat[((i%3)*4) + i/3] = transform_mat[ i%3 + 4*(i/3)];
	}

	for (i = 0 ; i < 3 ; i++ ){
		camera_mat[12 + i] = -MKZ_ARITHMETIC_dotProduct(transform_mat+i*4,transform_mat+12);
	}

}

void MKZ_ARITHMETIC_nulMatrix(float* mat){

	int i;
	for ( i = 0 ; i < 16 ; i++){
		mat[i] = 0;
	}

}

float * MKZ_ARITHMETIC_create_matrix(){
	float * mat = (float*) malloc(sizeof(float)*16);
	MKZ_ARITHMETIC_nulMatrix(mat);
	return mat;
}


void MKZ_ARITHMETIC_free_matrix(float * mat){
	free(mat);
}

void MKZ_ARITHMETIC_identityMatrix(float* mat){

	for (int i = 0 ; i < 16; i++) {
		mat[i] = ((i % 4) == (i / 4)) ? 1 : 0;
	}

}

float MKZ_ARITHMETIC_determinant(float * mat){

	float f = 0;

	f += mat[0]*mat[5]*mat[10];
	f += mat[4]*mat[9]*mat[2];
	f += mat[8]*mat[1]*mat[6];

	f -= mat[8]*mat[5]*mat[2];
	f -= mat[4]*mat[1]*mat[10];
	f -= mat[0]*mat[9]*mat[6];

	return f;
}

void MKZ_ARITHMETIC_normalize(float * v3){

	float dist = MKZ_ARITHMETIC_eulidean_norm(v3);

	v3[0] = v3[0]/dist;
	v3[1] = v3[1]/dist;
	v3[2] = v3[2]/dist;
}

void MKZ_ARITHMETIC_normalize_vector(MKZ_vector3 * v3){

	float  dist = MKZ_ARITHMETIC_eulidean_norm_vector(v3);

	v3->x = v3->x/dist;
	v3->y = v3->y/dist;
	v3->z = v3->z/dist;


}

void MKZ_ARITHMETIC_corssProduct_vector(MKZ_vector3* v1, MKZ_vector3* v2 , MKZ_vector3* v3){

	v3->x = v1->y*v2->z - v1->z*v2->y;
	v3->y = v1->z*v2->x - v1->x*v2->z;
	v3->z = v1->x*v2->y - v1->y*v2->x;

}

float MKZ_ARITHMETIC_dotProduct_vector(MKZ_vector3 * v1, MKZ_vector3 * v2){

	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

float MKZ_ARITHMETIC_dotProduct(float * v1, float * v2){

	int i;
	float psum = 0;

	for (i = 0 ; i < 3 ; i++){
		psum += v1[i]*v2[i];
	}

	return psum;
}

float MKZ_ARITHMETIC_eulidean_norm_vector(MKZ_vector3 * v3){

	return sqrt(v3->x*v3->x + v3->y*v3->y + v3->z*v3->z);
}

float MKZ_ARITHMETIC_eulidean_norm(float * v3){
	return sqrt(v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2]);
}

void MKZ_ARITHMETIC_transform_vector(float * mat, MKZ_vector3 * v3){

	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = mat[0]*v3->x + mat[4]*v3->y + mat[8]*v3->z 	+ mat[12];
	vy = mat[1]*v3->x + mat[5]*v3->y + mat[9]*v3->z 	+ mat[13];
	vz = mat[2]*v3->x + mat[6]*v3->y + mat[10]*v3->z	+ mat[14];

	v3->x = vx;
	v3->y = vy;
	v3->z = vz;

}

void MKZ_ARITHMETIC_transform(float * mat, float * v3){
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = mat[0]*v3[0] 	+ mat[4]*v3[1]  + mat[8]*v3[2] 	+ mat[12];
	vy = mat[1]*v3[0]  	+ mat[5]*v3[1]  + mat[9]*v3[2] 	+ mat[13];
	vz = mat[2]*v3[0]  	+ mat[6]*v3[1]  + mat[10]*v3[2] + mat[14];

	v3[0] = vx;
	v3[1] = vy;
	v3[2] = vz;


}
void MKZ_ARITHMETIC_copy_matrix(float * vin , float * vout){

	int i;
	for (i = 0; i < 16; i++) {
		vout[i] = vin[i];
	}
}

void MKZ_ARITHMETIC_matMul(float * mat1,float * mat2,float * mat3){

	MKZ_ARITHMETIC_nulMatrix(mat3);

	int i,j,k;
	for (i = 0 ; i < 4 ; i++){
		for (j = 0 ; j < 4 ; j++){

			for (k = 0 ; k < 4 ; k++){

				mat3[i + j*4] += mat1[i + k*4]*mat2[j*4 + k];
			}
		}
	}
}

/** Matrices **/
void MKZ_ARITHMETIC_matrix_translate(MKZ_vector3 * v3,float* mat){

	MKZ_ARITHMETIC_identityMatrix(mat);
	mat[12] = v3->x;
	mat[13] = v3->y;
	mat[14] = v3->z;

}

void MKZ_ARITHMETIC_matrix_rotate(MKZ_line * axis, float rads,float* mat){


	float * temp = MKZ_ARITHMETIC_create_matrix();
	float * temp2 = MKZ_ARITHMETIC_create_matrix();

	MKZ_vector3 * v3 = MKZ_GEOMETRY_create_vector3();

	v3->x =  - axis->p0.x;
	v3->y =  - axis->p0.y;
	v3->z =  - axis->p0.z;

	MKZ_ARITHMETIC_matrix_translate(v3, temp2);

	float coseno = cos(rads);
	float comp_cos = 1-coseno;
	float seno = sin(rads);

	float x = axis->p1.x - axis->p0.x;
	float y = axis->p1.y - axis->p0.y;
	float z = axis->p1.z - axis->p0.z;

	temp[0] = coseno + (comp_cos)*x*x;
	temp[1] = (comp_cos)*x*y + z*seno;
	temp[2] = (comp_cos)*x*z - y*seno;
	temp[4] = (comp_cos)*x*y - z*seno;
	temp[5] = coseno + (comp_cos)*y*y;
	temp[6] = (comp_cos)*y*z - x*seno;
	temp[8] = comp_cos*x*z + y*seno;
	temp[9] = comp_cos*y*z - x*seno;
	temp[10] = coseno + (comp_cos)*z*z;
	temp[15] = 1;


	MKZ_ARITHMETIC_matMul(temp, temp2, mat);

	mat[12] -= v3->x;
	mat[13] -= v3->y;
	mat[14] -= v3->z;

	MKZ_GEOMETRY_free_vector3(v3);
	MKZ_ARITHMETIC_free_matrix(temp);
	MKZ_ARITHMETIC_free_matrix(temp2);
}

void MKZ_ARITHMETIC_matrix_rotateX(float rads,float* mat){

	float seno = sin(rads);
	float coseno = cos(rads);
	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[5]=coseno;
	mat[6]=seno;
	mat[9]=-seno;
	mat[10]=coseno;
}

void MKZ_ARITHMETIC_matrix_rotateY(float rads,float* mat){

	float seno = sin(rads);
	float coseno = cos(rads);
	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[0]=coseno;
	mat[2]=-seno;
	mat[8]=seno;
	mat[10]=coseno;
}

void MKZ_ARITHMETIC_matrix_rotateZ(float rads,float* mat){

	float seno = sin(rads);
	float coseno = cos(rads);
	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[0]=coseno;
	mat[1]=seno;
	mat[4]=-seno;
	mat[5]=coseno;
}

void MKZ_ARITHMETIC_matrix_scale(MKZ_vector3 * along, float scalar,float* mat){

	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[0]*=along->x*scalar;
	mat[5]*=along->y*scalar;
	mat[10]*=along->z*scalar;

}
void MKZ_ARITHMETIC_matrix_scaleX(float scalar,float* mat){

	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[0]*=scalar;

}
void MKZ_ARITHMETIC_matrix_scaleY(float scalar,float* mat){

	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[5]*=scalar;
}

void MKZ_ARITHMETIC_matrix_scaleZ(float scalar,float* mat){
	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[10]*=scalar;
}

void MKZ_ARITHMETIC_matrix_scaleUniform(float scalar,float* mat){

	MKZ_ARITHMETIC_identityMatrix(mat);

	mat[0]*=scalar;
	mat[5]*=scalar;
	mat[10]*=scalar;
}

void MKZ_ARITHMETIC_print_matrix(float * mat){

	int i,j;

	for (i = 0  ; i < 4 ; i++){

			printf("%f %f %f %f\n",mat[i],mat[i+4],mat[i+8],mat[i+12]);

	}

	printf("\n");
}

