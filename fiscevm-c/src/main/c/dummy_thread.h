/**
 *  Copyright 2010-2013 Yuxuan Huang. All rights reserved.
 *
 * This file is part of libfisce.
 *
 * libfisce is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libfisce is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libfisce.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DUMMY_THREAD_H_
#define DUMMY_THREAD_H_

#ifndef HAVE_PTHREAD_H
typedef int pthread_t;
typedef int pthread_mutex_t;
#define pthread_self()			0
#define pthread_mutex_init(a, b)
#define pthread_mutex_lock(a)
#define pthread_mutex_unlock(a)
#define pthread_mutex_destroy(a)
#else
#include<pthread.h>
#endif

#endif /* DUMMY_THREAD_H_ */
