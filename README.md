# iOS_codeSamples


<p align="center">
<img src="https://img.shields.io/badge/Platform-iOS%2011.0+-lightgrey.svg" alt="Platform: iOS">
<a href="http://twitter.com/nholzschuch"><img src="https://img.shields.io/badge/Twitter-@nholzschuch-blue.svg?style=flat" alt="Twitter"/></a>
</p>

This repository is a companion for [LLVM for iOS](). It contains config files, sample code that compiles and runs on iOS, scripts, tips and tricks to make things work. 

## iOS "forbidden" functions 

some functions (`fork()`, `system()`...) are not available in iOS due to sandbox restrictions. Some are intercepted at compile time, others at run time. For the functions intercepted at run time, they are implemented as external functions in `lli.cpp`. If I forgot one, tell me and I'll add it.

For the functions intercepted at compile time, you'll have to edit the headers. For `system()`, that's in `~/usr/include/stdlib.h`, line 193. This line:
```c
__API_AVAILABLE(macos(10.0)) __IOS_PROHIBITED
```
is responsible for system exclusion. Replace it with:
```c
__API_AVAILABLE(macos(10.0), ios(11.0))
```

## Function replacement:

The LLVM interpreter (lli) calls external functions using libFFI. For some reason, external functions that operate on pointers (qsort, bsearch) cause the interpreter to crash. 

In the directory `libc_replacement`, we provide the source files for theses functions. If your code uses 

https://opensource.apple.com/source/xnu/xnu-344/bsd/kern/qsort.c
Repository of sample source code for LLVM on iOS / compiled locally
