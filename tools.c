#include "tools.h"

complex* x;		
int N;			
int bits;		

void fft_init(int fft_point, int xn_length, complex* xn) {
	int i = 0;
	N = fft_point;
	bits = (int)log2(N);
	x = (complex*)malloc(N * sizeof(complex));
	for (i = 0;i < N;i++) {
		if (i <= xn_length) {
			x[i].real = xn[i].real;
			x[i].imaginary = xn[i].imaginary;
		} else {
			x[i].real = 0;
			x[i].imaginary = 0;
		}
	}	
}

void ifft_init(int ifft_point, complex* xk) {
	int i;
	N = ifft_point;
	bits = (int)log2(N);
	x = (complex*)malloc(N * sizeof(complex));
	
	for (i = 0;i < N;i++) {
		x[i].real = xk[i].real;
		x[i].imaginary = -xk[i].imaginary;
	}
}

complex add(complex a, complex b) {
	complex result;
	result.real = a.real + b.real;
	result.imaginary = a.imaginary + b.imaginary;
	return result;
}

complex sub(complex a, complex b) {
	complex result;
	result.real = a.real - b.real;
	result.imaginary = a.imaginary - b.imaginary;
	return result;
}

complex multiplication(complex a, complex b) {
	complex result;
	result.real = a.real*b.real - a.imaginary*b.imaginary;
	result.imaginary = a.real*b.imaginary + a.imaginary*b.real;
	return result;
}

double model(complex a) {
	double result;
	double real2 = pow(a.real, 2);
	double imaginary2 = pow(a.imaginary, 2);
	result = pow(real2 + imaginary2, 0.5);
	return result;
}

complex conjugate(complex a) {			
	complex result;
	result.real = a.real;
	result.imaginary = -a.imaginary;
	return result;
}

void reverse() { 
	complex temp;
	char* flag = (char*)malloc(N * sizeof(char));
	int i, j, current_n, target_n;
	for (i = 0;i < N;i++) {
		flag[i] = 0;
	}
	for (i = 0;i < N;i++) {
		current_n = i;
		target_n = 0;
		for (j = 0;j < bits;j++) {
			target_n = target_n + (int)((current_n % 2) * pow(2, bits - j - 1));
			current_n /= 2;
		}
		current_n = i;
		if (current_n != target_n && flag[current_n] != 1) {
			temp = x[current_n];
			x[current_n] = x[target_n];
			x[target_n] = temp;
			flag[target_n] = 1;
		}
	}
	free(flag);
}

complex w_builder(int m,int k) {
	complex W;
	double base = pow(2, m);
	W.real = cos(2 * pi*k / base);
	W.imaginary = -sin(2 * pi*k / base);
	return W;
}

void butterfly(int x1_point, int x2_point, complex wn) {
	complex result1, result2, T;
	T = multiplication(x[x2_point], wn);
	result1 = add(x[x1_point], T);
	result2 = sub(x[x1_point], T);

	x[x1_point] = result1;
	x[x2_point] = result2;
}

void single_fft(int m) {
	int point_distance = (int)pow(2, m - 1);
	int group_distance = 2 * point_distance;
	int group_count = N / group_distance;

	int group_header = 0;

	int x1, x2;
	complex w;

	int i, j;
	for (i = 0;i < group_count;i++) {
		group_header = i * group_distance;
		for (j = 0;j < group_distance / 2;j++) {
			w = w_builder(m, j);
			x1 = j + group_header;
			x2 = x1 + point_distance;
			butterfly(x1, x2, w);
		}
	}
}

complex *fft(int fft_point, int xn_length, complex* xn) {
	int i;
	fft_init(fft_point, xn_length, xn);
	reverse();
	for (i = 1;i <= bits;i++) {
		single_fft(i);
	}
	return x;
}

complex *ifft(int xk_length, complex* xk) {
	int i;
	ifft_init(xk_length, xk);
	reverse();
	for (i = 1;i <= bits;i++) {
		single_fft(i);
	}
	for (i = 0;i < N;i++) {
		x[i] = conjugate(x[i]);
		x[i].real /= N;
		x[i].imaginary /= N;
	}
	return x;
}
