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

#include <stdio.h>
#include <limits.h>
#include "../cflags.h"

DEFINE_int32(intflag, -100, "Test integer flag");
DEFINE_uint32(uintflag, 101, "Test unsigned integer flag");
DEFINE_int64(int64flag, -4294967295 - 2000, "Test 64-bit integer flag");
DEFINE_uint64(uint64flag, 4294967295 + 2000, "Test unsigned 64-bit integer flag");
DEFINE_double(doubleflag, 1.0293, "Test double FP flag");
DEFINE_string(strflag, "CFLAGS", "Test string flag");
DEFINE_bool(boolflag,0, "Test boolean flag");

int validate(const char *flagname, int32_t val)
{
	if (val < 120)
		return 1;
	else
		return 0;
}

DEFINE_validator(int32, intflag, validate);

int main(int argc, char **argv)
{
	int result;

	SetUsageMessage("Usage: cflags ...");
	result = ParseCommandLineFlags(&argc, argv, 1);

	if (result < 0) {
		printf("Parsing failed\n");
		return 0;
	}

	printf("intflag %d\n", FLAGS_intflag);
	printf("uintflag %d\n", FLAGS_uintflag);
	printf("int64flag %ld\n", FLAGS_int64flag);
	printf("uint64flag %ld\n", FLAGS_uint64flag);
	printf("doubleflag %f\n", FLAGS_doubleflag);
	printf("strflag %s\n", FLAGS_strflag);
	printf("boolflag %d\n", FLAGS_boolflag);

	if (argc > 1) {
		int i;

		printf("Parameters %d\n", argc-1);

		for (i=1; i<argc; i++)
			printf("%d %s\n", i, argv[i]);
	}

	return 0;
}

