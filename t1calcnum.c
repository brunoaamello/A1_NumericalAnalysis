/* Trabalho 1 - Calculo Numerico
 * 2o semestre de 2018
 * USP Sao Carlos
 * Bruno Arantes de Achilles Mello - 9866490
 */

#include <stdio.h>
#include <stdlib.h>

typedef double    dataType;
typedef dataType* vector;
typedef vector*   matrix;

#define MAX_INT 0x7F

#define SIG_DIGITS 7

// Modulo de um valor qualquer
dataType mod(dataType value);

// Norma infinita de um vetor
dataType infiniteNorm(vector V, int size);

// Operacao de ssubtracao de vetores
int subVector(vector R, vector A, vector B, int size);

// Operacao de multiplicacao de uma matrix por um vetor
int multMatVec(vector R, matrix A, vector V, int size);

// Inicializa um vetor
int initVector(vector* V, int size);

// Inicializa uma matriz nxn
int initMatrix(matrix* A, int size);

// Atribui valores para a matriz de acordo com o enunciado do problema
int fillMatrixAsSpecified(matrix A, int size);

// Atribui valores para o vetor de acordo com o enunciado do problema B
int fillVectorAsSpecifiedA(vector V, matrix A, int size);

// Atribui valores para o vetor de acordo com o enunciado do problema c
int fillVectorAsSpecifiedB(vector V, int size);

// Checa a convergencia do metodo
int checkConvergence(matrix A, int size);

// Calcula os coeficientes para iteracao com gauss-seidel
int calculateCoeficients(matrix A, vector B, matrix coeficients, int size);

// Efetua uma iteracao do metodo de gauss-seidel
int iterateGaussSeidel(vector oldX, vector newX, matrix coeficients, int size);

// Imprime a operacao feita
int printOperation(matrix A, vector X, vector B, int size);

