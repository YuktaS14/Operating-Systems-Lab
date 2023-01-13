#include "readInput.h"
#include "data.h"
#include "Logger.h"
using namespace std;

void readInput(char *optArg, inputData &indata , outputData &outdata , Logger &log)
{   
    // opening the file 
    fstream file ;
    file.open(optArg, ios::in);

    int rows = -1, cols = 0;
    int dimAreGiven = 0;

    vector<vector<int>> tempMatrix ;
    int i = -1, j = 0 ;
    int lineNo = 0;

    if (file.is_open()) //checking whether the file is open
    { 
        string data;    // data of the line read

        while(file.peek() != EOF)
        {    
            getline(file, data);    // reading each line
            lineNo++;

            if(i == rows-1)
            {
                // more no of rows than required in the matrix
                if(data.find(',') != string::npos)
                {
                    indata.validInput = false ;
                    log << "\nMore rows than required in the matrix (See line no : " << lineNo << ") in input file\n" ; 
                    return ;
                }
                else
                {
                    // Matrix read successfully
                    indata.MatrixList.push(tempMatrix) ;
                    i = -1 ,j= 0;
                    rows = -1 , cols = 0 ;
                    dimAreGiven = 0 ;
                }
            }


            if(data.length() == 0)  // Empty lines 
            {
                continue;  
            }                    

            if(data[0] == '#')  // comment
            {
                continue;
            } 

            // if data line doesn't find " " and "," then 
            if (data.find(' ') == string::npos && data.find(',') == string::npos)
            {
                indata.ScalarList.push(stoi(data));
            }   
            else if(data.find(' ') != string::npos)     // dimensions of matrix
            {
                int setDim = 0;
                string val = "";
                
                for(auto x: data)
                {
                    if(setDim == 2)
                    {
                        log << "\nDimensions are not for the 2D matrix (See line no : " << lineNo << ") in input file\n" ; 
                        indata.validInput = false;
                        return;
                    }

                    if(x == ' ')
                    {   
                        if(setDim==0)
                        {
                            rows = stoi(val);
                        }
                        else
                        {
                            cols = stoi(val);
                        }
                        val = "";
                        setDim++;
                        
                    }
                    else
                    {
                        val += x;
                    }
                }

                if(setDim == 1)
                {
                    cols = stoi(val) ;
                }
                

                dimAreGiven = 1;

                // initialising tempmatrix
                tempMatrix = vector<vector<int>>(rows, vector<int>(cols,0));

                //for new matrix
                i = -1 , j = 0;
            }
            else if(data.find(',') != string::npos)
            {
                if(dimAreGiven == 0)
                {
                    log << "\nDimensions are not given for the matrix (See line no : " << lineNo << ") in input file\n" ; 
                    indata.validInput = false;
                    return;
                }

                string val = "";
                i++,j= -1;
                
                for(auto x: data)
                {
                    if(x == ',')
                    {
                        j++ ;
                        if(j >= cols)
                        {
                            log << "\nMore columns than required in the matrix (See line no : " << lineNo << ") in input file\n" ; 
                            indata.validInput = false;
                            return;
                        }

                        tempMatrix[i][j] = stoi(val);
                        val = "";
                    }
                    else
                    {
                        val += x;
                    }
                }

                if(j < cols - 1)
                {
                    log << "\nLess columns than required in the matrix (See line no : " << lineNo << ") in input file\n" ; 
                    indata.validInput = false ;
                    return ;
                }
            }
        }

        indata.MatrixList.push(tempMatrix);        

    }
    else
    {
        //cout << "ERROR : Unable to open input file \"" << optArg << "\"" << endl ;
        log << "ERROR : Unable to open input file \"" << optArg << "\"" ;
    }

    file.close();  
}
