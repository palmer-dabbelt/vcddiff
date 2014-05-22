
/*
 * Copyright (C) 2013, 2014 Palmer Dabbelt
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

#ifndef VCD_HXX
#define VCD_HXX

#include "datum.h++"
#include "longname.h++"
#include <map>
#include <string>
#include <vector>
#include <stdio.h>

namespace libvcd
{
    class vcd
    {
    private:
        /* This maps a short name to a datum.  This map is used when
         * reading the values from the VCD file, as here the values
         * are tagged with the short name. */
        std::map<const std::string, datum *> _short_name;

        /* This maps the long name of of an entry to its current
         * value.  The idea here is that we need to be able to compare
         * data across two files, and the only way to do that is to
         * use the full name. */
        std::map<const std::string, datum *> _long_name;

        /* The file that we're reading from. */
        FILE *_file;

        /* The current cycle that we're parsing, initialized to 0.  */
        size_t _cycle;

        /* The next cycle that we want to parse.  This is an artifact
         * of how the parser works: it parses the current cycle until
         * it sees the next cycle and therefor has to remember it
         * here. */
        size_t _next_cycle;

        /* This is set to TRUE when there are more cycles remaining in
         * this file, and FALSE otherwise. */
        bool _has_more_cycles;

    public:
        /* Generates a new VCD file that reads from the given filename. */
        vcd(const std::string filename);

        ~vcd(void);

        /* Steps a single cycle from the VCD file. */
        void step(void);

        /* Steps until the cycle matches the given value.  Throws an
         * exception when the requested cycle doesn't exist. */
        void step_unit(size_t cycle);

        /* Returns TRUE when there are more cycles left to parse in
         * this file, and FALSE otherwise. */
        bool has_more_cycles(void) const { return _has_more_cycles; }

        /* Checks that two VCDs are equal for the current step.
         * There's two functions given here: one that gets called for
         * every value difference, and one that gets called for
         * every */
        static bool diff_this_cycle(const vcd &a, const vcd &b,
                                    void (*val)(const std::string s,
                                                const std::string a,
                                                const std::string b,
                                                size_t a_cycle,
                                                size_t b_cycle) = NULL);

        /* Returns TRUE if these two VCDs are on the same step, and FALSE
         * otherwise. */
        friend bool same_step(const vcd &a, const vcd &b);

        /* Returns a list of every signal in this VCD file. */
        std::vector<std::string> all_long_names(void) const;

        /* Looks up the data of a long name. */
        std::string long_name_to_bits(const std::string& long_name);
    };
}

#endif
