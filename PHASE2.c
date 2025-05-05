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
#include "PHASE2.h"

#define COMMAND_SIZE 200 

int monitor_running = 0;
int monitor_killed;
int monitor_pid;

void list_hunts()
{
  struct dirent *file;
  const char *folder_path = "./"; //root game directory
  DIR *folder = opendir(folder_path);

  if(folder == NULL)
   {
     perror("Error opening root game directory");
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
      
	  list(game_name);
	}
    }
}

void delete_command()
{
  char filepath[50];
  strcpy(filepath, "command_file");
  
  int c; //command file
  if((c = open(filepath, O_WRONLY | O_TRUNC)) == -1)
  {
    perror("Error opening the command file");
    exit(-1);
  }

  close(c);
}

void handler(int sigtype)
{
  char filepath[50];
  strcpy(filepath, "command_file");
  
  int c; //command file
  if((c = open(filepath, O_RDONLY)) == -1)
  {
    perror("Error opening the command file");
    exit(-1);
  }

  char command[200] = {0};
  read(c, command, COMMAND_SIZE);

  command[strcspn(command, "\n")] = 0;

  if (strstr(command, "--list_hunts") != 0)
    {
      list_hunts();
      delete_command();
    }
  else if (strstr(command, "--list") != 0)
    {
      char hunt[30] = {0}, exec[30] = {0}, action[30] = {0};
      char *com = strtok(command, " ");
      strcpy(exec, com);
      com = strtok(NULL, " ");
      strcpy(action, com);
      com = strtok(NULL, " ");
      strcpy(hunt, com);

      char *arg[4];
      arg[0] = exec;   
      arg[1] = action;  
      arg[2] = hunt;   
      arg[3] = NULL; //mandatory

      int pid = fork();

      //Error fork
      if (pid < 0)
	{
	  perror("Unable to open process");
	  exit(-1);
	}
      if (pid == 0)
	{
	  if(execv(exec, arg) == -1)
	    {
	      perror("execv failed");
	      exit(1);
	    }	  
	}
      
      //list(hunt);
      delete_command();
    }
  else if (strstr(command, "--view") != 0)
    {
      char hunt[30] = {0};
      char *com = strtok(command, " ");
      com = strtok(NULL, " ");          
      strcpy(hunt, com);
      com = strtok(NULL, " ");          
      int ID = strtol(com, NULL, 10);

      view(hunt, ID);
      delete_command();
    }

  close(c);
}

void handler_stop_monitor(int sigtype)
{
  usleep(3000000);
  exit(0);
}

void monitor()
{
  struct sigaction sa;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
        
  if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
      perror("Sigaction error");
      exit(-1);
    }
  
  struct sigaction sa_stop_monitor;
  sa_stop_monitor.sa_flags = 0;
  sigemptyset(&sa_stop_monitor.sa_mask);
  sa_stop_monitor.sa_handler = handler_stop_monitor;
        
  if (sigaction(SIGTERM, &sa_stop_monitor, NULL) == -1)
    {
      perror("Sigaction error");
      exit(-1);
    }
  
  //wait for signal
  while (1)
    {
      pause(); 
    }

  exit(0);
}

void start_monitor()
{
  if(monitor_running)
    {
      printf("Monitor already running.\n");
    }
  else
    {
      monitor_pid = fork();

      //Error fork
      if (monitor_pid < 0)
	{
	  perror("Unable to open process");
	  exit(-1);
	}

      //in monitor
      if (monitor_pid == 0)
	{
	  //monitor_stuff
	  monitor_running = 1;
	  
	  //recieves_signals and do stuff
	  monitor();
	}
      else
	{ 
	  monitor_running = 1;
	  printf("[Hub] Monitor started with PID %d\n", monitor_pid);
	}     
    }
}

void add_command_to_file(char message[200])
{
  //create command file
  char filepath[50];
  strcpy(filepath, "command_file");
  
  int c; //command file
  if((c = open(filepath, O_WRONLY | O_CREAT, 0777)) == -1)
  {
    perror("Error opening the command file");
    exit(-1);
  }

  write(c, message, strlen(message));

  close(c);
}

void list_treasures()
{
  char hunt[30] = {0};
  printf("Enter hunt: ");
  scanf("%s",hunt);
  hunt[strcspn(hunt, "\n")] = 0;
  
  char full_message[200] = {0};
  sprintf(full_message,"%s %s %s", "./exec_PHASE1","--list", hunt);
  add_command_to_file(full_message);
  kill(monitor_pid, SIG_LIST_TREASURES);
}

void view_treasure()
{
  char hunt[30] = {0};
  int ID;
  printf("Enter hunt: ");
  scanf("%s",hunt);
  hunt[strcspn(hunt, "\n")] = 0;
  printf("Enter treasure ID: ");
  scanf("%d", &ID);
  
  char full_message[200] = {0};
  sprintf(full_message, "%s %s %d", "--view", hunt, ID);
  add_command_to_file(full_message);
  kill(monitor_pid, SIG_VIEW_TREASURE);
}

void list_hunts_wrapper()
{
  char full_message[200] = {0};
  sprintf(full_message,"%s", "--list_hunts");
  add_command_to_file(full_message);
  kill(monitor_pid, SIG_LIST_HUNTS);
}


void stop_monitor()
{
  printf("Monitor is stopping; stop sending commands!\n");
  
  // send SIGTERM signal
  kill(monitor_pid, SIG_STOP_MONITOR);
  monitor_killed = 1;
  
  while(monitor_pid != -1)
    {
      if(fgetc(stdin) == '\n')
	{
	  printf("Illegal action while monitor is stoppping!\n");
	}      
    }
 
  int status;
  int return_waitpid = waitpid(monitor_pid, &status, 0);
  if (return_waitpid == -1)
    {
      perror("Waitpid error");
      exit(-1);
    }
  
  //WIF = Wait IF
  if (WIFEXITED(status))
    {
      //Checks if the child process terminated via exit() or return
      printf("Monitor terminated with status %d\n", WEXITSTATUS(status));
      //WEXITSTATUS(status) extracts the exit code
    }
  else if (WIFSIGNALED(status))
    {
      //Checks if the child process terminated via by a signal
      printf("Monitor killed by signal %d\n", WTERMSIG(status));
      //WTERMSIG(status) gives the signal number that caused the process to terminate.
    }
  else
    {
      printf("Monitor terminated abnormally.\n");
    }
}


