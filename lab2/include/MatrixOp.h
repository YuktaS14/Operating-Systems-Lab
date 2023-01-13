#ifndef MATRIXOP_H
#define MATRIXOP_H

#include <iostream>
#include<vector>
#include "data.h"
#include "Logger.h"

using namespace std;

void addMatrix(inputData &indata , outputData &outdata , Logger &log);
void subMatrix(inputData &indata , outputData &outdata , Logger &log);
void mulMatrix(inputData &indata , outputData &outdata , Logger &log);

void transposeMatrix(inputData &indata , outputData &outdata, Logger &log);
void determinantMatrix(inputData &indata , outputData &outdata , Logger &log);
void findCofactor(vector<vector<int>>&matTemp, vector<vector<int>>matrix1, int r, int c,int n);
int findDet(vector<vector<int>> matrix1,int n);


#endif



