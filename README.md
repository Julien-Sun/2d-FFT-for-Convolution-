# 2d-FFT-for-Convolution
This is an implementation of 2-D convolution using FFT. 
This version uses openMP to parallelize the code and compare the runtime of the programe to the original one. 
## Runtime Results
The size of the input matrix is $256*256$.
We change the size of the convolution kernel and record the runtime.
| Size of Kernel     | Runtime     |
| -------- | -------- |
| 3 | 行1，列2 |
| 30 | 行2，列2 |
| 60 | 行3，列2 |
| 120 | 行3，列2 |
| 256 | 行3，列2 |

