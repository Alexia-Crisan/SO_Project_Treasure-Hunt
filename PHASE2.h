#ifndef PHASE2_H
#define PHASE2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIG_LIST_HUNTS     SIGUSR1
#define SIG_LIST_TREASURES SIGUSR1
#define SIG_VIEW_TREASURE  SIGUSR1
#define SIG_STOP_MONITOR   SIGTERM 

extern int monitor_running;
extern int monitor_pid;

void monitor();

void start_monitor();

void list_hunts_wrapper();

void list_treasures();

void view_treasure();

void stop_monitor();

#endif
