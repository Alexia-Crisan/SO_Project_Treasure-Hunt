#ifndef PHASE1_H
#define PHASE1_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct
{
  int ID;
  char User_name[30];
  float GPS_long, GPS_lat;
  int value;
  char hint[100];
}TREASURE;

void add(char hunt[30]);

void list(char hunt[30]);

void view(char hunt[30], int treasure_ID);

void remove_hunt(char hunt[30]);

#endif
