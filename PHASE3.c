#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "PHASE3.h"

void write_in_pipe(int pfd[2])
{
  close(pfd[0]); //close reading end			 
  dup2(pfd[1], STDOUT_FILENO);
}

void print_from_pipe(int pfd[2])
{
  FILE *stream;
  char buffer[2048] = {0};

  close(pfd[1]); // close writing end
  stream = fdopen(pfd[0], "r");
  if (!stream)
    {
      perror("Fdopen failed");
      exit(-1);
    }

  while (fgets(buffer, sizeof(buffer), stream))
    {
      printf("%s", buffer);
    }

  close(pfd[0]); //close used end
  fclose(stream);
}
