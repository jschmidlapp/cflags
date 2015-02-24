# cflags

An implementation of the Google flags (gflags) command line library in C.  See https://code.google.com/p/gflags/ for the original C++ version.

## Build and Installation

Compile and link cflags.c with your application, or build into a seperate library.  gcc is required to build.

## Usage

See the original gflags documentaiton for general usage.  Some of the APIs differ slightly (such as DEFINE_validator) due to differences between C and C++ but overall it should be very similar to the C++ API. 

## Status

Basic functionality is implemented and tested.

### Supported

* Defining and declaring flags: int32, uint32, int64, uint64, double, string, bool (int)
* Flag validation
* Usage and help message

## Not yet implemented

* Reading flags from environment
* Reading flags from file
* Full help (including gflag specific flags)
* Version reporting
* Command line completion
