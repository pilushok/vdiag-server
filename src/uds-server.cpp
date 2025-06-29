// project headers
// #include "can.h"
#include "logger.h"

// cpp headers
#include <chrono>
#include <iostream>
#include <thread>

const std::string WELCOME_MESSAGE = "Virtual UDS Server v0.0.0\n"
                                    "Powered by Nikita Piliugin\n"
                                    "Simulating ISO 14229 over CAN (UDS)\n";

int main(int argc, char **argv)
{
  std::cout << WELCOME_MESSAGE << std::endl;
  std::cout << INFO << "Please enter your command...";
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
