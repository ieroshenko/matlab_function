//
//  updateArray.c
//  Ivan's 2nd assignment
//
//  Created by Иван on 6/23/19.
//  Copyright © 2019 Иван. All rights reserved.
//

#include "mex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void updateArray (int M, int N, double *p, int *ineighbor, double *integralp, double *c, double* outMatrix){

    for(int m = 0; m < M; m++){
        
        for(int n = 0; n<N; n++){
            //pointer arithmentic
            int val_p = *(ineighbor + m * N + n) - 1; // -1 since in matlab the count starts from 1
            int val_i = * (integralp + m * N + n);
            
            p[val_p] = p[val_p] - c[m] * val_i;
            
        }
    }

    memcpy(outMatrix, p, sizeof(double)*M);
}

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{

    // input arguments
    int M;
    int N;
    double *p;
    double *ineighbor;
    double *integralp;
    double *c;
    
    // output argument
    double* outMatrix;
    
    //check for proper number of input arguments
    if(nrhs!=6) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","Six inputs required.");
    }
    

    
     //make sure the first input argument is integer
     if( !mxIsScalar(prhs[0]) ||
     mxIsComplex(prhs[0]) ||
     mxGetNumberOfElements(prhs[0])!=1 ) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notScalar","Input M must be an integer. (10^6 - 10^8)");
     }
     
    
    
    /*make sure the second input argument is integer*/
     if( !mxIsScalar(prhs[1]) ||
     mxIsComplex(prhs[0]) ||
     mxGetNumberOfElements(prhs[1])!=1 ) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notScalar","Input N must be an integer. (Max 256)");
     }
    
    
    /*make sure the third input argument is type double */
     if( !mxIsDouble(prhs[2])) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble","Input column vector p must be type double.");
     }
     
    
    /*make sure the fourth input argument is type integer*/
     if( !mxIsDouble(prhs[3]) ||
     mxIsComplex(prhs[3])) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble","Input matrix ineighbor must be type integer.");
     }
     
    
    //make sure the fifth input argument is type double
     if( !mxIsDouble(prhs[4]) ||
     mxIsComplex(prhs[4])) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble","Input column vector integralp must be type double.");
     }
    
    
    /*make sure the sixth input argument is type double*/
     if( !mxIsDouble(prhs[5])) {
     mexErrMsgIdAndTxt("MyToolbox:arrayProduct:notDouble","Input column vector c must be type double.");
     }
     
            

    //get the value of the integer M input
    M = mxGetScalar(prhs[0]);

    
    //get the value of the integer N input
    N = mxGetScalar(prhs[1]);

    
    //create a pointer to the real data in the input matrix p
    #if MX_HAS_INTERLEAVED_COMPLEX
    p = mxGetDoubles(prhs[2]);
    #else
    p = mxGetPr(prhs[2]);
    #endif

    
    //create an array ineighbor of size M * N of type int32
    int *local_ineighbor = (int *) malloc(M * N * sizeof(int));
    
    ineighbor = mxGetPr((prhs[3]));
        
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            *(local_ineighbor + i * N + j) = ineighbor[N * j + i];
        }
    }
    ineighbor = NULL;
    
    //create an array integralp of size M * N of type Double
    double *local_integralp = (double *) malloc(M * N * sizeof(double));
    
    integralp = mxGetPr((prhs[4]));
   
    for(int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            *(local_integralp + i * N + j) = integralp[N * i + j];
        }
    }
    integralp = NULL;
    
    //create a pointer to the real data in the input matrix integralp
    #if MX_HAS_INTERLEAVED_COMPLEX
    c = mxGetDoubles(prhs[5]);
    #else
    c = mxGetPr(prhs[5]);
    #endif

    
    //create the output matrix
    plhs[0] = mxCreateDoubleMatrix(M,1,mxREAL);

    
    //get a pointer to the real data in the output matrix
    #if MX_HAS_INTERLEAVED_COMPLEX
    outMatrix = mxGetDoubles(plhs[0]);
    #else
    outMatrix = mxGetPr(plhs[0]);
    #endif

    
    updateArray(M, N, p, local_ineighbor, local_integralp, c, outMatrix);

    
    return;
}



