#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PHASE1.h"

void gameplay(int argc, char **argv)
{
  if(argc == 1)
    {
      printf("No arguments have been given!\n");
      exit(-1);
    }
  
  else if(strcmp(argv[1], "--add") == 0)
    {
      if(argc != 3)
	{
	  printf("Unsuitable number of arguments for 'add' command.\n");
	  exit(-1);
	}
      else
	{	  
	  add(argv[2]);
	}
    }

  else if(strcmp(argv[1], "--list") == 0)
    {
      if(argc != 3)
	{
	  printf("Unsuitable number of arguments for 'list' command.\n");
	  exit(-1);
	}
      else
	{
	  list(argv[2]);
	}
    }

  else if(strcmp(argv[1], "--view") == 0)
    {
      if(argc != 4)
	{
	  printf("Unsuitable number of arguments for 'view' command.\n");
	  exit(-1);
	}
      else
	{
	  int treasure_ID = strtol(argv[3], NULL, 10);
	  view(argv[2], treasure_ID);
	}
    }

  else if(strcmp(argv[1], "--remove_hunt") == 0)
    {
      if(argc != 3)
	{
	  printf("Unsuitable number of arguments for 'remove_hunt' command.\n");
	  exit(-1);
	}
      else
	{
	  remove_hunt(argv[2]);
	}
    }

  else if(strcmp(argv[1], "--remove_treasure") == 0)
    {
      if(argc != 4)
	{
	  printf("Unsuitable number of arguments for 'remove_treasure' command.\n");
	  exit(-1);
	}
      else
	{
	  int treasure_ID = strtol(argv[3], NULL, 10);
	  remove_treasure(argv[2], treasure_ID);
	}
    }

  else
    {
      printf("Command not found.\n");
    }
}

int main(int argc, char **argv)
{
  gameplay(argc, argv);
  return 0;
}

  
