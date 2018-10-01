#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef double    dataType;
typedef dataType* vector;
typedef vector*   matrix;

#define MAX_INT 0x7F

#define SIG_DIGITS 7

int initVector(vector* V, int size);
int initMatrix(matrix* A, int size);
int fillVectorRandomly(vector V, int maxVal, dataType divider, int size);
int fillMatrixRandomly(matrix A, int maxVal, dataType divider, int size);
int checkConvergence(matrix A, int size);
int calculateCoeficients(matrix A, vector B, matrix coeficients, int size);
int iterateGaussSeidel(vector oldX, vector newX, matrix coeficients, int size);
int printOperation(matrix A, vector X, vector B, int size);

int main(){
	matrix A, coeficients;
	vector B, X[2];
	int i;
	int xOld = 0;
	int xNew = 1;
	int size, converges, maxIterations;
	int eF = 0; //Error Flag
	
	size = 3;
	
	eF |= initMatrix(&A, size);
	eF |= initMatrix(&coeficients, size+1);
	eF |= initVector(&B, size);
	eF |= initVector(&(X[0]), size);
	eF |= initVector(&(X[1]), size);
	
	if(eF){
		printf("Error initializing variables.\nExiting...\n");
		exit(1);
	}
	
	srand(time(NULL));
	
	do{
		eF |= fillMatrixRandomly(A, MAX_INT, 384.23, size);
		converges = checkConvergence(A, size);
	}while(!converges && !eF);
	
	eF |= fillVectorRandomly(B, MAX_INT, 384.23, size);
	
	if(eF){
		printf("Error filling matrix A or vector B.\nExiting...\n");
		exit(1);
	}
	
	eF |= calculateCoeficients(A, B, coeficients, size);
	
	if(eF){
		printf("Error calculating coeficients.\nExiting...\n");
		exit(1);
	}
	
	maxIterations=200*size;
	
	for(i=0;i<2*maxIterations && !eF;i++){
		if(xOld==0){
			xOld=1;
			xNew=0;
		}else{
			xOld=0;
			xNew=1;
		}
		eF |= iterateGaussSeidel(X[xOld], X[xNew], coeficients, size);
	}
	
	if(eF){
		printf("Error during Gauss Seidel iteration.\nExiting...\n");
		exit(1);
	}
	
	eF |= printOperation(A, X[xNew], B, size);
	
	if(eF){
		printf("Error printing operation.\nExiting...\n");
		exit(1);
	}
	
	return 0;
}

int initVector(vector* V, int size){
	if(size<=0){
		return 1;
	}else{
		*V = (vector) malloc(sizeof(dataType)*size);
		if(!(*V)){
			return 1;
		}else{
			return 0;
		}
	}	
}

int initMatrix(matrix* A, int size){
	if(size<=0){
		return 1;
	}else{
		*A = (matrix) malloc(sizeof(vector*)*size);
		if(!(*A)){
			return 1;
		}else{
			int i;
			for(i=0;i<size;i++){
				if(initVector(&((*A)[i]), size)){
					return 1;
				}
			}
			return 0;
		}
	}
}

int fillVectorRandomly(vector V, int maxVal, dataType divider, int size){
	if(V && size>0){
		int i;
		for(i=0;i<size;i++){
			V[i]=(i+1)*((dataType)(rand()%maxVal))/divider;
		}
		return 0;
	}else{
		return 1;
	}
}

int fillMatrixRandomly(matrix A, int maxVal, dataType divider, int size){
	if(A && size>0){
		int i;
		for(i=0;i<size;i++){
			if(fillVectorRandomly(A[i], maxVal, divider, size)){
				return 1;
			}
		}
		return 0;
	}else{
		return 1;
	}
}


int checkConvergence(matrix A, int size){
	if(!A || size<=0){
		return 0;
	}
	int i;
	for(i=0;i<size;i++){
		if(A[i][i]==0.0){
			return 0;
		}
	}
	return 1;
}

int calculateCoeficients(matrix A, vector B, matrix coeficients, int size){
	if(size<=0 || !(A && B && coeficients)){
		return 1;
	}
	int i, j;
	for(i=0;i<size;i++){
		if(A[i][i]==0.0){
			return 1;
		}
		for(j=0;j<size;j++){
			if(i==j){
				coeficients[i][j] = 0.0;
			}else{
				coeficients[i][j] = A[i][j]/A[i][i];
			}
		}
		coeficients[i][size] = B[i]/A[i][i];
	}
	return 0;
}

int iterateGaussSeidel(vector oldX, vector newX, matrix coeficients, int size){
	if(size<=0 || !(oldX && newX && coeficients)){
		return 1;
	}
	int i, j;
	for(i=0;i<size;i++){
		newX[i]=coeficients[i][size];
		for(j=0;j<size;j++){
			if(i<j){
				newX[i]-=coeficients[i][j]*newX[j];
			}else if(i>j){
				newX[i]-=coeficients[i][j]*oldX[j];
			}
		}
	}
	return 0;
}

int printOperation(matrix A, vector X, vector B, int size){
	if(size<=0 || !(A && X && B)){
		return 1;
	}
	int i, j;
	for(i=0;i<size;i++){
		printf("| ");
		for(j=0;j<size;j++){
			printf("%.*e ", SIG_DIGITS, A[i][j]);
		}
		if(i==size/2){
			printf("| . ");
		}else{
			printf("|   ");
		}
		printf("| %.*e | ", SIG_DIGITS, X[i]);
		if(i==size/2){
			printf("= ");
		}else{
			printf("  ");
		}
		printf("| %.*e |\n", SIG_DIGITS, B[i]);
	}
	return 0;
}

//printf("%.*e\n", Digs, value);