int main(){
	matrix A, coeficients;
	vector B, X[2], error, residue;
	dataType errorMargin, tempError;
	int i;
	int xOld;
	int xNew;
	int size, converges, maxIterations;
	int eF = 0; //Error Flag
	
	int ex;
	
for(ex=0;ex<3;ex++){
	
	if(ex==0){
		size = 100;
		maxIterations = size*size;
		errorMargin = 0.0000000001;
		printf("Teste b1: n=100, e=0.0000000001, itmax=10000.\n\n");
	}else if(ex==1){
		size = 200;
		maxIterations = size*size;
		errorMargin = 0.0000000001;
		printf("Teste b2: n=200, e=0.0000000001, itmax=40000.\n\n");
	}else if(ex==2){
		size = 500;
		maxIterations = 10*size;
		errorMargin = 0.0000000001;
		printf("Teste c: n=500, e=0.0000000001, itmax=5000.\n\n");
	}
	
	eF |= initMatrix(&A, size);
	eF |= initMatrix(&coeficients, size+1);
	eF |= initVector(&B, size);
	eF |= initVector(&(X[0]), size);
	eF |= initVector(&(X[1]), size);
	eF |= initVector(&error, size);
	eF |= initVector(&residue, size);
	
	if(eF){
		printf("Error initializing variables.\nExiting...\n");
		exit(1);
	}

	eF |= fillMatrixAsSpecified(A, size);
	
	if(eF){
		printf("Error filling matrix A.\nExiting...\n");
		exit(1);
	}
	
	converges = checkConvergence(A, size);
	if(!converges){
		printf("The matrix given does not converge.\nExiting...\n");
		exit(1);
	}
	if(ex==0 || ex==1){
		eF |= fillVectorAsSpecifiedA(B, A, size);
	}else if(ex==2){
		eF |= fillVectorAsSpecifiedB(B, size);
	}
	
	if(eF){
		printf("Error filling vector B.\nExiting...\n");
		exit(1);
	}
	
	eF |= calculateCoeficients(A, B, coeficients, size);
	
	if(eF){
		printf("Error calculating coeficients.\nExiting...\n");
		exit(1);
	}
	xOld=0;
	xNew=1;
	for(i=0;i<maxIterations && !eF;i++){
		if(xOld==0){
			xOld=1;
			xNew=0;
		}else{
			xOld=0;
			xNew=1;
		}
		eF |= iterateGaussSeidel(X[xOld], X[xNew], coeficients, size);
		eF |= subVector(error, X[xNew], X[xOld], size);
		tempError = infiniteNorm(error, size);
		eF |= (tempError == -1.0);
		if(tempError<=errorMargin){
			break;
		}
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
	
	if(ex==2){
		printf("Calculo do erro obtido na solucao:\n\n");
		multMatVec(error, A, X[xNew], size);
		subVector(residue, B, error, size);
		tempError = infiniteNorm(residue, size);
		printf("Erro da solucao = %+.*e \n", SIG_DIGITS, tempError);
	}
		
	
	for(i=0;i<size;i++){
		free(A[i]);
		free(coeficients[i]);
	}
	free(coeficients[size]);
	free(A);
	free(coeficients);
	free(error);
	free(residue);
	free(X[0]);
	free(X[1]);
	free(B);
	printf("\n#######################################################################\n\n");
}
	return 0;
}

dataType mod(dataType value){
	if(value<0.0){
		return -value;
	}else{
		return value;
	}
}

dataType infiniteNorm(vector V, int size){
	if(!V && size<=0){
		return -1.0;
	}else{
		int i;
		dataType max = 0;
		dataType acess;
		for(i=0;i<size;i++){
			acess=mod(V[i]);
			if(acess>max){
				max=acess;
			}
		}
		return max;
	}
}

int subVector(vector R, vector A, vector B, int size){
	if(!R || !A || !B || size<=0){
		return 1;
	}else{
		int i;
		for(i=0;i<size;i++){
			R[i]=A[i]-B[i];
		}
		return 0;
	}
}

int multMatVec(vector R, matrix A, vector V, int size){
	if(!R || !A || !V || size<=0){
		return 1;
	}else{
		int i, j;
		for(i=0;i<size;i++){
			R[i]=0;
			for(j=0;j<size;j++){
				R[i]+=A[i][j]*V[j];
			}
		}
		return 0;
	}
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

int fillMatrixAsSpecified(matrix A, int size){
	if(A && size>0){
		int i, j;
		for(i=0;i<size;i++){
			for(j=0;j<size;j++){
				if(j!=i && j+1!=1 && j+3!=i && j-1!=i && j-3!=i){
					A[i][j]=0;
				}
			}
			A[i][i]=4.5;
			if(i+1<size){
				A[i][i+1]=-1;
				if(i+3<size){
					A[i][i+3]=-1;
				}
			}
			if(i-1>=0){
				A[i][i-1]=-1;
				if(i-3>=0){
					A[i][i-3]=-1;
				}
			}
		}
		return 0;
	}else{
		return 1;
	}
}

int fillVectorAsSpecifiedA(vector V, matrix A, int size){
	if(V && size>0){
		int i;
		for(i=0;i<size;i++){
			V[i]=A[i][i];
			if(i+1<size){
				V[i]+=A[i][i+1];
				if(i+3<size){
					V[i]+=A[i][i+3];
				}
			}
			if(i-1>=0){
				V[i]+=A[i-1][i];
				if(i-3>=0){
					V[i]+=A[i-3][i];
				}
			}
		}
		return 0;
	}else{
		return 1;
	}
}

int fillVectorAsSpecifiedB(vector V, int size){
	if(V && size>0){
		int i;
		for(i=0;i<size;i++){
			V[i]=1.0/((dataType) i+1);
		}
		return 0;
	}else{
		return 1;
	}
}

// checa convergencia garantindo que a matriz é diagonal dominante tanto por linhas quanto por colunas
int checkConvergence(matrix A, int size){
	if(!A || size<=0){
		return 0;
	}
	int i, j;
	dataType countLine;
	dataType countColumn;
	for(i=0;i<size;i++){
		countLine=0;
		countColumn=0;
		for(j=0;j<size;j++){
			if(i!=j){
				countLine+=A[i][j];
				countColumn+=A[j][i];
			}
		}
		if(countLine<0){
			countLine=-countLine;
		}
		if(countColumn<0){
			countColumn=-countColumn;
		}
		if(countLine>A[i][i] || countColumn>A[i][i]){
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
			printf("%+.*e ", SIG_DIGITS, A[i][j]);
		}
		if(i==size/2){
			printf("| . ");
		}else{
			printf("|   ");
		}
		printf("| %+.*e | ", SIG_DIGITS, X[i]);
		if(i==size/2){
			printf("= ");
		}else{
			printf("  ");
		}
		printf("| %+.*e |\n", SIG_DIGITS, B[i]);
	}
	return 0;
}
