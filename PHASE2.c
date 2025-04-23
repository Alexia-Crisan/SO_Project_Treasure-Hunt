#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "PHASE1.h"
#include "PHASE2.h"

int get_number_of_treasures(char hunt[30])
{
  DIR *folder;

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

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      close(f);
      closedir(folder);
      perror("Error creating a treasure: ");
      exit(-1);
    }

  //count treasures
  int count = 0;
  while(read(f, fortune, sizeof(TREASURE)))
    {
      count ++;
    }
  
  close(f);
  closedir(folder);
  free(fortune);

  return count;
}

void list_hunts()
{
  struct dirent *file;
  const char *folder_path = "./"; //root game directory
  DIR *folder = opendir(folder_path);

  if(folder == NULL)
   {
     perror("Error at opening root game directory");
     exit(-1);
   }

  while((file = readdir(folder)) != NULL)
    {
      if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
	continue;
      
      if (file->d_type == DT_DIR)
	{
	  char game_name[512];
	  sprintf(game_name, "%s", file->d_name);
      
	  printf("Game name: %s\n", game_name);
	  printf("Number of treasure: %d\n\n", get_number_of_treasures(game_name));
	}
    }
}

void list_treasures(char hunt[30])
{
  list(hunt);
}

void view_treasure(char hunt[30], int treasure_ID)
{
  view(hunt, treasure_ID);
}
