

#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>

char ** TokenizeString(char * myInput)	//A function to tokenize the string.
{
//This part breaks the string into tokens and stores a pointer to each token into "array[]"
    
  char* p;
  char **array= malloc(strlen(myInput)*sizeof(char*));
  int i;
  i=0;
  p= strtok(myInput, " ");

  while( p!=NULL)
  {
      array[i++]=p;
      p = strtok (NULL, " ");
  }
  
  array[i]= NULL;        //Placing null character to end the array where arguments end.

  return array;
}

void addToString(char * myString, char a)	//Adds a char after a string. 
{
  int length= strlen(myString);
  myString[length-1]=a;		//length-1 to avoid having '#' on next line. 
}

void executeMe(char ** tokenizedString)		//Executes the given command
{
   if(tokenizedString[0]==NULL)		//Empty command is given.
   {	
	return;
   }
  
   pid_t pid, wpid;
   int status;
   
   pid= fork();

   if (pid == 0)  //Child process
   {
    if (execvp(tokenizedString[0], tokenizedString) == -1)	//Executing user command and handling return value for execvp. (-1 if error)
    {
      perror("The following error occured: ");
    }
       
    exit(EXIT_FAILURE);	//Returning to continue running shell.
   }
  
   else if (pid > 0) //Parent process
   {  
      do
      {
        wpid= waitpid(pid, &status,WUNTRACED);	//Calling waitpid to wait for the child process and passing WUNTRACED to return if child has stopped. 

      } while(!WIFEXITED(status) && !WIFSIGNALED(status));  //Continue running loop until child process has exited normally or stopped by a signal.
   }
    
   else       //Child could not be created. (pid < 0)
   {
       perror("The following error occured:");
   }
    
 return;
    
}

int main(int argc, char *argv[])
{  
   time_t currentTime;
   char* myTime;

   // Obtain current time.
   currentTime = time(NULL);

   // Convert to local time format
   myTime = ctime(&currentTime);
   addToString(myTime, '#');	//Adds '#' after current time. 

   char buf[50]; //An array of char (string) where input string would be stored.
   char ** TokenizedString; // An array that points to each token of the input string. 

  while(1)
  {
    printf("%s ", myTime);  //Command prompt
    fgets(buf, 200, stdin); //Takes the input string and stores it into string buf of size 200. (Given)

    buf[strlen(buf)-1]='\0';
    TokenizedString= TokenizeString(buf);
 	
   if(strcmp(TokenizedString[0], "exit") ==0)  //break the loop if command is "exit" and ends the program. 
   {
       break;
   }

   else
   {
	executeMe(TokenizedString); 		//Takes the command and execute it (Takes care of all commands other than exit.)
   }

   free(TokenizedString);	//Freeing memory. 
  }
    
  return EXIT_SUCCESS;
}

