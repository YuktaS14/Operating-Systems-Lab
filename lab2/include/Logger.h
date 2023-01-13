#ifndef LOGGER_H
#define LOGGER_H

#include<iostream>
#include<fstream>
#include<vector>

using namespace std ;

class Logger
{
    public :
        string logFileName ;    // log filename
        ofstream logFile ;      // log file stream object

        void logMessage(string msg) ;     // this fun helps to add "msg" to log file

        // using operator overloading for operator << to log a string "msg" into log file
        Logger& operator<<(string msg) ;

        // using operator overloading for operator << to log a int "msg" into log file
        Logger& operator<<(int number) ;

        // printing matrix to log file
        void logMatrix(vector<vector<int>> &matrix) ;

        // printing matrix to output file
        void ResultantMatrix(vector<vector<int>> &matrix);

        //printing scalar to output file
        void ResultantScalar(int scalar);
        // opening logfile in write mode using constructor
        void openFile(string filename) ;

        // closing log file 
        void closeFile() ;
} ;

#endif