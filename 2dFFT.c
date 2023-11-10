#include <stdio.h>
#include "tools.h"

// size of image
#define M 8
#define N 8
// size of convolution kernel
#define X 3
#define Y 3

complex **mat_initialize(int row, int col) {
    complex **mat = NULL;
    mat = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        mat[i] = (complex *)malloc(sizeof(complex) * col);
        for (int j = 0; j < col; j++) {
		    mat[i][j].imaginary = 0;
		    mat[i][j].real = i+j;
        }
    }
    return mat;
}

complex **mat_pad(int row_ori, int col_ori, int row, int col, complex **input) {
    complex **mat = NULL;
    mat = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        mat[i] = (complex *)malloc(sizeof(complex) * col);
        for (int j = 0; j < col; j++) {
		    mat[i][j].imaginary = 0;
		    mat[i][j].real = 0;
        }
    }
    for (int i = 0; i < row_ori; i++) {
        for (int j = 0; j < col_ori; j++) {
		    mat[i][j] = input[i][j];
        }
    }
    return mat;
}

complex **mat_multi(int row, int col, complex **input1, complex **input2) {
    complex **multi_result = NULL;
    multi_result = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        multi_result[i] = (complex *)malloc(sizeof(complex) * col);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            multi_result[i][j] = multiplication(input1[i][j], input2[i][j]);
        }
    }

    return multi_result;
}

void print_result(int row, int col, complex **input) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
		    printf("%f ", model(input[i][j]));
        }
        printf("\n");
	}
}

complex **fft_2d(int row, int col, int fft_point, complex **input) {
    complex* xk1[row];
    complex* xk2[col];
    // FFT
	for (int i = 0; i < row; i++) {
        xk1[i] = fft(fft_point, col, &input[i][0]);
    }

    complex x_temp[col][row];
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            x_temp[i][j] = xk1[j][i];
        }
    }

    for (int i = 0; i < col; i++) {
        xk2[i] = fft(fft_point, row, &x_temp[i][0]);
    }

    complex **xk = NULL;
    xk = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        xk[i] = (complex *)malloc(sizeof(complex) * col);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            xk[i][j] = xk2[j][i];
        }
    }

    return xk;
}

complex **ifft_2d(int row, int col, int fft_point, complex **input) {
    complex* xr1[row];
    complex* xr2[col];
    // IFFT
    for (int i = 0; i < row; i++) {
        xr1[i] = ifft(fft_point, &input[i][0]);
    }

    complex x_r_temp[col][row];

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            x_r_temp[i][j] = xr1[j][i];
        }
    }

    for (int i = 0; i < col; i++) {
        xr2[i] = ifft(fft_point, &x_r_temp[i][0]);
    }

    complex **xr = NULL;
    xr = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        xr[i] = (complex *)malloc(sizeof(complex) * col);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            xr[i][j] = xr2[j][i];
        }
    }

    return xr;
}

int main() {
    // initialize image
    int row_x = M;
    int col_x = N;
    int channel = 3;
    // complex **xn = NULL;
    // xn = mat_initialize(row_x, col_x);
    complex **xn[channel];
    for (int i=0; i<3; i++) {
        xn[i] = mat_initialize(row_x, col_x);
    }

    // initialize convolution kernel
    int row_k = X;
    int col_k = Y;

    complex **kn = NULL;
    kn = mat_initialize(row_k, col_k);

    int fft_point = M;
    int row = M;
    int col = N;

    complex **kn_pad = NULL;
    kn_pad = mat_pad(row_k, col_k, row, col, kn);
  
    // FFT
    // complex **xk = NULL;
    // xk = fft_2d(row, col, fft_point, xn);
    complex **xk[channel];
    for (int i=0; i<3; i++) {
        xk[i] = fft_2d(row, col, fft_point, xn[i]);
    }

    complex **kk = NULL;
    kk = fft_2d(row, col, fft_point, kn_pad);

    // Multiplication
    // complex **res_mid = NULL;
    // res_mid = mat_multi(row, col, xk, kk);
    complex **res_mid[channel];
    for (int i=0; i<3; i++) {
        res_mid[i] = mat_multi(row, col, xk[i], kk);
    }

    // IFFT
    complex **xr[channel];
    for (int i=0; i<3; i++) {
        xr[i] = ifft_2d(row, col, fft_point, xk[i]);
    }

    complex **kr = NULL;
    kr = ifft_2d(row, col, fft_point, kk);

    // complex **res = NULL;
    // res = ifft_2d(row, col, fft_point, res_mid);
    complex **res[channel];
    for (int i=0; i<3; i++) {
        res[i] = ifft_2d(row, col, fft_point, res_mid[i]);
    }

    complex **res_total = NULL;
    res_total = (complex **)malloc(sizeof(complex *) * row);
    for (int i = 0; i < row; i++) {
        res_total[i] = (complex *)malloc(sizeof(complex) * col);
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            res_total[i][j].imaginary = 0;
		    res_total[i][j].real = 0;
            for (int k = 0; k < channel; k++){
                res_total[i][j] = add(res[k][i][j], res_total[i][j]);
            }
        }
    }

    // print result
    printf("xn:\n");
    print_result(row, col, xn[2]);
    printf("kn_pad:\n");
    print_result(row, col, kn_pad);
    printf("xr:\n");
    print_result(row, col, xr[2]);
    printf("kr:\n");
    print_result(row, col, kr);
    printf("res_total:\n");
    print_result(row, col, res_total);

    for (int i = 0; i < channel; i++){
        free(xn[i]);
        free(xk[i]);
        free(xr[i]);
        free(res_mid[i]);
        free(res[i]);
    }

    free(kn);
    free(kn_pad);
    free(kk);
    free(kr);
    free(res_total);

}