#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>


int writer(const char* location, const char* string);


int main(int argc, char** argv)
{
    int status = 0;
    if(argc != 3)
    {
       syslog(LOG_ERR, "Invalid number of arguments, need: %d received: %d",3, argc);
       status = 1;
    }
    else
    {
	status = writer(argv[1], argv[2]);
    }

    return status;
}

int writer(const char* location, const char* string)
{
    int status = 1;
    openlog("writer.c", LOG_PID | LOG_CONS, LOG_USER);
    
    if(string != NULL && location != NULL)
    {   
	int fd;
	fd = open (location, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  

	if(fd == -1)
	{
           /* File open(create) failed */
	   syslog(LOG_ERR, "Failed to open file: %s (errno: %d)",location,errno);
           
	}
        else
        {
            int str_size = strlen(string);
            
            syslog(LOG_DEBUG, "Writing %s to %s", string, location);
            if( write(fd, string, str_size) != str_size)
            {
                /* File write failed */
                syslog(LOG_ERR, "Failed to write: %s to file: %s (errno: %d)", string, location,errno); 
            }
            else
            {
                /*File write succeeded*/
                syslog(LOG_DEBUG, "Wrote %s to %s", string, location);
		status = 0;     
            }
            
            if(close(fd) == -1)
            {
                /* File close failed */
                syslog(LOG_ERR, "Failed to close file: %s (errno: %d)", location,errno); 
            }
        }
    }
    else
    {
       /* Invalid arguments */
       syslog(LOG_ERR, "Invalid arguments");
    }

    closelog();
    return status;
} 








