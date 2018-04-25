# iOS_codeSamples


<p align="center">
<img src="https://img.shields.io/badge/Platform-iOS%2011.0+-lightgrey.svg" alt="Platform: iOS">
<a href="http://twitter.com/nholzschuch"><img src="https://img.shields.io/badge/Twitter-@nholzschuch-blue.svg?style=flat" alt="Twitter"/></a>
</p>

This repository is a companion for [LLVM for iOS](). It contains config files, sample code that compiles and runs on iOS, scripts, tips and tricks to make things work. 


## Function replacement:

The LLVM interpreter (lli) calls external functions using libFFI. For some reason, external functions that operate on pointers (qsort, bsearch) cause the interpreter to crash. 

In the directory `libc_replacement`, we provide the source files for theses functions. If your code uses 

https://opensource.apple.com/source/xnu/xnu-344/bsd/kern/qsort.c
Repository of sample source code for LLVM on iOS / compiled locally
