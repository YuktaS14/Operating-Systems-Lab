#include "MatrixOp.h"
#include "Logger.h"


void addMatrix( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    if (indata.rows1 == indata.rows2)
    {
        if ( indata.columns1 == indata.columns2 )
        {
            outdata.initialize_resultMatrix(indata.rows1 , indata.columns1);
            outdata.validresult = true;

            for(int i = 0 ; i < indata.rows1; i++)
            {
                for(int j = 0 ; j < indata.columns1 ; j++ )
                {
                    (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] + (indata.matrix2)[i][j] ; 
                }
            }
            return;
        }
    }
    log << "\n  ERROR : The 2 matrices taken as input are not of same dimensions. -A operation cannot be executed \n";
    outdata.validresult = false ;
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}


void subMatrix( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    if (indata.rows1 == indata.rows2)
    {
        if ( indata.columns1 == indata.columns2 )
        {
            outdata.initialize_resultMatrix(indata.rows1 , indata.columns1);
            outdata.validresult = true;

            for(int i = 0 ; i < indata.rows1; i++)
            {
                for(int j = 0 ; j < indata.columns1 ; j++ )
                {
                    (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] - (indata.matrix2)[i][j] ; 
                }
            }
            return;
        }
    }
    log << "\n  ERROR : The 2 matrices taken as input are not of same dimensions. -S operation cannot be executed \n";
    outdata.validresult = false ;
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}


void mulMatrix( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    if (indata.columns1 == indata.rows2)
    {
        ////cout << "hlo" ;
        outdata.initialize_resultMatrix(indata.rows1 , indata.columns2);
        outdata.validresult = true;

        for(int i = 0 ; i < indata.rows1; i++)
        {
            for(int j = 0 ; j < indata.columns2 ; j++ )
            {
                // AxB = C then C[i][j] = sigma(A[i][k]*B[k][j]) over k from 0 to indata.columns1-1
                for(int k = 0 ; k < indata.columns1 ; k++)
                {
                    (outdata.resultMatrix)[i][j] += (indata.matrix1)[i][k] * (indata.matrix2)[k][j] ;
                    //cout << (indata.matrix1)[i][k] << " " << (indata.matrix2)[k][j] << " " ;
                    //cout << (outdata.resultMatrix)[i][j] << endl ;
                }
            }
        }
        return;
    }
    log << "\n  ERROR : The 2 matrices taken as input are not of same dimensions. -M operation cannot be executed \n";
    outdata.validresult = false ;
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}


void transposeMatrix(inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    outdata.validresult = true ;
    for(int i = 0 ; i < indata.rows1 ; i++)
    {
        for(int j = 0 ; j < indata.columns1 ; j++ )
        {
            (outdata.resultMatrix)[i][j] = (indata.matrix1)[j][i] ;
        }
    }
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}



void findCofactor(vector<vector<int>> &matTemp, vector<vector<int>>matrix1, int r, int c,int n)
{
    int i = 0, j = 0;

    for(int l = 0; l< n; l++)
    {
        for(int m = 0; m<n; m++)
        {
            if(l != r && m != c)
            {
                matTemp[i][j++] = matrix1[l][m];

                if(j == n-1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int findDet(vector<vector<int>> matrix1,int n)
{
    int det = 0;
    if(n == 1)
        return matrix1[0][0];

    vector<vector<int>>matTemp (n, vector<int> (n,0));
    int sign = 1;
    for(int i = 0; i<n; i++)
    {
        findCofactor(matTemp,matrix1,0,i,n);
        det += sign * matrix1[0][i]* findDet(matTemp,n-1);
        sign = (-1 * sign);
    }
    return det;
}


void determinantMatrix(inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }
    
    if(indata.rows1 != indata.columns1)
    {
        log << "\n  ERROR : The input matrix is not a square matrix. -X operation cannot be executed \n";
        outdata.validresult = false;
        return;
    }

    outdata.validresult = true;
    outdata.isScalar = true; 
    int n = indata.columns1;

    vector<vector<int>> matrix1 = indata.matrix1;
    int det = findDet(matrix1, n);
    //cout << "det" << det << endl ;
    outdata.resultScalar = det;
    outdata.resultMatrix.clear() ;
}