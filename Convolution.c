#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// size of image
#define M 256
#define N 256
// size of convolution kernel
#define X 3
#define Y 3

double **mat_initialize(int row, int col) {
    double **mat = NULL;
    mat = (double **)malloc(sizeof(double *) * row);
    for (int i = 0; i < row; i++) {
        mat[i] = (double *)malloc(sizeof(double) * col);
        for (int j = 0; j < col; j++) {
            mat[i][j] = i+j;
        }
    }
    return mat;
}

double **convolve(int row_input, int col_input, int row_kernel, int col_kernel, double **input, double **kernel) {
    int i, j, k, l;
    double sum;

    double **output = NULL;
    output = (double **)malloc(sizeof(double *) * row_input);
    for (int i = 0; i < row_input; i++) {
        output[i] = (double *)malloc(sizeof(double) * col_input);
    }

    #pragma omp parallel for 
    for (i = 0; i < row_input; i++) {
        for (j = 0; j < col_input; j++) {
            sum = 0;
            for (k = 0; k < row_kernel; k++) {
                for (l = 0; l < col_kernel; l++) {
                    int x = i + k - row_kernel / 2;
                    int y = j + l - col_kernel / 2;
                    
                    // Check if the pixel is within the image boundaries
                    if (x >= 0 && x < row_input && y >= 0 && y < col_input) {
                        sum += input[x][y] * kernel[k][l];
                    }
                }
            }
            output[i][j] = sum;
        }
    }

    return output;
}

void print_result(int row, int col, double **input) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
		    printf("%f ", input[i][j]);
        }
        printf("\n");
	}
}

int main() {
    // initialize image
    int row_x = M;
    int col_x = N;
    int channel = 3;

    double **xn[channel];
    #pragma omp parallel for 
    for (int i=0; i<channel; i++) {
        xn[i] = mat_initialize(row_x, col_x);
    }

    // initialize convolution kernel
    int row_k = X;
    int col_k = Y;

    double **kn = NULL;
    kn = mat_initialize(row_k, col_k);

    int row = M;
    int col = N;

    double start_time, end_time;
    start_time = omp_get_wtime();

    double **res[channel];
    #pragma omp parallel for 
    for (int i=0; i<channel; i++) {
        res[i] = convolve(row_x, col_x, row_k, col_k, xn[i], kn);
    }

    double **res_total = NULL;
    res_total = (double **)malloc(sizeof(double *) * row);
    #pragma omp parallel for 
    for (int i = 0; i < row; i++) {
        res_total[i] = (double *)malloc(sizeof(double) * col);
    }
    #pragma omp parallel for 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            res_total[i][j] = 0;
            for (int k = 0; k < channel; k++){
                res_total[i][j] = res_total[i][j] + res[k][i][j];
            }
        }
    }

    end_time = omp_get_wtime();
/*
    // print result
    printf("xn:\n");
    print_result(row_x, col_x, xn[2]);
    printf("kn:\n");
    print_result(row_k, col_k, kn);
    printf("res_total:\n");
    print_result(row, col, res_total);
 */   
    printf("Time Used: %f s\n", end_time-start_time);

    for (int i = 0; i < channel; i++){
        free(xn[i]);
        free(res[i]);
    }

    free(kn);
    free(res_total);

    return 0;

}