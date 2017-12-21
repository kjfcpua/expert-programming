/*
 * nullptr.c
 *
 *  Created on: Sep 30, 2017
 *      Author: hanchen
 */

#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int* pi = nullptr;

  std::cout << pi << std::endl;

  return 0;
}
