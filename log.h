/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef Z100_LOG_H
#define Z100_LOG_H

typedef enum {
	Z100_LOG_QUIET,
	Z100_LOG_ERROR,
	Z100_LOG_WARN,
	Z100_LOG_INFO,
	Z100_LOG_DEBUG,
	Z100_LOG_TRACE
} Z100LogLevel;

void z100_log_set_level(Z100LogLevel level);
Z100LogLevel z100_log_get_level(void);
const char* z100_log_level_name(Z100LogLevel level);
int z100_log_parse_level(const char* name, Z100LogLevel* level);
int z100_log_enabled(Z100LogLevel level);
void z100_log_message(Z100LogLevel level, const char* format, ...)
#if defined(__GNUC__)
	__attribute__((format(printf, 2, 3)))
#endif
;

/* Avoid formatting or evaluating diagnostic arguments when disabled. */
#define Z100_LOG(level, ...) do { \
	if(z100_log_enabled(level)) z100_log_message(level, __VA_ARGS__); \
} while(0)
#define LOG_ERROR(...) Z100_LOG(Z100_LOG_ERROR, __VA_ARGS__)
#define LOG_WARN(...) Z100_LOG(Z100_LOG_WARN, __VA_ARGS__)
#define LOG_INFO(...) Z100_LOG(Z100_LOG_INFO, __VA_ARGS__)
#define LOG_DEBUG(...) Z100_LOG(Z100_LOG_DEBUG, __VA_ARGS__)
#define LOG_TRACE(...) Z100_LOG(Z100_LOG_TRACE, __VA_ARGS__)

#endif
