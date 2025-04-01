#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
  int ID;
  char User_name[30];
  int GPS_long, GPS_lat;
  int value;
  char hint[100]
}TREASURE;

void add(char hunt[30])
{
  DIR *folder;

  folder = opendir(hunt);
  if(folder == NULL)
   {
     //create directory
   }
   
   //open directory

     
   //add  trasure
     
  
   closedir(folder);
}

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

  
