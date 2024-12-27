#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    bool status = false;
    if(cmd != NULL)
    {
		status = system(cmd) == 0;
    }
    return status;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
   
    for(int i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);
    
    pid_t pid = fork();
    if(pid < 0)
    {
		syslog(LOG_ERR,"fork() failed");
    }
    else if(pid == 0)
    {
		execv(command[0],command);
		perror("execv");          
        exit(EXIT_FAILURE); 
    }
    else
    {
       int waitStatus;
   	   if(waitpid(pid, &waitStatus, 0) == -1)
   	   {
           syslog(LOG_ERR,"waitpid() failed");
           return false;          
	   } 
	   else if(WIFEXITED(waitStatus))
	   {
		   return (WEXITSTATUS(waitStatus) == 0);
       }   
   }
   return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
	if(outputfile != NULL)
	{
		int fd = open(outputfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if(fd < 0)
		{
	       syslog(LOG_ERR,"failed to open: %s", outputfile);          
		}
		else
		{
			va_list args;
			va_start(args, count);
			
			char * command[count + 1];
            for(int i = 0; i < count; ++i)
			{
				command[i] = va_arg(args, char*);
			}
			command[count] = NULL;
            va_end(args);

			pid_t pid = fork();
			if(pid < 0)
			{
		        syslog(LOG_ERR,"fork() failed");          
				close(fd);
			}
			else if (pid == 0)
			{
			    syslog(LOG_INFO,"Child process PID: %d",getpid());          
				if(dup2(fd, STDOUT_FILENO) < 0)
				{
				    syslog(LOG_ERR,"fork() failed");          		
				}
				close(fd);

				execv(command[0], command);
			    perror("execv");
                exit(EXIT_FAILURE);           
			}
			else
			{
				close(fd);
				int waitstatus;
				if(waitpid(pid, &waitstatus, 0) == -1)
				{
		            syslog(LOG_ERR,"waitpid() failed");
                    return false;
				}
				else if(WIFEXITED(waitstatus))
				{
		        	return (WTERMSIG(waitstatus) == 0);					
				}
			}        
		    
		}
	}
   
    return false;
}  
         

