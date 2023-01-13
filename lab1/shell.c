#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include <fcntl.h>


char cwd[1024];
bool isExit = false ;
bool backgroundProcess ;
int stdInput,stdOutput,stdErr;
pid_t pid;

//Initially file descriptors 0(stdInput), 1(stdOutput) points to the terminal
//so after executing commands if we modify the file descriptors , and if we want to restore back our stdInput, stdoutput to point back to terminal,
//we need to store them before modifying;

void storeFD()
{
        stdInput = dup(0);
        stdOutput = dup(1);
	stdErr = dup(2);
}

//^C (enter)
//it sends signal for the current process to terminate from shell
void Sigint_handler(int sig_num)
{
        if(kill(pid,SIGTERM) == 0)
                write(1,"\nCtrl+C executed : SIGINT signal\n", sizeof("\nCtrl+C executed : SIGINT signal\n"));
}

//^Z (enter)
//suspends the process
//SIGSTOP command cannot be caught, its handled by kernel 

//sigtstp, sigstop by default pause the process and sends to background which can be resumed back later using sigcont signal or fg command

void Sigtstp_handler(int sig_num)
{
        
        write(1,"\nCtrl+Z executed : SIGTSTP signal\n", sizeof("\nCtrl+Z executed : SIGTSTP signal\n")); 
}

// sends a signal when a child terminates so that the zombie can be removed from process table
void Sigchld_handler(int sig_num)
{
        //WNOHANG status returns 0 if all childs are returned;
        while(waitpid(-1,NULL, WNOHANG) > 0)
        {
                continue;
        }          
}

void executeCommand(char* tokens[]) ;
int implementPipes(char *tokens[]) ;
void inputRedirection(char *inputfilename) ;
void outputRedirection(char *outputfilename) ;
void output_error_redirection(char *outputfilename) ;
void tokenize(char *command, char **tokens) ;           // tokenizing the command given

int main()
{          
        //storing initial fd 0, fd 1,fd 2
        storeFD();
        
        // whenever signum gets value signal is invoked and is handled by sig_handler
        signal(SIGINT,Sigint_handler);
        signal(SIGCHLD,Sigchld_handler);
        signal(SIGTSTP,Sigtstp_handler);
        
        printf("\n**************************************************************");
        printf("\n*                                                            *");
        printf("\n*                       __SHELL__                            *");
        printf("\n*                  112001009_112001052                       *");
        printf("\n*                                                            *");
        printf("\n**************************************************************");
        fflush(0);
        while(1)
        {
                if ( !isExit )
                {
                        // printing the command prompt as ~~cwd$$ (fd = 1 : stdout)
                        write(1,"\n\n~~",3);

                        // cwd - current working directory
                        if( getcwd( cwd , sizeof(cwd) ) != NULL )
                        {
                        write(1,cwd,strlen(cwd));
                        }
                        write(1,"$$ ",3);
                        char input[1024];

                        //read command (fd = 0 : stdin)
                        //read system call returns number of bytes read
                        backgroundProcess = false ;     // initialising as false

                        char *tokens [513]; 
                        int n = read(0,input,1024) ;
                        
                        if ( n > 0 )
                        {
                                // we have to tokenize the command received
                                if(input[0] == '\n')
                                        continue;

                                input[n] = '\0';
                                if ( input[n-1] == '\n' )
                                {
                                        input[n-1] = '\0' ;
                                }

                                if( input[n-2] == '&' )
                                {
                                backgroundProcess = true;
                                input[n-2] = '\0';
                                }

                                tokenize(input,tokens);
                        }
                        else    // number of bytes read can be zero 
                        {
                                //in such a case we will exit from our shell with an error
                                perror("bash: read: Unable to read command");
                                exit(-1) ;
                        }
                }
                else
                {
                        // exiting the program
                        break;
                }
        }
	return 0;
}

/* tokenize - returns the number of tokens it made after tokenizing 
              since it is useful to check the last token made is "&" or not 
              for knowing whether the process is in background mode or not */
void tokenize(char *command, char **tokens)
{
       int count = 0 ;
        // we will tokenize command using certain delimiters
        // delimiters - "\t" , " "
        char *temp;
	temp = strtok(command," \n\t\r");

        char *inputfilename ;
        char *outputfilename ;
 
	while(temp != NULL)
	{       
                // checking for I/O redirection and pipes while tokenizing

                if( strcmp(temp , "<<<") == 0 )        // input redirection
                {
                        inputfilename = strtok(NULL , " \n\t") ;
                        inputRedirection(inputfilename) ;
                }
                else if ( strcmp(temp , ">>>") == 0 )          // output redirection
                {
                        outputfilename = strtok(NULL , " \n\t") ;
                        outputRedirection(outputfilename) ;
                }
                else if ( strcmp(temp , "&>>") == 0 )           // redirection of both stdout and stderr
                {
                        outputfilename = strtok(NULL , " \n\t") ;
                        output_error_redirection(outputfilename) ;
                }
                else if ( strcmp(temp , "||") == 0 )           // if token is pipe 
                {
                        // if command to be executed is built-in making last token as null will not affect its execution
                        // if command to be executed is not built-in then we have to execute by execvp command 
                        // execvp command expects arguments list to be ended by null 
                        tokens[count] = NULL ;  
                        implementPipes(tokens);  

                        // count is set to zero since it should start execution of next command and tokens should be for stored only for next command
                        count = 0 ;      
                }
                else
                {
                        // if it is not any operator then it is a argument of command and it should be stored as token
                        tokens[count] = temp;
                        count++;
                }

		temp = strtok(NULL, " \n\t");
	}
	
        tokens[count] = NULL ;  // last token is null for execution of command as said before

        // if tokens[0] is null then there is no command to execute , so return
        if(tokens[0] == NULL)
        {
                return ;
        }
        executeCommand(tokens) ;
}

