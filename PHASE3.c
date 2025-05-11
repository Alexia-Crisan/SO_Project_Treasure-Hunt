#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "PHASE1.h"
#include "PHASE3.h"

int calculate_score_one_game(char hunt[30], char Username[30])
{
  DIR *folder;

  //open game folder
  folder = opendir(hunt);
  if(folder == NULL)
   {
     printf("Game doesn't exist.\n");
     exit(-1);
   }
   
  //open file
  char filepath[50];
  sprintf(filepath, "%s/Game.b", hunt);
  
  int f; //treasure file
  if((f = open(filepath, O_RDONLY)) == -1)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }

  //iterate through treasures
  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      close(f);
      closedir(folder);
      perror("Error creating a treasure: ");
      exit(-1);
    }

  int sum = 0;
  while(read(f, fortune, sizeof(TREASURE)))
    {
      if(strcmp(fortune->User_name, Username) == 0)
	{
	  sum += fortune->value;
	}
    }

  close(f);
  closedir(folder);
  free(fortune);

  return sum;
}

int calculate_score(char Username[30])
{
  struct dirent *file;
  const char *folder_path = "./"; //root game directory
  DIR *folder = opendir(folder_path);

  if(folder == NULL)
   {
     perror("Error opening root game directory");
     exit(-1);
   }

  //create a pipe to send score from child to parent
  int pipefd[2];
  if (pipe(pipefd) == -1)
    {
      perror("Pipe error");
      exit(-1);
    }

  while((file = readdir(folder)) != NULL)
    {
      if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
	continue;
      
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".git") != 0)
	{
	  char game_name[512];
	  sprintf(game_name, "%s", file->d_name);

	  int pid = fork();
	  if (pid < 0)
	    {
	      perror("Unable to open process");
	      exit(-1);
	    }
	  if (pid == 0)
	    {
	      //in child process
	      int score = calculate_score_one_game(game_name, Username);

	      //write the score to the pipe
	      close(pipefd[0]);  //close reading end
	      write(pipefd[1], &score, sizeof(score));
	      close(pipefd[1]);  //close writing end
	      exit(0); 
	    }
	}
    }
  
  while(wait(NULL) > 0)
  {
    //wait for all child processes
  }
  
  //read scores from pipe
  close(pipefd[1]);  //close writing end
  int score, total_score = 0;
  while (read(pipefd[0], &score, sizeof(score)) > 0)
    {
      total_score += score;
    }
    
  close(pipefd[0]);  //close reading end

  closedir(folder);
  return total_score;
}

void write_in_pipe(int pfd[2])
{
  close(pfd[0]); //close reading end			 
  dup2(pfd[1], STDOUT_FILENO);
  close(pfd[1]); // close writing end
}

void print_from_pipe(int pfd[2])
{
  FILE *stream;
  char buffer[2048] = {0};

  close(pfd[1]); // close writing end
  stream = fdopen(pfd[0], "r");
  if (!stream)
    {
      perror("Fdopen failed");
      exit(-1);
    }

  while (fgets(buffer, sizeof(buffer), stream))
    {
      printf("%s", buffer);
    }

  fclose(stream);
  //close(pfd[0]); //close used end //implicit with fclose
  
}
