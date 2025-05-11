#ifndef PHASE3_H
#define PHASE3_H

extern int pfd1[2];
extern int pfd2[2];
extern int pfd3[2];

int calculate_score(char Username[30]);

void write_in_pipe(int pfd[2]);

void print_from_pipe(int pfd[2]);

#endif
