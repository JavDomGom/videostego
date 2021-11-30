<p align="center"><img src="https://github.com/JavDomGom/videostego/blob/main/img/videostego_logo.png"></p>

## Status

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/JavDomGom/videostego)
[![Build Status](https://travis-ci.com/JavDomGom/videostego.svg?branch=main)](https://travis-ci.com/github/JavDomGom/videostego)
![Contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg)

## Basic overview

**Videostego** is a tool that you can use to write and read hidden messages in MP4 files using Steganography techniques like LSB.

## Dependencies

Check has the following dependencies:

- [GCC](https://gcc.gnu.org/install/) (required)
- [GNU Make](https://www.gnu.org/software/make/) (optional, can be compiled manually)
- [Check](https://github.com/libcheck/check) (optional, for testing)
- [Gcovr](https://github.com/gcovr/gcovr) (optional, to generate code coverage reports)
- [Valgrind](https://valgrind.org/docs/manual/quick-start.html) (optional, to detect many memory-related errors)

## Building or install on GNU/Linux

On a GNU/Linux system you can build and install **videostego** with the usual `sudo make install` procedure.

```bash
~$ sudo make install
```

This way you will have the binary available in `/usr/local/bin/videostego` to run it from any directory on your computer.

To uninstall:

```bash
~$ sudo make uninstall
```

If you just want to compile the code and get a binary in the current directory you must run `make build` without `sudo`.

```bash
~$ make build
```

The resulting `videostego` binary file can then be placed and run anywhere on your computer of your choice.

## How to run

```
~$ ./videostego -h
USAGE
        VideoStego [-f filename] [-w [-m message] | -r | -i] [-h] [-v]

DESCRIPTION

        Tool to apply steganography in video files.

OPTIONS

        -f, --file      Input video file.
        -m, --message   Message to hide. It has to be specified only if the mode is -w, --write.
        -w, --write     Write mode. Hide a secret message in video file.
        -r, --read      Read mode. Read a secret message in video file.
        -i, --info      Info mode. Print general structure and containers of video file.
        -v, --version   Print the program version.
        -h, --help      Print this help.

LICENSE

        License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
        This is free software: you are free to change and redistribute it.
        There is NO WARRANTY, to the extent permitted by law.
```

### To write

```bash
~$ ./videostego -f some_file.mp4 -w -m "Hello world!"
```

### To read

```bash
~$ ./videostego -f some_file.mp4 -r
```
```
"Hello world!"
```

### To print file architecture

```bash
~$ ./videostego -f some_file.mp4 -i
```
```
[ftyp] (0x00000000)      28 bytes: File type and compatibility.
[mdat] (0x0000001c)       8 bytes: Media data container.
[mdat] (0x00000024)  303739 bytes: Media data container.
[moov] (0x0004a29f)    6202 bytes: Container for all the meta-data.
│
├───[mvhd] (0x0004a2a7)     108 bytes: Movie header, overall declarations.
├───[drm ] (0x0004a313)      18 bytes: DRM container.
├───[trak] (0x0004a325)    2536 bytes: Container for an individual track or stream.
│   │
│   ├───[tkhd] (0x0004a32d)      92 bytes: Track header, overall information about the track.
│   ├───[edts] (0x0004a389)      36 bytes: Edit list container.
│   └───[mdia] (0x0004a3ad)    2400 bytes: Container for the media information in a track.
│       │
│       ├───[mdhd] (0x0004a3b5)      32 bytes: Media header, overall information about the media.
│       ├───[hdlr] (0x0004a3d5)      53 bytes: Handler, declares the media (handler) type.
│       └───[minf] (0x0004a40a)    2307 bytes: Media information container.
│           │
│           ├───[smhd] (0x0004a412)      16 bytes: Sound media header, overall information (sound track only).
│           ├───[dinf] (0x0004a422)      36 bytes: Data information box, container.
│           └───[stbl] (0x0004a446)    2247 bytes: Sample table box, container for the time/space map.
│               │
│               ├───[stsd] (0x0004a44e)     103 bytes: Sample descriptions (codec types, initialization etc.).
│               ├───[stts] (0x0004a4b5)      24 bytes: (Decoding) time-to-sample.
│               ├───[stsc] (0x0004a4cd)     100 bytes: Sample-to-chunk, partial data-offset information.
│               ├───[stsz] (0x0004a531)    1016 bytes: Sample sizes (framing).
│               └───[stco] (0x0004a929)     996 bytes: Chunk offset, partial data-offset information.
└───[trak] (0x0004ad0d)    3532 bytes: Container for an individual track or stream.
    │
    ├───[tkhd] (0x0004ad15)      92 bytes: Track header, overall information about the track.
    ├───[edts] (0x0004ad71)      36 bytes: Edit list container.
    └───[mdia] (0x0004ad95)    3396 bytes: Container for the media information in a track.
        │
        ├───[mdhd] (0x0004ad9d)      32 bytes: Media header, overall information about the media.
        ├───[hdlr] (0x0004adbd)      53 bytes: Handler, declares the media (handler) type.
        └───[minf] (0x0004adf2)    3303 bytes: Media information container.
            │
            ├───[vmhd] (0x0004adfa)      20 bytes: Video media header, overall information (video track only).
            ├───[dinf] (0x0004ae0e)      36 bytes: Data information box, container.
            └───[stbl] (0x0004ae32)    3239 bytes: Sample table box, container for the time/space map.
                │
                ├───[stsd] (0x0004ae3a)     171 bytes: Sample descriptions (codec types, initialization etc.).
                ├───[stts] (0x0004aee5)     152 bytes: (Decoding) time-to-sample.
                ├───[stss] (0x0004af7d)     112 bytes: Sync sample table (random access points).
                ├───[stsc] (0x0004afed)     736 bytes: Sample-to-chunk, partial data-offset information.
                ├───[stsz] (0x0004b2cd)    1788 bytes: Sample sizes (framing).
                └───[stco] (0x0004b9c9)     272 bytes: Chunk offset, partial data-offset information.
```

## Memory check

The [Valgrind](https://valgrind.org/docs/manual/quick-start.html) tool suite provides a number of debugging and profiling tools that help you make your programs faster and more correct. The most popular of these tools is called Memcheck. It can detect many memory-related errors that are common in C and C++ programs and that can lead to crashes and unpredictable behaviour. To install it:

```bash
~$ sudo apt-get update
~$ sudo apt-get install valgrind
```

To start memory check:

```bash
~$ make valgrind FILE=some_file.mp4 MSG="Hello"
```

If everything is ok the output is:

```
...
==3676794== All heap blocks were freed -- no leaks are possible
==3676794== 
==3676794== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Questions

If you have questions, please email inquiries to <JavDomGom@protonmail.com>.

If you don't understand the documentation, please tell us, so we can explain it
better. The general idea is: if you need to ask for help, then something needs
to be fixed so you (and others) don't need to ask for help. Asking questions
helps us to know what needs to be documented, described, and/or fixed.
