# iOS_codeSamples


<p align="center">
<img src="https://img.shields.io/badge/Platform-iOS%2011.0+-lightgrey.svg" alt="Platform: iOS">
<a href="http://twitter.com/nholzschuch"><img src="https://img.shields.io/badge/Twitter-@nholzschuch-blue.svg?style=flat" alt="Twitter"/></a>
</p>

This repository is a companion for [LLVM for iOS](https://github.com/holzschu/llvm). It contains config files, sample code that compiles and runs on iOS, scripts, tips and tricks to make things work. 

## Compilation 

To compile a project:
- for each source file, `clang --config ~/clang.cfg source.c`. This will produce `source.ll`
- assemble the compiled sources with: `llvm-link -only-needed -o=executable.bc *.ll` This will produce the linked file.
- to start the program, use `lli executable.bc <flags to the main program>` 

## Configuration files

There are 2 configuration files in this repository: `clang.cfg` and `gnu.cfg`. The former is for standard projects:
```
 -S -emit-llvm -I ~/lib/clang/7.0.0/include -I ~/usr/include -Wno-nullability-completeness  -D_FORTIFY_SOURCE=0
```
The latter is for complex projects, using `configure`:
```
 -S -emit-llvm -I ../lib -I .. -I ../src -I ~/usr/include -I ~/lib/clang/7.0.0/include -Wno-nullability-completeness -DHAVE_CONFIG_H -D_FORTIFY_SOURCE=0 
```
The big difference is the `-DHAVE_CONFIG_H` which tells the compiler to look for a `config.h` file, which you have to create: `cp config.hin config.h` and edit the `config.h` file.

## Sample code

The directory `sampleCode` contains some simple projects you can compile:
- `hello.c` just experiments with input and output methods.
- `pwd.c` is the BSD implementation of the shell command `pwd`. It prints the current working directory.
- `bsd_find` contains the Free BSD implementation of the shell command `find`, with minor edits (see `fork`, below), 
- `bsd_diff` contains the Free BSD implementation of the shell command `diff`. It is a good command to test the difference between JIT and interpreter.

Additionally, [tree](https://github.com/nodakai/tree-command.git) works with no modifications, except you have to link it with `libc_replacement/qsort.c` (see below, "Function replacement"). 

## iOS "forbidden" functions 

Some functions (`fork()`, `system()`...) are not available in iOS due to sandbox restrictions. Some are intercepted at compile time, others at run time. For the functions intercepted at run time, they are implemented as external functions in `lli.cpp`. If I forgot one, tell me and I'll add it.

For the functions intercepted at compile time, you'll have to edit the headers. For `system()`, that's in: `~/usr/include/stdlib.h`, line 193. This line:
```c
__API_AVAILABLE(macos(10.0)) __IOS_PROHIBITED
```
is responsible for `system()` exclusion. Replace it with:
```c
__API_AVAILABLE(macos(10.0), ios(11.0))
```

`fork()` requires special attention. `lli` catches it so it doesn't stop the program, but it can't actually fork. And `exec()` doesn't exit. `fork()` returns 0, so the interpreter will always go through the child branch. You'll have to edit the code to make sure it doesn't throw an exception when `exec()` returns and goes through the parent branch. For example, here is the original code:
```c
	switch (pid = fork()) {
	case 0: {
		execvp(plan->e_argv[0], plan->e_argv);
		warn("%s", plan->e_argv[0]);
		_exit(1); 
	}
	}
	pid = waitpid(pid, &status, 0);
	if (pid != -1 && WIFEXITED(status) && !WEXITSTATUS(status))
		return (1);
	return (0);
```
And here is the modified version:
```c
	switch (pid = fork()) {
	case 0: {
		execvp(plan->e_argv[0], plan->e_argv);
		/* warn("%s", plan->e_argv[0]);
		_exit(1); */
	}
	}
	pid = waitpid(pid, &status, 0);
	if (pid != -1 && WIFEXITED(status) && !WEXITSTATUS(status))
		return (1);
	return (0);
```

## Function replacement

The LLVM interpreter (lli) calls external functions using libFFI. For some reason, external functions that operate on pointers (`qsort`, `bsearch`) cause the interpreter to crash. 

The directory `libc_replacement` contains source files for these functions. If your code uses one of these functions, compile the relevant source file and link with it. For now, there is `qsort` and `bsearch`. It will expand as more problematic functions are discovered.

