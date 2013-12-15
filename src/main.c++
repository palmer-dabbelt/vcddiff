
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

#include "vcd.h++"
#include <stdlib.h>
#include <stdio.h>

/* This global signal determines if we should print the current cycle
 * number of not. */
bool has_printed_cycle;

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
    if (argc != 3) {
        fprintf(stderr, "vcddiff <a.vcd> <b.vcd>: diffs two VCD files\n");
        exit(0);
    }

    /* Open the two files that we were given. */
    vcd a(argv[1]);
    vcd b(argv[2]);

    /* Read the given files all the way through. */
    bool any_failures = false;
    while (a.has_more_cycles() && b.has_more_cycles()) {
        /* Here's where we move to the next cycle. */
        a.step();
        b.step();

        /* Checks for differences between these two functions. */
        has_printed_cycle = false;
        if (diff_this_cycle(a, b, &sig_diff) == false)
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
    if (has_printed_cycle == false) {
        has_printed_cycle = true;
        printf("@%lu, %lu\n", a_cycle, b_cycle);
    }

    printf("-%s %s\n", signal.c_str(), a.c_str());
    printf("+%s %s\n", signal.c_str(), b.c_str());
}

void val_diff(const std::string signal)
{
    fprintf(stderr, "value:  '%s'\n", signal.c_str());
}
