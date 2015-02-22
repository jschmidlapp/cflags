/*
 * Copyright (c) 2015 Jason Schmidlapp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef CFLAGS_H_
#define CFLAGS_H_

#include <stdint.h>
#include <stddef.h>

int ParseCommandLineFlags(int *argc, char **argv, int remove_flags);
void SetUsageMessage(const char *usage);

#define DEFINE_bool(name, default_val, description)                      \
		DEFINE_flag(name, default_val, description, int, CFLAGS_BOOL);

#define DEFINE_int32(name, default_val, description)                     \
		DEFINE_flag(name, default_val, description, int32_t, CFLAGS_INT32);

#define DEFINE_uint32(name, default_val, description)                    \
		DEFINE_flag(name, default_val, description, uint32_t, CFLAGS_UINT32);

#define DEFINE_int64(name, default_val, description)                     \
		DEFINE_flag(name, default_val, description, int64_t, CFLAGS_INT64);

#define DEFINE_uint64(name, default_val, description)                    \
		DEFINE_flag(name, default_val, description, uint64_t, CFLAGS_UINT64);

#define DEFINE_double(name, default_val, description)                    \
		DEFINE_flag(name, default_val, description, double, CFLAGS_DOUBLE);

#define DEFINE_string(name, default_val, description)                    \
		DEFINE_flag(name, default_val, description, char *, CFLAGS_STRING);

#define DECLARE_int32(name)  extern uint32_t FLAGS_##name
#define DECLARE_uint32(name) extern uint32_t FLAGS##name
#define DECLARE_int64(name)  extern int64_t  FLAGS##name
#define DECLARE_uint64(name) extern uint64_t FLAGS##name
#define DECLARE_double(name) extern double   FLAGS##name
#define DECLARE_string(name) extern char *   FLAGS##name
#define DECLARE_bool(name)   extern int      FLAGS##name

#define DEFINE_validator(type, name, validator) \
	__attribute__((constructor(102))) void _cflag_validator_##name() { \
		_add_cflag_validator_##type(#name, validator); \
	};

/***** Internal *****/
typedef enum cflags_type
{
	CFLAGS_INT32,
	CFLAGS_UINT32,
	CFLAGS_INT64,
	CFLAGS_UINT64,
	CFLAGS_DOUBLE,
	CFLAGS_BOOL,
	CFLAGS_STRING
} cflag_type_t;

typedef struct cflag_info
{
	const char   *name;
	const char   *description;
	cflag_type_t  type;
	void         *value;
	int           (*validate)();
} cflag_info_t;

extern void _add_cflag(const char *name, const char *description, void *valptr, cflag_type_t type);

extern int _add_cflag_validator_int32(const char *name, int (*validate_fn)(const char *, int32_t));
extern int _add_cflag_validator_uint32(const char *name, int (*validate_fn)(const char *, uint32_t));
extern int _add_cflag_validator_int64(const char *name, int (*validate_fn)(const char *, int64_t));
extern int _add_cflag_validator_uint64(const char *name, int (*validate_fn)(const char *, uint64_t));
extern int _add_cflag_validator_double(const char *name, int (*validate_fn)(const char *, double));
extern int _add_cflag_validator_string(const char *name, int (*validate_fn)(const char *, const char *));

#define DEFINE_flag(name, default_val, description, type, typename)  \
	type FLAGS_##name = default_val;                                 \
	__attribute__((constructor(101))) void _add_cflag_##name() {          \
		_add_cflag(#name, description, &FLAGS_##name, typename);     \
	}

// TODO: from environment
// TODO: from file
// TODO: full help
// TODO: set version string
// TODO: version

#endif // CFLAGS_H_
