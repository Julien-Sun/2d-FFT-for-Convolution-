# 2d-FFT-for-Convolution
This is an implementation of 2-D convolution using FFT. 
This version uses openMP to parallelize the code and compare the runtime of the program to the original one. 
## Runtime Results
The size of the input matrix is $256*256$.
We change the size of the convolution kernel and record the runtime.
| Size of Kernel     | Original Runtime (s)     | Parallel Runtime (s)     |
| -------- | -------- | -------- |
| 3 | 1.4193 | 0.9090 |
| 15 | 1.4136 | 0.8929 |
| 60 | 1.4084 | 0.8927 |
| 120 | 1.4064 | 0.8656 |
| 256 | 1.4085 | 0.8558 |

