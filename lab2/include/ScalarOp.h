#ifndef SCALAROP_H
#define SCALAROP_H

#include <iostream>
#include "data.h"
#include "Logger.h"

using namespace std;

void addScalar(inputData &indata , outputData &outdata , Logger &log);
void subScalar(inputData &indata , outputData &outdata , Logger &log);
void mulScalar(inputData &indata , outputData &outdata , Logger &log);
void divScalar(inputData &indata , outputData &outdata , Logger &log);

#endif