// In inputredirection (<<<) : our stdin should be changed to file with filename "inputfilename"
void inputRedirection(char *inputfilename) 
{
        int inputfile_fd = open(inputfilename , O_RDONLY )  ;

        if(inputfile_fd == -1)
        {
                char msg[] = "Error in opening the file given for input redirection" ;
                write(1,msg,strlen(msg));
                exit(-1);
        }
        // now stdin will be replaced by value of filedescriptor "inputfile_fd"
        dup2(inputfile_fd , 0) ;

        // And inputfile_fd is not needed further since value of it is already duplicated to stdin
        close(inputfile_fd) ;
}

// stdout should be redirected(>>>) to file having file name "inputfilename"
void outputRedirection(char *outputfilename)
{
        int outputfile_fd = open(outputfilename , O_CREAT | O_WRONLY , 0700 ) ;

        if(outputfile_fd == -1)
        {
                char msg[] = "Error in opening the file given for output redirection" ;
                write(1,msg,strlen(msg));
                exit(-1) ;
        }
        // now stdout will be replaced by value of filedescriptor "outputfile_fd"
        // And outputfile_fd is not needed further
        dup2(outputfile_fd , 1) ;
        close(outputfile_fd) ;
}

// "&>>" : it should redirect both stdout and stderr both to file with filename "outputfilename"
void output_error_redirection(char *outputfilename) 
{
        int outputfile_fd = open(outputfilename , O_CREAT | O_WRONLY  , 0700) ;
        if(outputfile_fd == -1)
        {
                char msg[] = "Error in opening the file given for output&error redirection" ;
                write(1,msg,strlen(msg));
        }

        // Now both stdout , stderr will point to outputfile
        dup2(outputfile_fd , 1) ;
        dup2(outputfile_fd , 2) ;
        close(outputfile_fd) ;
}

void executeCommand(char**tokens)
{
        //Checks whether given command is built-in command or not
        // Built in commands : "cd" , "pwd" , "exit" i.e. 1st token is any of them or not
        if ( strcmp( tokens[0] , "cd") == 0 )           // matches with cd
        {
                // built-in cd command execution
                if ( tokens[1] == NULL || *tokens[1] == '~' )         // no directory is given as argument
                {
                        chdir(getenv("HOME")) ;
                }
                else
                {
                        // on calling successfully chdir , 0 is returned       
                        int ret = chdir(tokens[1]);
                                
                        if(ret != 0)
                        {
		                perror("Something went wrong");
                        }
                }                
        }
        else if( strcmp(tokens[0], "pwd") == 0)
	{
                // cwd contains current working directory (see while printing for prompt in main function)
	        char message[] = "\nCurrent Working Directory is:  ";
		write(1,message,strlen(message));
		write(1,cwd,strlen(cwd));
	}
	else if(strcmp(tokens[0], "exit") == 0)
	{
                char msg[] = "Exiting custom shell\n" ;
                write(1,msg,strlen(msg));
                isExit = true ;         
       	}
        else
        {
                pid = fork();
                int res;

                if(pid<0)
                {
                        perror("Fork failed");
                }
                else if(pid == 0)       // Child process
                {
                        res = execvp(tokens[0],tokens);
                        if(res == -1)
                        {
                                perror("Exec failed: Command not found ");
                                exit(1);
                        }
                }                        
                else    // Parent process
                {
                        //if process is to be done in foreground, the parent has to wait till its child completes the task
                        if(!backgroundProcess)
                        {             
                                //suspends parent process till child has exited                 
                                int wp = waitpid(pid,NULL,0);
                        }                                  
                }
        }  
        
        //giving the terminal control back to fd 0, 1, 2 (stdInput, stdOutput,stdErr resp)
        dup2(stdInput,0);
        dup2(stdOutput,1);   
	dup2(stdErr,2);
}



int implementPipes(char *tokens[])
{
        //creating pipe with write(fdarray[1]) and read(fdarray[0]) ends
        int fdarray[2];
        int ret = pipe(fdarray);
        
        //in case of failure while creting pipe it return -1;
        if(ret == -1)
        {
               perror("Failed while implementing pipe");
               return -1;           
        }

        //std Ouput points to fdarray[1]
        //connecting write end of pipe to fd 1
        dup2(fdarray[1],1);
        close(fdarray[1]);

        //executing the command , the output of which would be written into the pipe
        executeCommand(tokens);

        //after executing the command we connect the read end of the pipe to fd 0 (std Input)
        //so next command after || will be executed in another child 
        //for which std input would be pointing to read end of this pipe
        dup2(fdarray[0],0);
        close(fdarray[0]);
}
