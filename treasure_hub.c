#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "PHASE2.h"

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

