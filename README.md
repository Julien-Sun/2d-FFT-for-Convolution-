# 2d-FFT-for-Convolution
This is an implementation of 2-D convolution using FFT. 
This version uses openMP to parallelize the code and compare the runtime of the program to the original one. 
## Runtime Results
The size of the input matrix is $256*256$.
We change the size of the convolution kernel and record the average runtime, and compare it with that of trivial figure convolution.
| Size of Kernel | Original Convolution Runtime (s) | Parallel Convolution Runtime (s) | Original FFT Runtime (s) | Parallel FFT Runtime (s) |
| -------- | -------- | -------- | -------- | -------- |
| 3 | 0.0257 | 0.0428 | 1.4193 | 0.7016 |
| 15 | 0.2384 | 0.1523 | 1.4136 | 0.7033 |
| 60 | 3.2623 | 1.8867 | 1.4084 | 0.7177 |
| 120 | 12.6859 | 6.7755 | 1.4064 | 0.7121 |
| 256 | 52.7343 | 26.5379 | 1.4085 | 0.6968 |

