#include "data.h"
#include "Logger.h"

void outputData::initialize_resultMatrix(int r , int c)
{
    resultMatrix.clear() ;
    resultMatrix = vector<vector<int>>(r , vector<int>(c,0)) ;
    rows = r ;
    columns = c ;
}

void setMatrices(inputData &indata , outputData &outdata , Logger &log , int noOfMatrices , int noOfScalars) 
{
    if(outdata.isScalar == true && noOfMatrices == 1 && noOfScalars == 1)
    {
        indata.scalar = outdata.resultScalar ;

        if( indata.MatrixList.empty() )
        {
            outdata.validresult = false ;
            return ;
        }

        indata.matrix1 = indata.MatrixList.front() ;
        indata.MatrixList.pop() ;
        indata.rows1 = indata.matrix1.size() ;
        indata.columns1 = (indata.matrix1)[0].size() ;
        // //cout << "? " << indata.rows1 << " " << indata.columns1 << endl ;
    }
    else if (outdata.isScalar == false && noOfMatrices == 1)
    {
        ////cout << "used det" << endl ;
        indata.matrix1 = outdata.resultMatrix ;
        indata.rows1 = indata.matrix1.size() ;
        indata.columns1 = (indata.matrix1)[0].size() ;
        ////cout << "rows " <<  indata.rows1  << " cols " << indata.columns1 << endl ;

        if(noOfScalars == 1)
        {
            if(indata.ScalarList.empty())
            {
                outdata.validresult = false ;
                return ;
            }

            indata.scalar = indata.ScalarList.front() ;
            indata.ScalarList.pop() ;
        }
    }
    else if(outdata.isScalar == false && noOfMatrices == 2)
    {
        indata.matrix1 = outdata.resultMatrix ;
        indata.rows1 = indata.matrix1.size() ;
        indata.columns1 = (indata.matrix1)[0].size() ;
        //cout << "size : " << indata.rows1 << " " << indata.columns1 << endl ;

        if( indata.MatrixList.empty() )
        {
            //cout << "Hello" ;
            outdata.validresult = false ;
            return ;
        }
        //cout << "Hi" ;
        indata.matrix2 = indata.MatrixList.front() ;
        indata.MatrixList.pop() ;
        indata.rows2 = indata.matrix2.size() ;
        indata.columns2 = indata.matrix2[0].size() ;
    }
       
}
