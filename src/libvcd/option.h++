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

#ifndef OPTION_HXX
#define OPTION_HXX

#include <stdlib.h>

namespace libvcd
{
    /* represents a value which may or may not exists */
    template <typename T>
    class option
    {
        private:
            bool _valid;
            T _value;

        public:
            /* create an empty option */
            option () : _valid(false) {}
            /* create a valid option with the given value */
            option (T value) : _valid(true), _value(value) {}
            /* copy constructor */
            option (const option<T> &opt) :
                _valid(opt._valid), _value(opt._value) {}
            /* true if this option actually holds a value,
             * false otherwise */
            bool valid(void) { return _valid; }
            /* the value contained by this option */
            T value(void)
            {
                if (!_valid) {
                    fprintf(stderr, "Trying to access invalid option value");
                    abort();
                }
                return _value;
            }
    };
}

#endif
