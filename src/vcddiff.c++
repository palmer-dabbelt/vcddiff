
/*
 * Copyright (C) 2013 Palmer Dabbelt
 *   <palmer@dabbelt.com>
 *
 * This file is part of vcddiff.
 *
 * vcddiff is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vcddiff is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with vcddiff.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libvcd/vcd.h++>
#include <libvcd/sizet_format.h++>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "version.h"

/* This global signal determines if we should print the current cycle
 * number of not. */
bool has_printed_cycle;

/* This global signal determines if we should print out the file
 * mapping or not. */
bool any_failures;

/* The names of the input files. */
std::string a_filename, b_filename;

/* These two functions get called whenever there's a differing signal
 * or value between the two given files. */
static void sig_diff(const std::string signal,
                     const std::string a,
                     const std::string b,
                     size_t a_cycle,
                     size_t b_cycle)
    __attribute__((unused));

static void val_diff(const std::string signal) __attribute__((unused));

int main(int argc, const char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "--version") == 0)) {
        printf("vcddiff " VCDDIFF_VERSION "\n");
        exit(0);
    }

    if ((argc == 2 && (strcmp(argv[1], "--help") == 0)) || argc != 3) {
        printf("vcddiff <a.vcd> <b.vcd>: diffs two VCD files\n"
               "  vcddiff is like diff, but it understands some VCD\n"
               "  semantics.  As such, it can deal with differently\n"
               "  named and ordered signals.\n"
               "\n"
               "  --version: Print the version number and exit\n"
               "  --help:    Print this help text and exit\n"
            );
        exit(0);
    }

    a_filename = argv[1];
    b_filename = argv[2];

    /* Open the two files that we were given. */
    libvcd::vcd a(a_filename);
    libvcd::vcd b(b_filename);

    /* Read the given files all the way through. */
    any_failures = false;
    while (a.has_more_cycles() && b.has_more_cycles()) {
        /* Here's where we move to the next cycle. */
        a.step();
        b.step();

        /* Checks for differences between these two functions. */
        has_printed_cycle = false;
        if (libvcd::vcd::diff_this_cycle(a, b, &sig_diff) == false)
            any_failures = true;
    }

    if (a.has_more_cycles() || b.has_more_cycles()) {
        printf("mismatched cycle count\n");
        return 1;
    }

    if (any_failures == false)
        return 0;
    else if (any_failures == true)
        return 1;

    return 2;
}

void sig_diff(const std::string signal,
              const std::string a,
              const std::string b,
              size_t a_cycle,
              size_t b_cycle)
{
    if (any_failures == false) {
        any_failures = true;
        printf("--- %s\n", a_filename.c_str());
        printf("+++ %s\n", b_filename.c_str());
    }

    if (has_printed_cycle == false) {
        has_printed_cycle = true;
        printf("@" SIZET_FORMAT ", " SIZET_FORMAT "\n", a_cycle, b_cycle);
    }

    printf("-%s %s\n", signal.c_str(), a.c_str());
    printf("+%s %s\n", signal.c_str(), b.c_str());
}

void val_diff(const std::string signal)
{
    fprintf(stderr, "value:  '%s'\n", signal.c_str());
}
