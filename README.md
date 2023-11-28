# 2d-FFT-for-Convolution
This is an implementation of 2-D convolution using FFT. 
This version uses openMP to parallelize the code and compare the runtime of the program to the original one. 
## Runtime Results
The size of the input matrix is $256*256$.
We change the size of the convolution kernel and record the average runtime, and compare it with that of naive figure convolution.

| Size of Kernel | Original Convolution Runtime (s) | Parallel Convolution Runtime (s) | Original FFT Runtime (s) | Parallel FFT Runtime (s) |
| -------- | -------- | -------- | -------- | -------- |
| 3 | 0.0257 | 0.0208 | 1.4193 | 0.7016 |
| 15 | 0.2384 | 0.1260 | 1.4136 | 0.7033 |
| 60 | 3.2623 | 1.4955 | 1.4084 | 0.7177 |
| 120 | 12.6859 | 5.3977 | 1.4064 | 0.7121 |
| 256 | 52.7343 | 20.7354 | 1.4085 | 0.6968 |


Results are shown below.

![convolution_time](https://github.com/Julien-Sun/2d-FFT-for-Convolution-/assets/67185297/0f32cdc2-91d8-4ebc-b8fc-0ec7d0513a8f)

![FFT_time](https://github.com/Julien-Sun/2d-FFT-for-Convolution-/assets/67185297/b8178da6-11f1-42e9-9a28-d87c77ad2c09)

## FLOP Results

We also measured the FLOPs of different methods. The results showed that when the kernel size is small, FFT does not accelerate the program and instead increases the runtime. However, when the kernel size is large, FFT shows a significant acceleration effect. The reason for the acceleration provided by FFT is its ability to greatly reduce the FLOPs of the code.

| Size of Kernel | Naive Convolution | FFT Convolution |
| -------- | -------- | -------- |
| 3 | 1956876 | 176422912 |
| 15 | 43152576 | 176422912 |
| 60 | 627471408 | 176422912 |
| 120 | 2206679808 | 176422912 |
| 256 | 7247953920 | 176422912 |
