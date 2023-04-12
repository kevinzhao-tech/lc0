#ifndef SGEMM_H
#define SGEMM_H

#ifdef __cplusplus
extern "C" {
#endif

void matrix_multiply_4(const float *A, const float *B, float *C, const int lda, const int ldb, const int ldc, const int M, const int N, const int K, const int i, const int j, const int k);
void new_sgemm(const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc);

#ifdef __cplusplus
}
#endif

#endif
