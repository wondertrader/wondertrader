/*!
 * \file StackTracer.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2021/04/13
 * 
 * \brief 堆栈查看器
 */
#pragma once

#ifdef _WIN32
#include <cstdio>
#include <Windows.h>
#include <Psapi.h>
#else
#include <stdio.h>
#endif // _WINDOWS

#include <functional>

typedef std::function<void(const char*)> TracerLogCallback;
typedef std::function<void(int)> ExitHandler;

void print_stack_trace(TracerLogCallback cb);

