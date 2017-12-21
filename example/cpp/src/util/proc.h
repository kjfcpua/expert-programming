/*
 * proc.h
 *
 *  Created on: Aug 15, 2017
 *      Author: hanchen
 */

#ifndef PROC_H_
#define PROC_H_

#if !(defined STACK_SIZE)
#define STACK_SIZE (4 * 1024 * 1024)
#endif

int daemonize(const char *dir, int noclose);
int set_core_dump();
int set_core_size(int size);
int set_stack_size(int size);
int get_cpu_num();
int bind_cpu(int cpu);
/*
 * get all pid expect current process
 * ppid can be NULL
**/
int get_pids_by_name(pid_t *pids, int max, const char *name);
/*
 * kill process with special signo
 * just send signal to process only once
 *
 * wait != 0, like bool true, wait until all process exit or not exist
 *
 * if signo is valid, will send signal to all process at first
 * even get some process has no auth
 *
 * if any one process no auth to kill but still wait all
 * process which has auth exist
 *
 * in one words, kill process can kill
 *
 * return:
 * 0, is kill all process
 * 1, kill but current process had been exist by g_terminated
 * -1, failed because signo is invalid or no auth to kill some process
**/
int kill_procs(pid_t *pids, int num, int signo, int wait);

WEAK_VAR_INIT(g_terminated, volatile int, 0);
void terminate(int signo);
int sig_ignore(int sig);
void RegSignal();
/*
 * e.g.
 *  sig_reg(terminate, SIGINT, SIGQUIT, SIGTERM, 0);
    sig_reg(SIG_IGN, SIGHUP, 0);
 *
 * terminate:
 * SIGINT:  <CTRL_C>
 * SIGQUIT: <CTRL_\>
 * SIGTERM: <kill>
 *
 * SIG_IGN:
 * SIGHUP
 */
int sig_reg_s(void (*handler)(int), int *sigs, int num);
int sig_reg(void (*handler)(int), ...);

/*
 * get shm num in the system 
 * the num include the shm even current process can't visit
 */
int get_shm_num(void);
/*
 * get shm key, if shm can't not exist or can't read will fail
 */
ssize_t get_shm_size(key_t key);
/*
 * get exist or create new shm
 * 1.try to get exist shm match with key, if size < 0, will get whole shm
 * 2.if flag has IPC_CREAT, try to create
 *
 * RETURN VALUE:
 * == 0, get exist shm succ
 * == 1, create new shm succ
 * <  0, fail, no auth to read exist or create new
 */
int get_shm(void **shm, key_t key, ssize_t size, int flag);

#endif /* PROC_H_ */
