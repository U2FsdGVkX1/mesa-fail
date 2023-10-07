/*
 * Copyright © 2021 Google, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _UTIL_PERFETTO_H
#define _UTIL_PERFETTO_H

#include "util/u_atomic.h"

#ifdef __cplusplus
extern "C" {
#endif

enum util_perfetto_category {
   UTIL_PERFETTO_CATEGORY_DEFAULT,
   UTIL_PERFETTO_CATEGORY_SLOW,

   UTIL_PERFETTO_CATEGORY_COUNT,
};

#ifdef HAVE_PERFETTO

extern int util_perfetto_category_states[UTIL_PERFETTO_CATEGORY_COUNT];

void util_perfetto_init(void);

static inline bool
util_perfetto_is_category_enabled(enum util_perfetto_category category)
{
   return p_atomic_read_relaxed(&util_perfetto_category_states[category]);
}

void util_perfetto_trace_begin(enum util_perfetto_category category,
                               const char *name);

void util_perfetto_trace_end(enum util_perfetto_category category);

#else /* HAVE_PERFETTO */

static inline void
util_perfetto_init(void)
{
}

static inline bool
util_perfetto_is_category_enabled(enum util_perfetto_category category)
{
   return false;
}

static inline void
util_perfetto_trace_begin(enum util_perfetto_category category,
                          const char *name)
{
}

static inline void
util_perfetto_trace_end(enum util_perfetto_category category)
{
}

#endif /* HAVE_PERFETTO */

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_PERFETTO_H */
