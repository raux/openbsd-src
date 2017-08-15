/*	$OpenBSD: mutex.h,v 1.4 2017/05/18 01:27:49 visa Exp $	*/

/*
 * Copyright (c) 2004 Artur Grabowski <art@openbsd.org>
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef _ARM_MUTEX_H_
#define _ARM_MUTEX_H_

#include <sys/_lock.h>

/*
 * Simple non-mp implementation.
 */
struct mutex {
	int mtx_lock;
	int mtx_wantipl;
	int mtx_oldipl;
#ifdef WITNESS
	struct lock_object mtx_lock_obj;
#endif
};

void _mtx_init(struct mutex *, int);

#ifdef WITNESS
#define MUTEX_INITIALIZER_FLAGS(ipl, name, flags) \
	{ 0, ipl, 0, MTX_LO_INITIALIZER(name, flags) }
#else
#define MUTEX_INITIALIZER_FLAGS(ipl, name, flags) \
	{ 0, ipl, 0 }
#endif

#ifdef DIAGNOSTIC
#define MUTEX_ASSERT_LOCKED(mtx) do {					\
	if ((mtx)->mtx_lock == 0)					\
		panic("mutex %p not held in %s", (mtx), __func__);	\
} while (0)

#define MUTEX_ASSERT_UNLOCKED(mtx) do {					\
	if ((mtx)->mtx_lock != 0)					\
		panic("mutex %p held in %s", (mtx), __func__);		\
} while (0)
#else
#define MUTEX_ASSERT_LOCKED(mtx) do { } while (0)
#define MUTEX_ASSERT_UNLOCKED(mtx) do { } while (0)
#endif

#define MUTEX_LOCK_OBJECT(mtx)	(&(mtx)->mtx_lock_obj)
#define MUTEX_OLDIPL(mtx)	(mtx)->mtx_oldipl

#endif
