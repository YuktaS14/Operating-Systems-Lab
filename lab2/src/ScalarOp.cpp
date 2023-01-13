#include "ScalarOp.h"
#include "Logger.h"


void addScalar( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    outdata.initialize_resultMatrix(indata.rows1 , indata.columns1) ;

    for(int i = 0 ; i < indata.rows1 ; i++)
    {
        for(int j = 0 ; j < indata.columns1 ; j++ )
        {
            (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] + indata.scalar ;
        }
    }
    outdata.validresult = true ;   
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
}


void subScalar( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    outdata.initialize_resultMatrix(indata.rows1 , indata.columns1) ;

    for(int i = 0 ; i < indata.rows1 ; i++)
    {
        for(int j = 0 ; j < indata.columns1 ; j++ )
        {
            (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] - indata.scalar ;
        }
    }
    outdata.validresult = true ;  
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}



void mulScalar( inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    outdata.initialize_resultMatrix(indata.rows1 , indata.columns1) ;
    //cout << indata.rows1 << endl ;
    for(int i = 0 ; i < indata.rows1 ; i++)
    {
        for(int j = 0 ; j < indata.columns1 ; j++ )
        {
            //cout << "value" << endl ;
            (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] * indata.scalar ;
        }
    }
    outdata.validresult = true ; 
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
}


void divScalar(inputData &indata , outputData &outdata , Logger &log)
{
    if(outdata.validresult == false)
    {
        log << "\nExecution got terminated.Operation given can not be carried on available inputs.\n";
        return;
    }

    if( indata.scalar != 0 )
    {
        outdata.validresult = true ;
        outdata.initialize_resultMatrix(indata.rows1 , indata.columns1) ;

        for(int i = 0 ; i < indata.rows1 ; i++)
        {
            for(int j = 0 ; j < indata.columns1 ; j++ )
            {
                (outdata.resultMatrix)[i][j] = (indata.matrix1)[i][j] / indata.scalar ;
            }
        }
        outdata.validresult = true ;
        return ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    }
    log << "\n  ERROR : Division by 0 : -d Operation cannot be carried\n";
    outdata.validresult = false ;
    outdata.isScalar = false ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}


