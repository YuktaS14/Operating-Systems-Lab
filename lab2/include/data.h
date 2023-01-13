#ifndef DATA_H_
#define DATA_H_

#include<iostream>
#include<vector>
#include<queue>
#include "Logger.h"
using namespace std ;

//extern void initialize_resultMatrix(int r , int c);

class inputData
{ 
    public:
        queue<vector<vector<int>>> MatrixList;
        queue<int> ScalarList;
        bool validInput;
        int scalar;
        int rows1 , rows2 ;
        int columns1 , columns2 ;
        vector<vector<int>> matrix1 ;
        vector<vector<int>> matrix2 ;
};

class outputData
{
    public:
        int rows , columns ;
        bool validresult ;
        bool isScalar ;
        int resultScalar ;
        vector<vector<int>> resultMatrix ;
    
        void initialize_resultMatrix(int r , int c) ;
} ;

void setMatrices(inputData &indata , outputData &outdata , Logger &log , int noOfMatrices , int noOfScalars) ;

#endif