#ifndef PHASE3_H
#define PHASE3_H

extern int pfd[2];

void write_in_pipe(int pfd[2]);

void print_from_pipe(int pfd[2]);

#endif
