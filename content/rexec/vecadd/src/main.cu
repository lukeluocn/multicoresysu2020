#include <stdio.h>

__global__ void vec_add(int *a, int *b, int *c) {
    int tid = threadIdx.x;
    c[tid] = a[tid] + b[tid];
}

int main() {
    int n = 8;

    int *a_h, *b_h, *c_h;
    a_h = (int *) malloc(sizeof(int)*n);
    b_h = (int *) malloc(sizeof(int)*n);
    c_h = (int *) malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++) {
        a_h[i] = i;
        b_h[i] = i;
    }

    int *a_d, *b_d, *c_d;
    cudaMalloc((void **)&a_d, sizeof(int)*n);
    cudaMalloc((void **)&b_d, sizeof(int)*n);
    cudaMalloc((void **)&c_d, sizeof(int)*n);

    cudaMemcpy(a_d, a_h, sizeof(int)*n, cudaMemcpyHostToDevice);
    cudaMemcpy(b_d, b_h, sizeof(int)*n, cudaMemcpyHostToDevice);

    vec_add<<<1, n>>>(a_d, b_d, c_d);
    cudaDeviceSynchronize();

    cudaMemcpy(c_h, c_d, sizeof(int)*n, cudaMemcpyDeviceToHost);
    cudaFree(a_d);
    cudaFree(b_d);
    cudaFree(c_d);

    for (int i = 0; i < n; i++) {
        printf("v%d: %d\n", i, c_h[i]);
    }

    return 0;
}