/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of fiscevm
 *
 * fiscevm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * fiscevm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with fiscevm  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FY_CONFIG_H_
#define FY_CONFIG_H_

#ifndef MAX_GEN
# define MAX_GEN 8
#endif

#ifndef FY_GC_IDV
# define FY_GC_IDV 2148483647
#endif

#ifndef FY_GC_FORCE_IDV
# define FY_GC_FORCE_IDV 2148483647
#endif

#ifndef MAX_CLASSES
# define MAX_CLASSES 4096
#endif

#ifndef MAX_METHODS
# define MAX_METHODS 32768
#endif

#ifndef MAX_FIELDS
# define MAX_FIELDS 32768
#endif

#ifndef MAX_OBJECTS
# define MAX_OBJECTS 65536
#endif

#ifndef MAX_THREADS
# define MAX_THREADS 32
#endif

#ifndef MAX_STREAMS
# define MAX_STREAMS 16
#endif

#ifndef MAX_INSTRUCTIONS
# define MAX_INSTRUCTIONS 512
#endif

#ifndef STACK_SIZE
# define STACK_SIZE 16384
#endif

#ifndef FY_ENGINE_COUNT
# define FY_ENGINE_COUNT 2
#endif

#ifndef FY_NO_GOTO
# define FY_GOTO
#endif

#if defined(__GNUC__) && !defined(FY_STRICT_DECLARATION)
# define FY_SUPPORT_THREADING
#endif

#define FY_DISPATCH_SWITCH 0
#define FY_DISPATCH_REPLICA_SWITCH 1
#define FY_DISPATCH_THREAD 2

#ifndef FY_DISPATCH_MODE
# if defined(FY_SUPPORT_THREADING)
#  define FY_DISPATCH_MODE FY_DISPATCH_THREAD
# elif defined(FY_REPLICA_SWITCH)
#  define FY_DISPATCH_MODE FY_DISPATCH_REPLICA_SWITCH
# else
#  define FY_DISPATCH_MODE FY_DISPATCH_SWITCH
# endif
#endif

#ifndef REPL_MIN
# if FY_DISPATCH_MODE == FY_DISPATCH_THREAD || FY_DISPATCH_MODE == FY_DISPATCH_REPLICA_SWITCH
#  define REPL_MIN 5000
# else
#  define REPL_MIN 0
# endif
#endif

#if /*!defined(EMSCRIPTEN) &&*/ (defined(_C99) || defined(__GNUC__)) && !defined(FY_STRICT_DECLARATION)
# define FY_LATE_DECLARATION
#endif

#if 0
#define FY_GC_FORCE_FULL
#define FY_GC_DEBUG
#endif


#endif
