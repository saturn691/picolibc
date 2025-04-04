/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2025 Keith Packard
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <limits.h>

#if __SIZEOF_WCHAR_T__ == 2
#define LAST_CHAR 0xffff
#else
#define LAST_CHAR 0xe01ef
#endif

static const char *locales[] = {
    "C",
#ifdef HAVE_UTF_CHARSETS
    "C.UTF-8",
#endif
};

#define NUM_LOCALE sizeof(locales)/sizeof(locales[0])

int main(int argc, char **argv)
{
    wint_t      c;
    unsigned    i;
    int         error = 0;
    FILE        *out = stdout;

    if (argc > 1) {
        out = fopen(argv[1], "w");
        if (!out) {
            perror(argv[1]);
            exit(1);
        }
    }

    for (i = 0; i < NUM_LOCALE; i++) {
        if (setlocale(LC_ALL, locales[i]) == NULL) {
            printf("%s: setlocale failed\n", locales[i]);
            error = 1;
            continue;
        }
        for (c = 0x0000; ; c++) {
            wint_t  u, l;

            u = towupper(c);
            l = towlower(c);
            if (c != u || c != l)
                fprintf (out, "%s: %#x %#x %#x\n", locales[i], c, towupper(c), towlower(c));
            if (c == LAST_CHAR)
                break;
        }
    }
    fflush(out);
    return error;
}
