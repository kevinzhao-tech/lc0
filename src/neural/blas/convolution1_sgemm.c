#include <string.h>
#include "convolution1_sgemm.h"

void matrix_multiply_4(const float *A, const float *B, float *C, const int lda, const int ldb, const int ldc, const int M, const int N, const int K, const int i, const int j, const int k) {
    int n = 4;

    //printf("mmul4 is being called.\n");


    for (int i0 = 0; i0 < n && i + i0 < M; ++i0) {
        for (int j0 = 0; j0 < n && j + j0 < N; ++j0) {
            float sum = 0.0f;
            for (int k0 = 0; k0 < n && k + k0 < K; ++k0) {
                sum += A[(i + i0) * lda + k + k0] * B[(k + k0) * ldb + j + j0];
            }
            C[(i + i0) * ldc + j + j0] += sum;
        }
    }
}

void new_sgemm(const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc) {

    //printf("new_sgemm() is being called.\n");

    for (int i = 0; i < M; i += 4) {
        for (int j = 0; j < N; j += 4) {
            for (int k = 0; k < K; k += 4) {
                matrix_multiply_4(A, B, C, lda, ldb, ldc, M, N, K, i, j, k);
            }
        }
    }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i * ldc + j] = alpha * C[i * ldc + j] + beta * C[i * ldc + j];
        }
    }
}
