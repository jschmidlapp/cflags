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

#include "cflags.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_FLAGS 10

static size_t cflags_num_flags = 0;
static const char *cflags_usage = NULL;

static cflag_info_t cflags[MAX_NUM_FLAGS] = {{0}};

void _add_cflag(const char *name, const char *description, void *valptr, cflag_type_t type)
{
	int i;

	if (cflags_num_flags == MAX_NUM_FLAGS) {
		fprintf(stderr, "Too many cflags specified\n");
		return;
	}

	i = cflags_num_flags++;

	cflags[i].name = name;
	cflags[i].value = (void *) valptr;
	cflags[i].description = description;
	cflags[i].type = type;
}

/* TODO: real gflags output */
void _print_usage(void)
{
	int i;
	int max_flag_name_len = 0;
	char tempstr[80];
	const char *namestr;

	if (cflags_usage)
		puts(cflags_usage);

	for (i=0; i<cflags_num_flags; i++) {

		size_t len = strlen(cflags[i].name);

		if (cflags[i].type == CFLAGS_BOOL)
			len = len*2 + 5; /* the name twice, plus '--' and '/no' */

		if (len > max_flag_name_len)
			max_flag_name_len = len;
	}

	/* Add some padding between name and description */
	max_flag_name_len += 5;

	for (i=0; i<cflags_num_flags; i++) {
		if (cflags[i].type == CFLAGS_BOOL) {
			snprintf(tempstr, sizeof(tempstr), "%s/--no%s", cflags[i].name, cflags[i].name);
			namestr = tempstr;
		}
		else
			namestr = cflags[i].name;

		printf("  --%-*s%s\n", max_flag_name_len, namestr, cflags[i].description);
	}

}

void SetUsageMessage(const char *usage)
{
	cflags_usage = usage;
}

static int _flag_match(cflag_info_t *flag, const char *name)
{
	if (strcmp(flag->name, name) == 0)
		return 1;

	if ((flag->type == CFLAGS_BOOL) &&
		(strncmp(name, "no", 2) == 0) &&
		(strlen(name) > 2) &&
		(strcmp(flag->name, name+2) == 0))
		return 1;

	return 0;
}

int _validate(cflag_info_t *cflag, const char *valstr)
{
	int ret;

	if (!cflags->validate)
		return 1;

	switch (cflag->type) {
	case CFLAGS_INT32:
		ret = cflags->validate(cflag->name, strtol(valstr, NULL, 0));
		break;
	case CFLAGS_UINT32:
		ret = cflags->validate(cflag->name, strtol(valstr, NULL, 0));
		break;
	case CFLAGS_INT64:
		ret = cflags->validate(cflag->name, strtol(valstr, NULL, 0));
		break;
	case CFLAGS_UINT64:
		ret = cflags->validate(cflag->name, strtol(valstr, NULL, 0));
		break;
	case CFLAGS_DOUBLE:
		ret = cflags->validate(cflag->name, strtod(valstr, NULL));
		break;
	case CFLAGS_STRING:
		ret = cflags->validate(cflag->name, valstr);
		break;
	case CFLAGS_BOOL:
		ret = 1;
		break;
	}

	return ret;
}

int ParseCommandLineFlags(int *argc, char **argv, int remove_flags)
{
	char *opt;
	int i, j;
	char *valstr, *name;
	int numargs = *argc;
	int first_non_flag = 1;

	if (!remove_flags) {
		fprintf(stderr, "remove_flags = 0 not yet supported\n");
		return -1;
	}

	for (i=1; i<numargs; i++) {
		opt = argv[i];
		if (*opt == '-') {
			if (*++opt == '-')
				opt++;
			(*argc)--;
			/* Split name from value */
			name = opt;
			if ((valstr = strchr(name, '='))) {
				name[valstr - opt] = '\0';
				valstr++;
			}

			if (strcmp(name, "help") == 0) {
				_print_usage();
				return 0;
			}
			for (j=0; j<cflags_num_flags; j++) {
				if (_flag_match(&cflags[j], name)) {

					// TODO: validate parameter

					switch (cflags[j].type) {
					case CFLAGS_INT32:
					case CFLAGS_UINT32:
					case CFLAGS_INT64:
					case CFLAGS_UINT64:
					case CFLAGS_DOUBLE:
					case CFLAGS_STRING:
						if (!valstr) {
							printf("Missing value for parameter %s\n", name);
							_print_usage();
							return -1;
						}
						break;
					case CFLAGS_BOOL:
						if (valstr) {
							fprintf(stderr, "Value specified for boolean flag %s\n", name);
							_print_usage();
							return -1;
						}
					}

					if (!_validate(&cflags[j], valstr)) {
						printf("Failed validation of new value %s for flag %s\n",
								valstr, cflags[j].name);
						return -1;
					}

					switch (cflags[j].type) {
					case CFLAGS_INT32:
					{
						*((int32_t*) cflags[j].value) = strtol(valstr, NULL, 0);
						break;
					}
					case CFLAGS_UINT32:
						*((uint32_t*) cflags[j].value) = strtol(valstr, NULL, 0);
						break;
					case CFLAGS_INT64:
						*((int64_t*) cflags[j].value) = strtol(valstr, NULL, 0);
						break;
					case CFLAGS_UINT64:
						*((uint64_t*) cflags[j].value) = strtol(valstr, NULL, 0);
						break;
					case CFLAGS_DOUBLE:
						*((double*) cflags[j].value) = strtod(valstr, NULL);
						break;
					case CFLAGS_STRING:
						*((char **) cflags[j].value) = (char *) malloc(strlen(valstr));
						strcpy(*((char **) cflags[j].value), valstr);
						break;
					case CFLAGS_BOOL:
						if (strncmp(name, "no", 2) == 0)
							*((int *) cflags[j].value) = 0;
						else
							*((int *) cflags[j].value) = 1;
						break;
					default:
						fprintf(stderr, "Unknown type for flag %s\n", name);
						break;
					}
				}
			}

			if (j != cflags_num_flags) {
				fprintf(stderr, "Unknown flag %s\n", opt);
				_print_usage();
				return -1;
			}
		}
		else {
			/* This isn't a flag, so move it to the front of argv. */
			argv[first_non_flag++] = opt;
		}
	}

	return 0;
}

int _add_cflag_validator(const char *name, int (*validate_fn)())
{
	int i;

	for (i=0; i<cflags_num_flags; i++) {
		if (strcmp(cflags[i].name,name) == 0) {
			cflags[i].validate = validate_fn;
		}
	}

	return 0;
}

int _add_cflag_validator_int32(const char *name, int (*validate_fn)(const char *, int32_t))
{
	return _add_cflag_validator(name, validate_fn);
}

int _add_cflag_validator_uint32(const char *name, int (*validate_fn)(const char *, uint32_t))
{
	return _add_cflag_validator(name, validate_fn);
}

int _add_cflag_validator_int64(const char *name, int (*validate_fn)(const char *, int64_t))
{
	return _add_cflag_validator(name, validate_fn);
}

int _add_cflag_validator_uint64(const char *name, int (*validate_fn)(const char *, uint64_t))
{
	return _add_cflag_validator(name, validate_fn);
}

int _add_cflag_validator_double(const char *name, int (*validate_fn)(const char *, double))
{
	return _add_cflag_validator(name, validate_fn);
}

int _add_cflag_validator_string(const char *name, int (*validate_fn)(const char *, const char *))
{
	return _add_cflag_validator(name, validate_fn);
}
