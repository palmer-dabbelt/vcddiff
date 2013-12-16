
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

#ifndef DATUM_HXX
#define DATUM_HXX

#include "longname.h++"

/* Holds the value of a particular node at a particular time, along
 * with the full name of that node. */
class datum
{
private:
    /* This is TRUE when there is valid data for this datum, and FALSE
     * otherwise. */
    bool _valid;

    /* The text string that shows up in the VCD file. */
    std::string _text;

public:
    /* Initializes a new (and invalid) datum. */
    datum(void);

    /* Builds a new datum, given some text from a VCD file that
     * represents the value. */
    datum(const std::string text);

    /* Accessor functions. */
    const std::string text(void) const { return this->_text; }

    /* Overwrites the current value with a new one. */
    datum& operator=(const std::string new_text);

    /* Comparison operator on a single datum -- this doesn't just do a
     * plain string comparison, it checks for radix, too!  The second
     * operator is a helper function, which does the comparison in one
     * direction (we have to do it both ways). */
    friend bool operator==(const datum &a, const datum &b);
    friend bool operator!=(const datum &a, const datum &b) { return !(a == b); }

    friend bool help_equal(const datum &a, const datum &b);

    /* Returns TRUE whenever this is valid, in other words it has been
     * set to a value. */
    bool valid(void) const { return _valid; }
};

#endif
