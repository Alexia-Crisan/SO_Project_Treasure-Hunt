#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "PHASE1.h"

TREASURE *create_treasure()
{
  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure");
      exit(-1);
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
       perror("Error creating the game directory");
       exit(-1);
     }
   }
   
  //open file
  char filepath[50];
  sprintf(filepath, "%s/Game.b", hunt);
  FILE *f; //treasure file
  if((f = fopen(filepath, "ab")) == NULL)
  {
    perror("Error opening the treasure file");
    exit(-2);
  }

  TREASURE *fortune = create_treasure();
      
  //add  trasure
  fwrite(fortune, sizeof(TREASURE), 1, f); 

  free(fortune);
  fclose(f);
  closedir(folder);
}

void print_treasure(TREASURE *fortune, int index)
{
    if (fortune == NULL)
    {
        printf("No treasure to display.\n");
        return;
    }
    
    printf("Treasure %d:\n", index);
    printf("Treasure ID: %d\n", fortune->ID);
    printf("Username: %s\n", fortune->User_name);
    printf("GPS Coordinates: (%.2f, %.2f)\n", fortune->GPS_lat, fortune->GPS_long);
    printf("Value: %d\n", fortune->value);
    printf("Hint: %s\n", fortune->hint);
}

void list(char hunt[30])
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
  FILE *f; //treasure file
  if((f = fopen(filepath, "r")) == NULL)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }

  struct stat file_info;
  if (stat(filepath, &file_info) != 0)
    {
        perror("Error getting file information");
        fclose(f);
        closedir(folder);
        exit(-1);
    }
  
  printf("Hunt name: %s\n", hunt);
  printf("Size in bytes: %ld.\n", file_info.st_size);
  
  char str[32];
  strftime(str, sizeof(str), "%c", localtime(&file_info.st_mtime));
  printf("Last modification: %s.\n", str);
  printf("===============================================\n\n");
  printf("Treasures:\n\n");

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      fclose(f);
      closedir(folder);
      perror("Error creating a treasure: ");
      exit(-1);
    }

  int index = 1;
  while(fread(fortune, sizeof(TREASURE), 1 , f))
    {
      print_treasure(fortune, index);
      index ++;
    }
  
  fclose(f);
  closedir(folder);
}


void view(char hunt[30], int treasure_ID)
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
  FILE *f; //treasure file
  if((f = fopen(filepath, "r")) == NULL)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }
  
  printf("Hunt name: %s\n\n", hunt);

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure"); 
      fclose(f);
      closedir(folder);
      exit(-1);
    }

  int found = 0;
  while(fread(fortune, sizeof(TREASURE), 1 , f))
    {
      if(fortune->ID == treasure_ID)
	{
	  print_treasure(fortune, 0);
	  found = 1;
	  break;
	} 
    }

  if(found == 0)
    {
      printf("There aren't any treasures with the specified ID.\n");
    }
  
  fclose(f);
  closedir(folder);
}

void remove_hunt(char hunt[30])
{
  struct dirent *file;
  DIR *folder;

  folder = opendir(hunt);
  if(folder == NULL)
   {
     printf("Game doesn't exist.\n");
     exit(-1);
   }

  if((file = readdir(folder)) != NULL)
    {
      char filepath[50];
      sprintf(filepath, "%s/Game.b", hunt);
      
      if (remove(filepath) == 0)
	{
	  printf("Deleted file: %s\n", filepath);
	}
      else
	{
	  perror("Failed to delete file");
          closedir(folder);
          exit(-1);
        }
    }
  
  closedir(folder);
  
  if(rmdir(hunt) != 0)
    {
      perror("Error deleting the hunt");
      exit(-1);
    }
}

void remove_treasure(char hunt[30], int treasure_ID)
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
  FILE *f; //treasure file
  if((f = fopen(filepath, "r")) == NULL)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }
  
  printf("Hunt name: %s\n\n", hunt);

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure"); 
      fclose(f);
      closedir(folder);
      exit(-1);
    }

  int found = 0;
  FILE *new = NULL;
  char filepathnew[50];
  sprintf(filepathnew, "%s/New.b", hunt);
  
  if((new = fopen(filepathnew, "ab")) == NULL)
  {
    perror("Error opening the new file");
    exit(-1);
  }

  //iterare through all treasures, write all of them in the new file
  while(fread(fortune, sizeof(TREASURE), 1 , f))
    {
      if(fortune->ID == treasure_ID)
	{
	  print_treasure(fortune, 0);
	  found = 1;
	}
      else
	{
	  fwrite(fortune, sizeof(TREASURE), 1, new);
	}
    }

   //delete old file
   struct dirent *file;
   
   if((file = readdir(folder)) != NULL)
    {
      char filepath[50];
      sprintf(filepath, "%s/Game.b", hunt);
      
      if (remove(filepath) == 0)
	{
	  printf("Deleted file: %s\n", filepath);
	}
      else
	{
	  perror("Failed to delete file");
          closedir(folder);
          exit(-1);
        }
    }

   fclose(new);
   //rename new file
   rename(filepathnew, filepath);

  if(found == 0)
    {
      printf("There aren't any treasures with the specified ID.\n");
    }
  
  fclose(f);
  closedir(folder);
}
