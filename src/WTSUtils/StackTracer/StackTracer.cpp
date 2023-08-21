
#include "StackTracer.h"
#include "cstdlib"

#ifdef _WIN32
#	ifdef _MSC_VER
#include "StackWalker.h"
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")
void print_stack_trace(TracerLogCallback cb)
{
	cb("Uncaught exception");
	StackWalker sw(cb);
	sw.ShowCallstack();
}
#	else //_GCC
void print_stack_trace(TracerLogCallback cb) {
	cb("Cannot print stack trace due to being build on windows with GCC");
}
#	endif
#else
#include <cerrno>
#include <execinfo.h>
#include <cxxabi.h>
void print_stack_trace(TracerLogCallback cb) {
	unsigned int max_frames = 127;
	// storage array for stack trace address data
	void *addrlist[max_frames + 1];

	// retrieve current stack addresses
	unsigned int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void *));

	if (addrlen == 0) {
		cb("no trace fetched");
		return;
	}

	// resolve addresses into strings containing "filename(function+address)",
	// Actually it will be ## program address function + offset
	// this array must be free()-ed
	char **symbollist = backtrace_symbols(addrlist, addrlen);

	// iterate over the returned symbol lines. skip the first, it is the
	// address of this function.
	for (unsigned int i = 4; i < addrlen; i++) {
		char *begin_name = nullptr;
		char *begin_offset = nullptr;
		char *end_offset = nullptr;

		// ./module(function+0x15c) [0x8048a6d]
		for (char *p = symbollist[i]; *p; ++p) {
			if (*p == '(')
				begin_name = p;
			else if (*p == '+')
				begin_offset = p;
			else if (*p == ')' && (begin_offset || begin_name))
				end_offset = p;
		}

		if (begin_name && end_offset && (begin_name > end_offset)) {
			*begin_name++ = '\0';
			*end_offset++ = '\0';
			if (begin_offset)
				*begin_offset++ = '\0';

			// mangled name is now in [begin_name, begin_offset) and caller
			// offset in [begin_offset, end_offset). now apply
			// __cxa_demangle():

			int status = 0;
			size_t funcnamesize = 8192;
			char funcname[8192];
			char *ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
			cb(ret);
			char buf[256] = { 0 };
			sprintf(buf, "%30s ( %40s  + %6s) %s", symbollist[i], status == 0 ? ret : begin_name, begin_offset ? begin_offset : "", end_offset);
			cb(buf);
		} else {
			// couldn't parse the line? print the whole line.
			cb(symbollist[i]);
		}
	}
	free(symbollist);
}
#endif // _WINDOWS
