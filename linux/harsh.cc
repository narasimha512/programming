/*-------------------------------------------------------------------------- 
 *  Demonstrate various sorts of out-of-memory conditions
 *   1. running out of virtual memory due to malloc
 *    2. running out of virtual memory due to new
 *     3. running out of virtual memory due to too many/too large thread stacks
 *      4. overrunning thread stack
 *      --------------------------------------------------------------------------*/

#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#include <new>

int mymallocsize;
int mynewsize;
int mylocalsize;

#include <execinfo.h>

#define MAXSTACKDEPTH 2048

void handler(int signum, siginfo_t *si, void *)
{
	int i;
	static void *bt[MAXSTACKDEPTH];
	int bt_size;

	printf("Signal %d received\n", signum);
	printf("si_errno %d si_code %d si_pid %d\n", si->si_errno, si->si_code, si->si_pid);
	bt_size = backtrace(bt, sizeof(bt) / sizeof(void *));
	printf("Number of elements in backtrace: %d\n", bt_size);
	for (i = 0; i < bt_size; i++)
		printf("%p\n", bt[i]);
	fflush(stdout);
	kill(0, SIGTERM);
}


void use_local(int kb)
{
	char bar[1024];
	if (kb > 0)
		use_local(kb - 1);
	*bar = 0;
}

void use_new()
{
	char *bar = new char[mynewsize];
	/* touch every page */
	int npages = mynewsize / 4096;
	int i;
	for (i=0; i<npages; i++)
		bar[i << 12] = 0;
}

void use_malloc()
{
	char *bar = (char *)malloc(mymallocsize);
	if (bar == 0) {
		printf("malloc returned zero at line %d, function %s\n", __LINE__, __FUNCTION__);
		exit(1);
	}
	/* touch every page */
	int npages = mymallocsize / 4096;
	int i;
	for (i=0; i<npages; i++)
		bar[i << 12] = 0;
}

void bletch()
{
	use_local(mylocalsize);
	use_new();
	use_malloc();
}

void bar()
{
	bletch();
}

void foo()
{
	bar();
}

void *threadMain(void *arg)
{
	/* arrange for alternate signal stack so we can catch SIGSEGV when
 * 	 * user code overruns stack
 * 	 	 */
	stack_t sigstk;
	if ((sigstk.ss_sp = malloc(SIGSTKSZ)) == NULL) {
		perror("malloc");
		exit(1);
	}
	sigstk.ss_size = SIGSTKSZ;
	sigstk.ss_flags = 0;
	if (sigaltstack(&sigstk,(stack_t *)0) < 0) {
		perror("sigaltstack");
		exit(1);
	}
	struct sigaction SignalAction;

	SignalAction.sa_sigaction = handler;
	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO | SA_ONSTACK;
	sigaction(SIGSEGV, &SignalAction, NULL);
	sigaction(SIGABRT, &SignalAction, NULL);

	try {
		foo();
	} catch (const std::bad_alloc &e) {
		printf("caught bad_alloc\n");
		exit(1);
	} catch (const std::exception &e) {
		printf("caught generic exception\n");
		exit(1);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	int err;
	pthread_t thr;
	pthread_attr_t attrs;
	int nthreads;
	int stacksize;
	int vmsize;

	setlinebuf(stdout);

	if (argc != 7) {
		printf("usage: %s mallocsize newsize localsize nthreads stacksize vmsize\n", argv[0]);
		printf("All values in kilobytes\n");
		exit(1);
	}
	mymallocsize = atoi(argv[1]) * 1024;
	mynewsize = atoi(argv[2]) * 1024;
	mylocalsize = atoi(argv[3]);
	nthreads = atoi(argv[4]);
	stacksize = atoi(argv[5]) * 1024;
	vmsize = atoi(argv[6]) * 1024;

	printf("stress: mallocsize %d newsize %d localsize %d nthreads %d\n", mymallocsize/1024, mynewsize/1024, mylocalsize, nthreads);
	printf("limits: stacksize %d vmsize %d\n", stacksize/1024, vmsize/1024);
	printf("alternate signal stack consuming %d bytes per thread\n", SIGSTKSZ);

	struct rlimit limit;

	if (getrlimit(RLIMIT_AS, &limit)) {
		perror("getrlimit");
		exit(1);
	}
	limit.rlim_cur = vmsize;
	if (setrlimit(RLIMIT_AS, &limit)) {
		perror("getrlimit");
		exit(1);
	}

	/* arrange for kill(0) to not kill the caller! */
	setsid();

	/* arrange for alternate signal stack so we can catch SIGSEGV when
 * 	 * user code overruns stack
 * 	 	 */
	stack_t sigstk;
	if ((sigstk.ss_sp = malloc(SIGSTKSZ)) == NULL) {
		perror("malloc");
		exit(1);
	}
	sigstk.ss_size = SIGSTKSZ;
	sigstk.ss_flags = 0;
	if (sigaltstack(&sigstk,(stack_t *)0) < 0) {
		perror("sigaltstack");
		exit(1);
	}

	struct sigaction SignalAction;

	SignalAction.sa_sigaction = handler;
	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO | SA_ONSTACK;
	sigaction(SIGSEGV, &SignalAction, NULL);
	sigaction(SIGABRT, &SignalAction, NULL);
	sigaction(SIGILL, &SignalAction, NULL);
	sigaction(SIGBUS, &SignalAction, NULL);

	pthread_attr_init(&attrs);
	err = pthread_attr_setstacksize(&attrs, stacksize);
	if (err != 0) {
		printf("Setting stacksize to %d returns err %d\n", stacksize, err);
		exit(1);
	}

	for (i=0; i<nthreads; i++) {
		err = pthread_create(&thr, &attrs, threadMain, 0);
		if (err != 0) {
			printf("pthread_create returns err %d creating %d'th thread (counting from 1)\n", err, i+1);
			exit(1);
		}
	}
	sleep(2);
	exit(0);
}
