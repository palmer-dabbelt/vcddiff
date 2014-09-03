
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

#include "datum.h++"
#include <stdlib.h>
#include <string.h>

using namespace libvcd;

datum::datum(void)
    : _valid(false),
      _changed(false),
      _text("X")
{
}

datum::datum(const std::string text, bool valid)
    : _valid(valid),
      _changed(false),
      _text(text)
{
}

datum& datum::operator=(const std::string new_text)
{
    this->_changed = this->_valid;
    this->_valid = true;
    this->_text = new_text;
    return *this;
}

size_t datum::width(void) const
{
    if (_text[0] == 'b')
        return _text.length() - 1;
    return 1;
}

void datum::setbit(int pos, char bit)
{
    this->_valid = true;
    this->_text[pos + 1] = bit;
}

bool libvcd::operator==(const datum &a, const datum &b)
{
    /* If one is valid and the other isn't then bail out. */
    if (a._valid != b._valid)
        return false;

    /* If both are invalid then we can't even look at the data. */
    if (!a._valid && !b._valid)
        return true;

    if (strcmp(a._text.c_str(), b._text.c_str()) == 0)
        return true;

    /* FIXME: Actually implement this check... */
    if (a._text.c_str()[0] != b._text.c_str()[0]) {
        fprintf(stderr, "datum comparison with different radixes\n");
        abort();
    }

    return false;
}
