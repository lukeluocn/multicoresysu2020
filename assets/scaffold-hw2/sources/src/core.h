#ifndef _INCL_CORE
#define _INCL_CORE

#include <stdio.h>

#define CALLBACK1 cudaCallbackCPU
#define CALLBACK2 cudaCallbackGPU
// #define CALLBACK3 ...
// #define CALLBACK4 ...
// #define CALLBACK5 ...
// #define CALLBACK6 ...
// #define CALLBACK7 ...
// #define CALLBACK8 ...
// #define CALLBACK9 ...
// #define CALLBACK10 ...

// The main function would invoke the "cudaCallback"s on each sample. Note that
// you don't have to (and shouldn't) free the space of searchPoints,
// referencePoints, and result by yourself since the main function have included
// the free statements already.
//
// To make the program work, you shouldn't modify the signature of\
// "cudaCallback"s.
extern void cudaCallbackCPU(int k, int m, int n, float *searchPoints,
                            float *referencePoints, int **results);
extern void cudaCallbackGPU(int k, int m, int n, float *searchPoints,
                            float *referencePoints, int **results);

// divup calculates n / m and would round it up if the remainder is non-zero.
extern int divup(int n, int m);

// CHECK macro from Grossman and McKercher, "Professional CUDA C Programming"
#define CHECK(call)                                         \
{                                                           \
    const cudaError_t error = call;                         \
    if (error != cudaSuccess) {                             \
        printf("Error: %s:%d, ", __FILE__, __LINE__);       \
        printf("code:%d, reason: %s \n",                    \
                error, cudaGetErrorString(error));          \
        exit(1);                                            \
    }                                                       \
}                                                           \

#endif