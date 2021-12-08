#pragma once
#include <signal.h>
#include "./StackTracer/StackTracer.h"
TracerLogCallback g_cbSignalLog = NULL;

void handle_signal(int signum)
{
	static char buf[64] = { 0 };
	memset(buf, 0, 64);
	switch (signum)
	{
#ifdef _WIN32
	case SIGINT:          // interrupt
	case SIGBREAK:        // Ctrl-Break sequence
		g_cbSignalLog("app interrupted");
		break;
	case SIGTERM:         // Software termination signal from kill
		g_cbSignalLog("app terminated");
		break;
	case SIGILL:          // illegal instruction - invalid function image
	case SIGFPE:          // floating point exception
	case SIGSEGV:         // segment violation
	case SIGABRT:         // abnormal termination triggered by abort call
	case SIGABRT_COMPAT:  // SIGABRT compatible with other platforms, same as SIGABRT
		sprintf(buf, "app stopped by signal %d", signum);
		g_cbSignalLog(buf);
		print_stack_trace(g_cbSignalLog);
		exit(signum);
		break;
#else
	case SIGURG:       // discard signal       urgent condition present on socket
	case SIGCONT:      // discard signal       continue after stop
	case SIGCHLD:      // discard signal       child status has changed
	case SIGIO:        // discard signal       I/O is possible on a descriptor (see fcntl(2))
	case SIGWINCH:     // discard signal       Window size change
		sprintf(buf, "app discard signal %d", signum);
		g_cbSignalLog(buf);
		break;
	case SIGSTOP:      // stop process         stop (cannot be caught or ignored)
	case SIGTSTP:      // stop process         stop signal generated from keyboard
	case SIGTTIN:      // stop process         background read attempted from control terminal
	case SIGTTOU:      // stop process         background write attempted to control terminal
		sprintf(buf, "app stopped by signal %d", signum);
		g_cbSignalLog(buf);
		exit(signum);
	case SIGINT:       // terminate process    interrupt program
		g_cbSignalLog("app interrupted");
		break;
	case SIGTERM:      // terminate process    software termination signal
		g_cbSignalLog("app terminated");
		break;
	case SIGKILL:      // terminate process    kill program
		g_cbSignalLog("app killed");
		exit(signum);
	case SIGHUP:       // terminate process    terminal line hangup
	case SIGPIPE:      // terminate process    write on a pipe with no reader
	case SIGALRM:      // terminate process    real-time timer expired
	case SIGXCPU:      // terminate process    cpu time limit exceeded (see setrlimit(2))
	case SIGXFSZ:      // terminate process    file size limit exceeded (see setrlimit(2))
	case SIGVTALRM:    // terminate process    virtual time alarm (see setitimer(2))
	case SIGPROF:      // terminate process    profiling timer alarm (see setitimer(2))
		sprintf(buf, "app terminated by signal %d", signum);
		g_cbSignalLog(buf);
		print_stack_trace(g_cbSignalLog);
		exit(signum);
	case SIGUSR1:      // terminate process    User defined signal 1
	case SIGUSR2:      // terminate process    User defined signal 2
		sprintf(buf, "app caught user defined signal %d", signum);
		g_cbSignalLog(buf);
		print_stack_trace(g_cbSignalLog);
		exit(signum);
	case SIGQUIT:      // create core image    quit program
	case SIGILL:       // create core image    illegal instruction
	case SIGTRAP:      // create core image    trace trap
	case SIGABRT:      // create core image    abort program (formerly SIGIOT)
	case SIGFPE:       // create core image    floating-point exception
	case SIGBUS:       // create core image    bus error
		g_cbSignalLog("bus error");
		print_stack_trace(g_cbSignalLog);
		exit(signum);
	case SIGSEGV:      // create core image    segmentation violation
		g_cbSignalLog("segmentation violation");
		print_stack_trace(g_cbSignalLog);
		exit(signum);
	case SIGSYS:       // create core image    non-existent system call invoked
		sprintf(buf, "app caught unexpected signal %d", signum);
		g_cbSignalLog(buf);
		print_stack_trace(g_cbSignalLog);
		exit(signum);
#endif // _WINDOWS
	default:
		sprintf(buf, "app caught unknown signal %d, signal ignored", signum);
		g_cbSignalLog(buf);
	}
}

void install_signal_hooks(TracerLogCallback cb)
{
	g_cbSignalLog = cb;
	for (int s = 1; s < NSIG; s++)
	{
		signal(s, handle_signal);
	}
}

