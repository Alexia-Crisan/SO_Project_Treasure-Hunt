#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "PHASE1.h"

#define SIG_LIST_HUNTS     SIGUSR1
#define SIG_LIST_TREASURES SIGUSR2
#define SIG_VIEW_TREASURE  SIGUSR1
#define SIG_STOP_MONITOR   SIGUSR1

int monitor_running = 0;
int monitor_pid;

void print(int sig)
{
  printf("caca");
  fflush(stdout);
}

void monitor()
{
  struct sigaction sa;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  sa.sa_handler = print;
  sigaction(SIG_LIST_HUNTS, &sa, NULL);

  sa.sa_handler = print;
  sigaction(SIG_LIST_TREASURES, &sa, NULL);

  sa.sa_handler = print;
  sigaction(SIG_LIST_HUNTS, &sa, NULL);

  sa.sa_handler = print;
  sigaction(SIG_LIST_HUNTS, &sa, NULL);
  
  //wait for signal
  while (1)
    {
      pause(); 
    }
}

int main()
{
  while(1)
    {
      char command[30];
      printf("treasure_hub>> ");
      scanf("%s", command);
      command[strcspn(command, "\n")] = 0;
      
      if(strcmp(command, "start_monitor") == 0)
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

	  else if(strcmp(command, "list_hunts") == 0)
	    {
	      if(!monitor_running)
		{
		  printf("Error: Monitor not running.\n");
		}
	      else
		{
		  //send signal stuff
		  kill(monitor_pid, SIG_LIST_HUNTS);
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
		  kill(monitor_pid, SIG_LIST_TREASURES);
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
		  kill(monitor_pid, SIG_VIEW_TREASURE);
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
		  kill(monitor_pid, SIG_STOP_MONITOR);
		}
	    }

	  else if(strcmp(command, "exit") == 0)
	    {
	      if(!monitor_running)
		{
		  printf("Error: Monitor not running.\n");
		}
	      else
		{
		  //send signal stuff
		}
	    }
	  
	  else
	    {
	      printf("Unknown command: %s\n", command);
	    }
    }
  
  return 0;
}

