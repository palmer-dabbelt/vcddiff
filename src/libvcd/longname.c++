
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

#include "longname.h++"
#include <stdlib.h>
#include <string.h>

using namespace libvcd;

#ifndef LINE_MAX
#define LINE_MAX 1024
#endif

longname::longname(const std::string name)
    : _name(name),
      _parent(NULL)
{
}

longname::longname(const std::string name, const longname *parent)
    : _name(name),
      _parent(parent)
{
}

bool libvcd::operator==(const longname &a, const longname &b)
{
    fprintf(stderr, "Testing Equality: ");
    a.print(stderr);
    fprintf(stderr, " == ");
    b.print(stderr);
    fprintf(stderr, "\n");

    if (strcmp(a._name.c_str(), b._name.c_str()) != 0)
        return false;

    if (a._parent == b._parent)
        return true;

    if (a._parent == NULL || b._parent == NULL)
        return false;

    return *a._parent == *b._parent;
}

void longname::print(FILE *f) const
{
    if (this->_parent != NULL) {
        this->_parent->print(f);
        fprintf(f, "::%s", this->_name.c_str());
    } else {
        fprintf(f, "%s", this->_name.c_str());
    }
}

const char *longname::c_str(const char *ending) const
{
    char buffer[LINE_MAX];
    buffer[0] = '\0';
    c_str_help(buffer);

    char *out;
    if (asprintf(&out, "%s::%s", buffer, ending) == -1) {
        perror("asprintf falied");
        abort();
    }

    return out;
}

void longname::c_str_help(char *pbuf) const
{
    if (_parent == NULL) {
        snprintf(pbuf, LINE_MAX, "%s", this->_name.c_str());
        return;
    }

    char buffer[LINE_MAX];
    buffer[0] = '\0';
    _parent->c_str_help(buffer);
    snprintf(pbuf, LINE_MAX, "%s::%s", buffer, this->_name.c_str());
}
