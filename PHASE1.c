#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "PHASE1.h"

TREASURE *create_treasure()
{
  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure: ");
      exit(-3);
    }

  printf("Add details about the treasure:\n");
  
  printf("Treasure ID: ");
  scanf("%d", &fortune->ID);

  printf("Username: ");
  scanf("%s", fortune->User_name);

  printf("GPS coordinates:\n");
  printf("Latitude:");
  scanf("%f", &fortune->GPS_lat);
  printf("Longitude: ");
  scanf("%f", &fortune->GPS_long);

  printf("Value: ");
  scanf("%d", &fortune->value);
  
  while (getchar() != '\n'); 

  printf("Hint: ");
  fgets(fortune->hint, 99, stdin);

  return fortune;
}

void add(char hunt[30])
{
  DIR *folder;

  folder = opendir(hunt);
  if(folder == NULL)
   {
     //create directory
     if (mkdir(hunt, 0777) == 0)
     {
       printf("Directory '%s' created successfully.\n", hunt);
     }
     else
     {
       perror("Error creating the game directory: ");
       exit(-2);
     }
   }
   
  //open file
  char filepath[50];
  sprintf(filepath, "%s/Game.b", hunt);
  FILE *f; //treasure file
  if((f = fopen(filepath, "wb")) == NULL)
  {
    perror("Error opening the treasure file: ");
    exit(-2);
  }

  TREASURE *fortune = create_treasure();
      
  //add  trasure
  fwrite(fortune, sizeof(TREASURE), 1, f); 

  free(fortune);
  fclose(f);
  closedir(folder);
}
