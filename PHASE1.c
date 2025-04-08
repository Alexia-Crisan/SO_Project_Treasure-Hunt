#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "PHASE1.h"

void log_action(char action[], int treasure_ID, char hunt[30])
{
  //open directory
  DIR *folder;

  folder = opendir(hunt);
  if(folder == NULL)
    {
      perror("Error opening the folder for log file");
    }

  //create log file
  char filepath[50];
  sprintf(filepath, "%s/logged_hunt", hunt);
  
  int l; //log file
  if((l = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777)) == -1)
  {
    perror("Error opening the log file");
    exit(-1);
  }

  struct stat file_info;
  if (stat(filepath, &file_info) != 0)
    {
        perror("Error getting file information");
        close(l);
        closedir(folder);
        exit(-1);
    }
  
  char str_log[100], str[30];
  strftime(str, sizeof(str), "%c", localtime(&file_info.st_mtime));
  sprintf(str_log, "Modification time: %s.\n", str);
  write(l, str_log, strlen(str_log));
   
  
  if(strcmp(action, "--add") == 0)
    {
      char str_action[40];
      sprintf(str_action, "Performed action: --add.\n");
      write(l, str_action, strlen(str_action));

      char message[100];
      sprintf(message, "Message: A treasure with the ID <%d> has been added to hunt <%s>.\n\n", treasure_ID, hunt);
      write(l, message, strlen(message));
    }

   if(strcmp(action, "--list") == 0)
    {
      char str_action[40];
      sprintf(str_action, "Performed action: --list.\n");
      write(l, str_action, strlen(str_action));

      char message[100];
      sprintf(message, "Message: The treasures from the hunt <%s> have been listed.\n\n", hunt);
      write(l, message, strlen(message));
    }

   if(strcmp(action, "--view") == 0)
    {
      char str_action[40];
      sprintf(str_action, "Performed action: --view.\n");
      write(l, str_action, strlen(str_action));

      char message[100];
      sprintf(message, "Message: A treasure with the ID <%d> from the hunt <%s> has been viewed.\n\n", treasure_ID, hunt);
      write(l, message, strlen(message));
    }

   if(strcmp(action, "--remove_treasure") == 0)
    {
      char str_action[40];
      sprintf(str_action, "Performed action: --remove_treasure.\n");
      write(l, str_action, strlen(str_action));

      char message[100];
      sprintf(message, "Message: A treasure with the ID <%d> from the hunt <%s> has been removed.\n\n", treasure_ID, hunt);
      write(l, message, strlen(message));
    }

   char filepath_symlink[50], name[50];
   sprintf(filepath_symlink, "./%s/logged_hunt", hunt);
   sprintf(name, "logged_hunt-<%s>", hunt);
   symlink(filepath_symlink, name);

   close(l);
   closedir(folder);
}

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
  int f; //treasure file
  if((f = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777)) == 0)
  {
    perror("Error opening the treasure file");
    exit(-2);
  }

  TREASURE *fortune = create_treasure();
      
  //add  trasure
  write(f, fortune, sizeof(TREASURE)); 
  
  close(f);
  closedir(folder);

  log_action("--add", fortune->ID, hunt);

  free(fortune);
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
  
  int f; //treasure file
  if((f = open(filepath, O_RDONLY)) == -1)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }

  struct stat file_info;
  if (stat(filepath, &file_info) != 0)
    {
        perror("Error getting file information");
        close(f);
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
      close(f);
      closedir(folder);
      perror("Error creating a treasure: ");
      exit(-1);
    }

  int index = 1;
  while(read(f, fortune, sizeof(TREASURE)))
    {
      print_treasure(fortune, index);
      index ++;
    }
  
  close(f);
  closedir(folder);
  free(fortune);

  log_action("--list", 0, hunt);
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
  
  int f; //treasure file
  if((f = open(filepath, O_RDONLY)) == -1)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }
  
  printf("Hunt name: %s\n\n", hunt);

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure"); 
      close(f);
      closedir(folder);
      exit(-1);
    }

  int found = 0;
  while(read(f, fortune, sizeof(TREASURE)))
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
  else
    {
      log_action("--view", fortune->ID, hunt);
    }
  
  close(f);
  closedir(folder);
  free(fortune);
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

  while((file = readdir(folder)) != NULL)
    {
      if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
	continue;
      
      char filepath[512];
      sprintf(filepath, "%s/%s", hunt, file->d_name);
      
      if (remove(filepath) == 0)
	{
	  printf("Deleted file: %s\n", filepath);
	}
      else
	{
	  perror("Failed to delete file");
        }
    }

  //remove symlink
  char filepath[50];
  sprintf(filepath, "./logged_hunt-<%s>", hunt);
  if (remove(filepath) == 0)
    {
      printf("Deleted file: %s\n", filepath);
    }
  else
    {
      perror("Failed to delete file");
    }
  
  closedir(folder);
  
  if(rmdir(hunt) != 0)
    {
      perror("Error deleting the hunt");
      exit(-1);
    }

  //log_action("--remove_hunt", hunt);
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
  
  int f; //treasure file
  if((f = open(filepath, O_RDONLY)) == -1)
  {
    perror("Error opening the treasure file");
    exit(-1);
  }
  
  printf("Hunt name: %s\n\n", hunt);

  TREASURE *fortune = NULL;
  if((fortune = malloc(sizeof(TREASURE))) == NULL)
    {
      perror("Error creating a treasure"); 
      close(f);
      closedir(folder);
      exit(-1);
    }

  int found = 0;
  int new;
  char filepathnew[50];
  sprintf(filepathnew, "%s/New.b", hunt);
  
  if((new = open(filepathnew, O_WRONLY | O_APPEND | O_CREAT, 0777)) == -1)
  {
    perror("Error opening the new file");
    exit(-1);
  }

  //iterare through all treasures, write all of them in the new file
  while(read(f, fortune, sizeof(TREASURE)))
    {
      if(fortune->ID == treasure_ID)
	{
	  print_treasure(fortune, 0);
	  found = 1;
	}
      else
	{
	  write(new, fortune, sizeof(TREASURE));
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

   close(new);
   //rename new file
   rename(filepathnew, filepath);

  if(found == 0)
    {
      printf("There aren't any treasures with the specified ID.\n");
    }
  else
    {
      log_action("--remove_treasure", fortune->ID, hunt);
    }
  
  close(f);
  closedir(folder);
  free(fortune);
}
