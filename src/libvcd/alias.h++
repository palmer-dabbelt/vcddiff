/*
 * Copyright (C) 2014 Howard Mao
 *   <zhehao.mao@gmail.com>
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

#include "longname.h++"
#include "option.h++"

#include <string>

#ifndef ALIAS_HXX
#define ALIAS_HXX

namespace libvcd
{
    /* Represents the target of a VCD shortname.
     * fullname is the longname of the VCD signal
     * bit is the bit index (little-endian order) in
     * that signal that the alias refers to */
    class alias
    {
        private:
            const std::string _fullname;
            const option<int> _bit;

        public:
            alias(const std::string &fullname, const option<int> bit)
                : _fullname(fullname), _bit(bit) {}
            /* The long name of the signal */
            std::string fullname(void) { return _fullname; }
            /* The individual bit in the multi-bit signal that
             * the short name refers to.
             * If the option is not valid, the alias refers to
             * the entire signal. */
            option<int> bit(void) { return _bit; }
    };
}

#endif
