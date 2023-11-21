#pragma once
#include <math.h>
#include <malloc.h>
#include <omp.h>

#define pi 3.14159265358979

typedef struct {
	double real;
	double imaginary;
} complex;

void fft_init(int fft_point, int xn_length, complex* xn);
void ifft_init(int ifft_point, complex* xk);
complex add(complex a, complex b);
complex sub(complex a, complex b);
complex multiplication(complex a, complex b);
double model(complex a);
complex conjugate(complex a);
void reverse();
complex w_builder(int m, int k);
void butterfly(int x1_point, int x2_point, complex wn);
void single_fft(int m);
complex* fft(int fft_point, int xn_length, complex* xn);
complex* ifft(int xk_length, complex* xk);