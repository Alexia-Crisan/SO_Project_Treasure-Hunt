#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PHASE1.h"

void gameplay(int argc, char **argv)
{
  if(strcmp(argv[1], "--add") == 0)
    {
      if(argc != 3)
	{
	  printf("Unsuitable number of arguments for 'add' command.\n");
	  exit(-1);
	}
      else
	{	  
	  add(argv[2]);

	  char filepath[50], name[50];
	  sprintf(filepath, "./%s/Game.b", argv[2]);
	  sprintf(name, "Game.b-<%s>", argv[2]);
	  if(symlink(filepath, name) == -1)
	    perror("Symbolic link creation failed");
	}
    }

  if(strcmp(argv[1], "--list") == 0)
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

  if(strcmp(argv[1], "--view") == 0)
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

  if(strcmp(argv[1], "--remove_hunt") == 0)
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

  if(strcmp(argv[1], "--remove_treasure") == 0)
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
}

int main(int argc, char **argv)
{
  gameplay(argc, argv);
  return 0;
}

  
