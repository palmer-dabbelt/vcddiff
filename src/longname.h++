
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

#include <string>

#ifndef LONGNAME_HXX
#define LONGNAME_HXX

/* Holds the long name of a signal. */
class longname
{
private:
    const std::string _name;
    const longname *_parent;

public:
    longname(const std::string name);
    longname(const std::string name, const longname *parent);

    friend bool operator==(const longname &a, const longname &b);

    const longname *parent(void) const { return _parent; }

    void print(FILE *f) const;

    const char *c_str(const char *end) const;
private:
    void c_str_help(char *pbuf) const;
};

#endif
