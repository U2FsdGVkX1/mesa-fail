# Copyright (c) Imagination Technologies Ltd.
#
# The contents of this file are subject to the MIT license as set out below.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

files_pvr = files(
  'mesa_context.c',
  'pvrcb.c',
  'pvrcompat.c',
  'pvrdri.c',
  'pvrext.c',
  'pvrutil.c',
)

dep_libpvr = [
  dep_libdrm,
]

if with_platform_tizen
  dep_libpvr += dep_dlog
endif

libpvr = static_library(
  'pvr',
  [files_pvr, main_dispatch_h],
  include_directories : [inc_include, inc_src, inc_mapi, inc_mesa,
                         inc_gallium, inc_dri_common, inc_util, inc_pvr],
  c_args : [no_override_init_args],
  gnu_symbol_visibility : 'hidden',
  dependencies : [dep_libpvr],
)
