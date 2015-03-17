Fast Interpreter SCript Engine <small>(former Feather-In SCript Engine)</small>
====
This is a virtual machine which can run java's bytecode and has a (maybe) minimal subset of java runtime, which implemented in pure C.<br />
用C语言实现的解释型Java虚拟机

Originally used as a cross-platform AVG script interpreter written in Java, but translated to C for portability(Yes! C is much more portable than Java.) and optimized faster and faster ;)<br />
最初是是Java写的跨平台AVG用脚本引擎，完成后作者对跨平台脚本引擎的兴趣大增导致用C重写了一遍，从此走向了优化的不归路。

Highlights
----
  * Portability: implemented in Pure C, most code complies ANSI-C90, with following exception which is :

    1 Native handler for System.currentTimeMillis() / System.nanoTime() uses gettimeofday in POSIX systems and QueryPerformanceCounter in windows.

    2 Need int64 type, use long long in GCC and __int64 in MSVC, you can also specific a custom int64 time for your platform.

    3 Threading-based dispatching scheme requires GCC/clang. Switch-based dispatching scheme is used in other compilers.

  * Emulated multi-thread: Multi thread is not required on host OS.

  * Save/Load state: Can dump VM's state at almost anytime(when vm is not busy running), and resume later

  * Pure C code: Not familiar with ASM or C++? All VM code here are in pure C, some code in forth(vmgen), but compiled to C and these C code are in src. (Of course runtime code are in Java, as this is somehow a "java" VM)
  
  * Performance: Linpack performance is almost as fast as java's interpreter mode in threading dispatch mode.

  * Can run under java/android with JNI interface (Run java under java, pretty crazy huh?)

  * No need to worry about license: The code of VM(fiscevm-c fiscevm-c-native fiscevm-ios) is licensed under the terms of the LGPL and the code of Runtime(fiscevm-runtime) is licensed under Apache. I choose these licenses on purpose of

    1 If you optimized fiscevm itself and release, please open source so the VM could become better and better.

    2 Anything you code that runs INSIDE fiscevm need not to be open-sourced.

Supports
----
  * java.lang.*
  * java.util.*
  * java.math.*
  * java.beans.*
  * JNI
  * Reflection
  * Reference
  * Proxy
  * Regex
  * Read file through Class.getResourceAsInputStream
  * With native-invoke interface, you can always add more function into it

  **WITHOUT**

  * packages not listed above
  * All security manager related function is mocked
  * Multi/custom classloader
  * Serialize (related classes are added for compatibility, but all these code does nothing ;)
  * java.util.concurrent.*
  * Runtime related functions
  * Locales / Charsets (Supports only UTF-8)
  * Network support

Quick start
----
<h3>Prerequisite</h3>
CMake 3.0+ (optional)

<h3>Build runtime package</h3>

...WIP...