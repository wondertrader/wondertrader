#pragma once
#include <thread>
#ifdef __APPLE__
#include <mach/mach.h>
#endif

class CpuHelper
{
public:
	static uint32_t get_cpu_cores()
	{
		static uint32_t cores = std::thread::hardware_concurrency();
		return cores;
	}

#ifdef _WIN32
#include <thread>
	static bool bind_core(uint32_t i)
	{
		uint32_t cores = get_cpu_cores();
		if (i >= cores)
			return false;

		HANDLE hThread = GetCurrentThread();
		DWORD_PTR mask = SetThreadAffinityMask(hThread, (DWORD_PTR)(1 << i));
		return (mask != 0);
	}
#elif __APPLE__
#include <pthread.h>
#ifdef THREAD_AFFINITY_POLICY
	static bool bind_core(uint32_t i)
	{
 		int cores = get_cpu_cores();
		if (i >= cores)
			return false;

        thread_affinity_policy_data_t policy;
        policy.affinity_tag = i;
        kern_return_t ret;
        ret = thread_policy_set(pthread_mach_thread_np(pthread_self()), THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, THREAD_AFFINITY_POLICY_COUNT);
        return ret == KERN_SUCCESS;
    }
#endif
#else
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
	static bool bind_core(uint32_t i)
	{
		int cores = get_cpu_cores();
		if (i >= cores)
			return false;

		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(i, &mask);
		return (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) >= 0);
	}
#endif
};