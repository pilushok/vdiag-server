// project headers
// #include "can.h"
#include "can.h"
#include "logger.h"
#include <stdio.h>
#include <unistd.h>


#define WELCOME_MESSAGE  ("Virtual UDS Server v0.0.0\n\
                          Powered by Nikita Piliugin\n\
                          Simulating ISO 14229 over CAN (UDS)\n")

int main(int argc, char **argv)
{
  printf(WELCOME_MESSAGE);
  printf("Please enter command");

  while (1) {
    usleep(10000);
  }

  return 0;
}
