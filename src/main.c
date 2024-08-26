#include <stdio.h>
#include <config.h>
#include "logger.h"

int main() {
  LOG_INFO("%s", PACKAGE_STRING);
  printf("Hello, World!\n");
  return 0;
}
