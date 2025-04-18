#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int monitor_running = 0;
int monitor_pid;

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
	      if ((monitor_pid = fork()) < 0)
		{
		  perror("Unable to open process");
		  exit(-1);
		}

	      //in monitor
	      if (monitor_pid == 0)
		{
		  //monitor_stuff
		  printf("monitor\n");
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

