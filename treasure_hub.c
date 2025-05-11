#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "PHASE2.h"
#include "PHASE3.h"

void handler_sigchld(int sig)
{
  if (monitor_killed)
    {
      monitor_pid = -1;
      printf("[Hub] Monitor is  done.\n");
      
      monitor_killed = 0; 
    }
 }

void do_action(char command[30])
{
  if(strcmp(command, "start_monitor") == 0)
    {
      start_monitor();
    }
  
  else if(strcmp(command, "list_hunts") == 0)
    {
      if(!monitor_running)
	{
	  printf("Error: Monitor not running.\n");
	}
      else
	{
	  //send signal stuff
	  list_hunts_wrapper();
	  
	  //printf from pipe
	  usleep(100000);
	  print_from_pipe(pfd1);
	} 
    }
      
  else if(strcmp(command, "list_treasures") == 0)
    {
      if(!monitor_running)
	{
	  printf("Error: Monitor not running.\n");
	}
      else
	{
	  //send signal stuff
	  list_treasures();

	  //printf from pipe
	  usleep(100000);
	  print_from_pipe(pfd2);
	}
    }
  
  else if(strcmp(command, "view_treasure") == 0)
    {
      if(!monitor_running)
	{
	  printf("Error: Monitor not running.\n");
	}
      else
	{
	  //send signal stuff
	  view_treasure();

	  //printf from pipe
	  usleep(100000);
	  print_from_pipe(pfd3);
	}
    }

  else if(strcmp(command, "stop_monitor") == 0)
    {
      if(!monitor_running)
	{
	  printf("Error: Monitor not running.\n");
	}
      else
	{
	  //send signal stuff
	  stop_monitor();
	  monitor_running = 0;
	}
    }
      
  else if(strcmp(command, "exit") == 0)
    {
      if(monitor_running)
	{
	  printf("Error: Monitor still running.\n");
	  printf("Use stop_monitor command first.\n");
	}
      else
	{
	  exit(0);
	}  
    }
  
  else
    {
      printf("Unknown command: %s\n", command);
    }
}

int main()
{
  struct sigaction sa_sigchld;
  sa_sigchld.sa_flags = 0;
  sigemptyset(&sa_sigchld.sa_mask);
  sa_sigchld.sa_handler = handler_sigchld;
        
  if (sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
    {
      perror("Sigaction error");
      exit(-1);
    }

  while(1)
    {
      char command[30];
      printf("treasure_hub>> ");
      scanf("%s", command);
      command[strcspn(command, "\n")] = 0;
      
      do_action(command);
    }
  return 0;
}

