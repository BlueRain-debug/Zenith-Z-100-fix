/* SPDX-License-Identifier: GPL-2.0-only */
#include <pthread.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

static atomic_int logLevel = ATOMIC_VAR_INIT(Z100_LOG_WARN);
static pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;
static const char* const levelNames[] = {"quiet", "error", "warn", "info", "debug", "trace"};

void z100_log_set_level(Z100LogLevel level)
{
	if(level >= Z100_LOG_QUIET && level <= Z100_LOG_TRACE)
		atomic_store_explicit(&logLevel, level, memory_order_relaxed);
}

Z100LogLevel z100_log_get_level(void)
{
	return atomic_load_explicit(&logLevel, memory_order_relaxed);
}

const char* z100_log_level_name(Z100LogLevel level)
{
	return level >= Z100_LOG_QUIET && level <= Z100_LOG_TRACE ? levelNames[level] : "unknown";
}

int z100_log_parse_level(const char* name, Z100LogLevel* level)
{
	for(int i = Z100_LOG_QUIET; i <= Z100_LOG_TRACE; i++) {
		if(!strcmp(name, levelNames[i])) {
			*level = (Z100LogLevel)i;
			return 1;
		}
	}
	return 0;
}

int z100_log_enabled(Z100LogLevel level)
{
	return level > Z100_LOG_QUIET && level <= z100_log_get_level();
}

void z100_log_message(Z100LogLevel level, const char* format, ...)
{
	if(!z100_log_enabled(level)) return;
	pthread_mutex_lock(&logMutex);
	fprintf(stderr, "[%s] ", z100_log_level_name(level));
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	size_t length = strlen(format);
	if(!length || format[length - 1] != '\n') fputc('\n', stderr);
	pthread_mutex_unlock(&logMutex);
}
