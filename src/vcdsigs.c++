
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "version.h"

int main(int argc, const char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "--version") == 0)) {
        printf("vcdsigs " VCDDIFF_VERSION "\n");
        exit(0);
    }

    if ((argc == 2 && (strcmp(argv[1], "--help") == 0)) || argc != 2) {
        printf("vcdsigs <VCD.vcd> <b.vcd>: lists the signals in a VCD file\n"
               "  --version: Print the version number and exit\n"
               "  --help:    Print this help text and exit\n"
            );
        exit(0);
    }

    /* Open the two files that we were given. */
    libvcd::vcd vcd(argv[1]);

    for (const auto& name : vcd.all_long_names())
        printf("%s\n", name.c_str());

    return 0;
}
