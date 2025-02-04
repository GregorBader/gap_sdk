/* Copyright (C) 2005-2014 Free Software Foundation, Inc.
 C ontributed by Richard Henderson <r*th@redhat.com>.

 This file is part of the GNU OpenMP Library (libgomp).

 Libgomp is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 Under Section 7 of GPL version 3, you are granted additional
 permissions described in the GCC Runtime Library Exception, version
 3.1, as published by the Free Software Foundation.

 You should have received a copy of the GNU General Public License and
 a copy of the GCC Runtime Library Exception along with this program;
 see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 <http://www.gnu.org/licenses/>.  */

/* Copyright 2014 DEI - Universita' di Bologna
   author       DEI - Universita' di Bologna
                Alessandro Capotondi - alessandro.capotondi@unibo.it
   info         Public OpenMP API implementation */

#include "libgomp.h"

/************************* APIs *************************/
//FIXME useless stuff to fix!
void
omp_set_num_threads (int n)
{
    GOMP_WARN_NOT_SUPPORTED("omp_set_num_threads");
}

void
omp_set_dynamic (int val)
{
    GOMP_WARN_NOT_SUPPORTED("omp_set_dynamic");
}

int
omp_get_dynamic (void)
{
    GOMP_WARN_NOT_SUPPORTED("omp_get_dynamic");
    return 0;
}

void
omp_set_nested (int val)
{
    GOMP_WARN_NOT_SUPPORTED("omp_set_nested");
}

int
omp_get_nested (void)
{
    GOMP_WARN_NOT_SUPPORTED("omp_get_nested");
    return 0;
}
