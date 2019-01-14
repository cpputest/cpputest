/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef D_Shuffle_h
#define D_Shuffle_h

#include "StandardCLibrary.h"


#define SHUFFLE_DISABLED 0
#define SHUFFLE_ENABLED_RANDOM_SEED 1
#define SHUFFLE_SEED_MINIMUM_VALUE 2

typedef int (*rand_func_t)(void);

// wrapper needed for translating calling convention on Watcom/DOS
static inline int rand_(void) // rand_func_t
{
    return rand();
}

// "Durstenfeld shuffle" according to Wikipedia
static inline void shuffle_list(rand_func_t rand_func, int numElems, void* listToShuffleInPlace[])
{
    for (int i = numElems - 1; i >= 1; --i)
    {
        int j = rand_func() % (i + 1); // distribution biased by modulo, but good enough for shuffling
        void* e1 = listToShuffleInPlace[j];
        void* e2 = listToShuffleInPlace[i];
        listToShuffleInPlace[i] = e1;
        listToShuffleInPlace[j] = e2;
    }
}

#endif
