#include "Logger.h"

//constructor
void Logger::openFile(string filename)
{
    this->logFileName = filename ;
    this->logFile.open(logFileName) ;
}

void Logger::logMessage(string msg) 
{
    if(logFile.is_open())
    {    
        this->logFile << "\n" << msg ;
    }
    else
    {
        cout << "ERROR : Log file \"" << logFileName << "\" is not open to write message" << endl ;
    }
}     


Logger& Logger::operator<<(string msg)
{
    if(logFile.is_open())
    {
        this->logFile << msg  ;
        return *this ;
    }
    else
    {
        cout << "ERROR : Log file \"" << logFileName << "\" is not open to write message" << endl ;
        return *this ;
    }
}


Logger& Logger::operator<<(int number) 
{
    if(logFile.is_open())
    {
        this->logFile << number  ;
        return *this ;
    }
    else
    {
        cout << "ERROR : Log file \"" << logFileName << "\" is not open to write message" << endl ;
        return *this ;
    }
}


void Logger::logMatrix(vector<vector<int>> &matrix)
{
    if(logFile.is_open())
    {
        this->logFile << "\n[ " ;

        for(int i = 0 ; i < matrix.size() ; i++)
        {
            if(i != 0)
            {
                this->logFile << "  " ;
            }

            for(int j = 0 ; j < matrix[0].size() ; j++)
            {
                if( j != (matrix[0].size() - 1) )
                {
                    this->logFile << matrix[i][j] << ", " ;                        
                }
                else
                {
                    this->logFile << matrix[i][j] ;
                }
            }

            if( i != (matrix.size() - 1) )
            {
                this->logFile << "\n" ;
            }
        }

        this->logFile << " ]\n" ;
    }
    else
    {
        cout << "ERROR : Log file \"" << logFileName << "\" is not open to write matrix" << endl ;
    }
}

void Logger::ResultantMatrix(vector<vector<int>> &matrix)
{
    if(logFile.is_open())
    {
        this->logFile << matrix.size() << " "<<matrix[0].size()<<"\n";

        for(int i = 0 ; i < matrix.size() ; i++)
        {
            for(int j = 0 ; j < matrix[0].size() ; j++)
            {
                this->logFile << matrix[i][j] << "," ;
            }

            if( i != (matrix.size() - 1) )
            {
                this->logFile << "\n" ;
            }
        }
    }
    else
    {
        cout << "ERROR : Output file \"" << logFileName << "\" is not open to write matrix" << endl ;
    }
}

void Logger::ResultantScalar(int scalar)
{
    if(logFile.is_open())
    {
        this->logFile << scalar ;
    }
    else
    {
        cout << "ERROR : Output file \"" << logFileName << "\" is not open to write scalar" << endl ;
    }

}

void Logger::closeFile()
{
    if(logFile.is_open())
    {
        this->logFile.close() ;
    }
    else
    {
        cout << "ERROR : Log file \"" << logFileName << "\" is not open to close it" << endl ;
    }
}
