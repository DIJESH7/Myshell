/*
Name: Dijesh Pradhan
ID: 1001516650
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10     // Mav shell only supports ten arguments


static void sigintHandler(int sig_num)
{

}

int main()
{

  char* cmd_str  = (char*) malloc( MAX_COMMAND_SIZE );

  char history[15][255]; //creating history so that it supports upto 15 command and 255 letters

  int history_index=0; //index for history at the beginning that is zero

  pid_t pid[15]; //Declaring array of PID so that I can use it to store different pids and use of listpids



  int i =0;

  //Used to silence the function of ctrl+z and ctrl+c
  struct sigaction act;

  memset(&act,'\0',sizeof(act));
  act.sa_handler=&sigintHandler;

  sigaction(SIGINT,&act,NULL);
  sigaction(SIGTSTP,&act,NULL);

int status;

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];
    //used to seperate the semicolon and kepp the seperated array saved in array
    char *semi[MAX_NUM_ARGUMENTS];
    // used to count the number of element in token array
    int   token_count = 0;
    // used to count the number of element in the semi array
    int   token_semi = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;

    // Pointer to point to the semi
    // parsed by strsep
    char *semicolon;

    char *working_semi = strdup(cmd_str);

    char *working_str  = strdup( cmd_str );

    char *temp;

    //array used to save the array seperated by whitespace
    //in the array of semi
    char *array [MAX_NUM_ARGUMENTS];
    int k;
    for( k=0;k<MAX_NUM_ARGUMENTS;k++)
    {
      array[k]=(char*)malloc(MAX_NUM_ARGUMENTS);
    }

    //char tocheck[3];
    //tocheck = cmd_str;

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;
    char *working_rootsemi= working_semi;


    while ( ( (semicolon = strsep(&working_semi, ";" ) ) != NULL) &&
              (token_semi<MAX_NUM_ARGUMENTS))
    {
      semi[token_semi] = strndup( semicolon, MAX_COMMAND_SIZE );
      if( strlen( semi[token_semi] ) == 0 )
      {
        semi[token_semi] = NULL;
      }
        token_semi++;

    }


    //used to compare if the first element is !
    //so that this if statement will run the history of the given number
    //if followed by !
    if(working_str[0]=='!')
   {
   char word2 [10];
   strcpy(word2,&working_str[1]);
     int final;
     final = atoi(word2);
     int i;
     for (i=0;i <history_index; i++)
     {
       if (final==i)
       {
         working_str = history[i];
         cmd_str=history[i];
       }
     }

   }


// Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }



    // Now print the tokenized input as a debug check
    // \TODO Remove this code and replace with your shell functionality

    // check if the user hits enter so that the code returns to the top to print msh>
    if (token[0]==NULL)
    {
    continue;
    }

    //check if the user enters exit or quit only; stops if so
    else if (((strcmp("exit",token[0])==0)&&(token[1]==NULL))||((strcmp ("quit",token[0])==0)&&(token[1]==NULL)))
    {
      exit(0);
    }

    //check if the user enters cd and performs the fuction of cd
     else if (strcmp(token[0],"cd")==0)
    {
      chdir(token[1]); //peforms the function of cd
      //copies and stores the command to the history array
      strncpy(history[history_index],cmd_str,255);
      if (history_index>14) //to store only 15 command
      history_index=0;
      history_index++;
    }
//this is used if two commands are passed like echo foo ;echo dd
    else if (token_semi>1)
    {
      int t;
      for(t=0;t<token_semi;t++)
      {
        int a =0;
          while ( ( (temp = strsep(&semi[t], WHITESPACE) ) != NULL) &&
            (token_count<MAX_NUM_ARGUMENTS))
            {
              array[a] = strndup( temp, MAX_COMMAND_SIZE );
              if( strlen( array[a] ) == 0 )
              {
                array[a] = NULL;
              }
              a++;
            }
            strncpy(history[history_index],cmd_str,255);
            if (history_index>14)
            history_index=0;
            history_index++;

            pid[i]=fork();
            if( pid[i] == 0 )
            {
              execvp(array[0],array );
              exit( EXIT_SUCCESS );
            }

            waitpid( pid[i], &status, 0 );
          }

    }


    else // if the user anything else like listpids, history or any other commads
    {

      // if the user enters listpids it displays all the PID of the command we wrote
      // PID are stored in the pid array
      if ((strcmp("listpids",token[0])==0)&&(token[1]==NULL))
      {
        int k;
        //prints the PID of the commands
        for(k=0;k<i;k++)
        printf("%d : %d\n",k,pid[k] );
        //saves the command to the history array
        strncpy(history[history_index],cmd_str,255);
        if (history_index>14)
        history_index=0;
        history_index++;

      }

      //if the user enter history this will print all the commands the user has entered
      else if ((strcmp("history",token[0])==0)&&(token[1]==NULL))
      {
        int i;
        for(i=0;i<history_index;i++)
        printf("%d : %s\n",i,history[i] );
      }

      else if (strcmp(token[0],"bg")==0)
      {
        kill(pid[i-1],SIGCONT);
        strncpy(history[history_index],cmd_str,255);
        if (history_index>14)
        history_index=0;
        history_index++;
      }

      else
      {
          pid[i]= fork();              //fork child

            if(pid[i]==0)
            {           //Child
              execvp(token[0], token);
              perror("Command not found");
              exit(0);
            }
            else
            {
              strncpy(history[history_index],cmd_str,255);
              if (history_index>14)
              history_index=0;
              history_index++;             //Parent
                  wait(NULL);
                //  printf("Child exited\n");
            }

      }
    i++;
  }
    free( working_root );
    free( working_rootsemi );


}


  return 0;
}
