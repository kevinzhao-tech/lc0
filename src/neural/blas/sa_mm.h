#include "cnn.h"

#ifndef SA_MM_H
#define SA_MM_H

#ifdef __cplusplus
extern "C" {
#endif

cnn_state *state;

void matrix_multiply_4(const float *A, const float *B, float *C, const int lda, const int ldb, const int ldc, const int M, const int N, const int K, const int i, const int j, const int k);

#ifdef __cplusplus
}
#endif

#endif