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
    double **mat = (double **)malloc(sizeof(double *) * row);
    for (int i = 0; i < row; i++) {
        mat[i] = (double *)malloc(sizeof(double) * col);
        for (int j = 0; j < col; j++) {
            mat[i][j] = i + j;
        }
    }
    return mat;
}

double **convolve(int row_input, int col_input, int row_kernel, int col_kernel, double **input, double **kernel) {
    double **output = (double **)malloc(sizeof(double *) * row_input);
    for (int i = 0; i < row_input; i++) {
        output[i] = (double *)malloc(sizeof(double) * col_input);
    }

//    #pragma omp parallel for collapse(2) num_threads(4)
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < row_input; i++) {
        for (int j = 0; j < col_input; j++) {
            double sum = 0;
            for (int k = 0; k < row_kernel; k++) {
                for (int l = 0; l < col_kernel; l++) {
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
            printf("%.4f ", input[i][j]);
        }
        printf("\n");
    }
}

int main() {

//    omp_set_num_threads(64);
    
    // initialize image
    int row_x = M;
    int col_x = N;
    int channel = 3;

    double ***xn = (double ***)malloc(sizeof(double **) * channel);
    for (int i = 0; i < channel; i++) {
        xn[i] = mat_initialize(row_x, col_x);
    }

    // initialize convolution kernel
    int row_k = X;
    int col_k = Y;
    double **kn = mat_initialize(row_k, col_k);

    // initialize result matrix
    double **res_total = (double **)malloc(sizeof(double *) * row_x);
    for (int i = 0; i < row_x; i++) {
        res_total[i] = (double *)malloc(sizeof(double) * col_x);
    }

    double start_time, end_time;
    start_time = omp_get_wtime();

    #pragma omp parallel for num_threads(channel)
    for (int i = 0; i < channel; i++) {
        double **res = convolve(row_x, col_x, row_k, col_k, xn[i], kn);

        #pragma omp critical
        {
            for (int j = 0; j < row_x; j++) {
                for (int k = 0; k < col_x; k++) {
                    res_total[j][k] += res[j][k];
                }
            }
        }

        // free memory
        for (int j = 0; j < row_x; j++) {
            free(res[j]);
        }
        free(res);
    }

    end_time = omp_get_wtime();
/*
    // print result
    printf("xn:\n");
    print_result(row_x, col_x, xn[2]);
    printf("kn:\n");
    print_result(row_k, col_k, kn);
    printf("res_total:\n");
    print_result(row_x, col_x, res_total);
*/
    printf("Time Used: %f s\n", end_time - start_time);

    // free memory
    for (int i = 0; i < channel; i++) {
        for (int j = 0; j < row_x; j++) {
            free(xn[i][j]);
        }
        free(xn[i]);
    }
    for (int i = 0; i < row_x; i++) {
        free(res_total[i]);
    }

    free(xn);
    free(kn);
    free(res_total);

    return 0;
}
