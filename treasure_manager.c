#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
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
	}
    }
}

int main(int argc, char **argv)
{
  gameplay(argc, argv);
  return 0;
}

  
