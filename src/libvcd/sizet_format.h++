/*
 * Copyright (C) 2014 Palmer Dabbelt
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

#ifndef LIBVCD__SIZET_FORMAT_HXX
#define LIBVCD__SIZET_FORMAT_HXX

#ifndef SIZET_FORMAT
#if defined(__amd64__) && defined(__linux__)
#define SIZET_FORMAT "%lu"
#elif defined(__i386__) && defined(__linux__)
#define SIZET_FORMAT "%u"
#elif defined(__amd64__) && defined(__APPLE__)
#define SIZET_FORMAT "%lu"
#else
#error "define SIZET_FORMAT on this platform"
#endif
#endif

#endif
