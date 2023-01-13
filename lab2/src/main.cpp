#include<iostream>
#include<unistd.h>
#include<fstream>
#include "data.h"
#include "readInput.h"
#include "ScalarOp.h"
#include "MatrixOp.h"
#include "Logger.h"
using namespace std;

int main(int argc, char *argv[])
{
    // options applicable in command
    char options[] = "asmdASMTX";

    // options read in the command 
    int cread;

    // creating an object of data
    inputData inData ;
    outputData outData ;

    // creating a logger file & opening it
    Logger logger ;
    logger.openFile("log.txt") ;

    // creating an output file for output
    Logger outputFile ;
    outputFile.openFile(argv[argc-1]) ;
    
    logger << "Input file : " << argv[argc-2] <<"\n" ;
    // Parsing the input file (argv[argc-1])
    inData.validInput = true ;
    readInput(argv[argc-2] , inData , outData , logger);

    // If inputfile contains invalid input then operations will not be executed
    if(inData.validInput == false)
    {
        logger.logMessage("Input file contains invalid data\n") ;
        logger.logMessage("Execution will not start") ;
        logger.closeFile() ;
        outputFile.closeFile() ;
        return -1 ;
    }

    // After parsing the input file if MatrixList is empty then no matrices are given in inputfile which means any operation execution cant happen 
    if( inData.MatrixList.empty() )
    {
        logger.logMessage("There are no matrices in inputfile which means no operations can happen") ;
        logger.logMessage("Execution will not start") ;
        logger.closeFile() ;
        outputFile.closeFile() ;
        return -1 ;
    }
    else
    {
        logger << "Parsed input file successfully" << "\n" ;
        outData.resultMatrix = inData.MatrixList.front() ;
        outData.rows = outData.resultMatrix.size() ;
        outData.columns = (outData.resultMatrix)[0].size() ;
        inData.MatrixList.pop() ;

        outData.isScalar = false ;
        outData.validresult = true ;
    }

    // Finds the options given in the command
    bool invalidOption = false ;
    int count = 0 ;
    while((cread = getopt(argc,argv,options)) != -1)
    {
        count++ ;
        if(outData.validresult == true )
        {
            if(count != 1)
            {
                logger << "\n\t" << argv[count-1] << " option executed" ;

                if(outData.isScalar == true)
                {
                    logger << "\nOutput of " << argv[count-1] << " is" << outData.resultScalar << "\n";
                }
                else
                {
                    logger << "\nOutput of " << argv[count-1] << " is : " ;
                    logger.logMatrix(outData.resultMatrix) ;
                    logger << "\n" ;
                }
            }

            switch(cread)
            {
                case 'a':
                    setMatrices( inData , outData , logger , 1 , 1) ; 
                    addScalar(inData , outData , logger);
                    break;
                case 's':
                    setMatrices( inData , outData , logger , 1 , 1) ; 
                    subScalar(inData , outData , logger) ;
                    break;
                case 'm':
                    setMatrices( inData , outData , logger , 1 , 1) ; 
                    mulScalar(inData , outData , logger) ;
                    break;
                case 'd':
                    setMatrices( inData , outData , logger , 1 , 1) ; 
                    divScalar(inData , outData , logger) ;
                    break;
                case 'A':
                    setMatrices( inData , outData , logger , 2 , 0) ; 
                    addMatrix(inData , outData , logger) ;
                    break;
                case 'S':
                    setMatrices( inData , outData , logger , 2 , 0) ; 
                    subMatrix(inData , outData , logger) ;
                    break;
                case 'M':
                    setMatrices( inData , outData  , logger, 2 , 0) ; 
                    mulMatrix(inData , outData , logger) ;
                    break;
                case 'T':
                    setMatrices( inData , outData , logger , 1 , 0) ; 
                    transposeMatrix(inData , outData , logger) ;
                    break;
                case 'X':
                    setMatrices( inData , outData , logger , 1 , 0) ; 
                    determinantMatrix(inData , outData , logger) ;
                    //cout << outData.resultScalar << endl ;
                    break;
                default:
                    logger << "\nInvalid command line option found" ;
                    logger.closeFile() ;
                    outputFile.closeFile() ;
                    invalidOption = true ;
                    outData.validresult = false ;
                    break;
            }
        }
        else
        {
            if(invalidOption == true)
            {
                return -1 ;     // handled in default of switch case
            }


            break;
        }
    }
    
    if(invalidOption == true)
    {
        return -1 ;     // handled in default of switch case
    }

    if(outData.validresult == true)
    {
        logger << "\n\t" << argv[count] << " option executed" ;
        if(outData.isScalar == true)
        {
            logger << "\nOutput of " << argv[count] << " is" << outData.resultScalar << "\n";
        }
        else
        {
            logger << "\nOutput of " << argv[count] << " is : " ;
            logger.logMatrix(outData.resultMatrix) ;
            logger << "\n" ;
        }
    } 

    // cout << endl ;
    // cout <<  outData.resultMatrix.size()  << " res r "<< outData.rows << endl  ;
    // cout << (outData.resultMatrix)[0].size() << " res c " << outData.columns << endl  ;
    // cout << outData.resultScalar << "res scalar" << endl ;


    // for(int i = 0 ; i < outData.rows ; i++)
    // {
    //     for(int j = 0 ; j < outData.columns ; j++)
    //     {
    //         cout << (outData.resultMatrix)[i][j] << " " ;
    //     }
    //     cout << endl ;
    // }

    if(outData.validresult == true)
    {
        if(outData.isScalar == true)
            outputFile.ResultantScalar(outData.resultScalar);
        else
            outputFile.ResultantMatrix(outData.resultMatrix);

    }


    return 0;
}