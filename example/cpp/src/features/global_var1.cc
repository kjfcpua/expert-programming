/*
 * global_var_init.cc
 *
 *  Created on: 2017年2月8日
 *      Author: hanchen
 */
#include <stdio.h>

int init_global_num() {
	printf("init_global_num\n");
	return 11;
}

/**
 * only run in cc progarm, run when load program,
 * before main function start
 *
 * compile error: "initializer element is not a compile-time constant"
 * in c program
 */
int global_num = init_global_num();

int main(int argc, char *argv[]) {
	printf("main\n");
	printf("global_num: %d\n", global_num);
	return 0;
}

