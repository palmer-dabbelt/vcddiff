
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

#include "vcd.h++"
#include "sizet_format.h++"
#include <stdlib.h>
#include <string.h>

using namespace libvcd;

#ifndef LINE_MAX
#define LINE_MAX 1024
#endif

/* Returns TRUE if haystack starts with needle. */
static bool str_start(std::string haystack, std::string needle);

/* Returns TRUE if the line consists entirely of white space. */
static bool all_white_p(const char *s);

/* Return the bit position that the signal is selecting.
 * If this is not a bit selection, return an invalid option */
static option<int> parse_bitsel(const char *sel) {
    int bit;

    // if the string is of the form [7:0]
    // this is not a bit selection
    if (strchr(sel, ':'))
        return option<int>();

    if (sscanf(sel, "[%d]", &bit) == 1)
        return option<int>(bit);

    return option<int>();
}

vcd::vcd(const std::string filename, int raise_signals)
    : _short_name(),
      _long_name(),
      _file(NULL),
      _has_more_cycles(false)
{
    _file = fopen(filename.c_str(), "r");
    if (_file == NULL) {
        fprintf(stderr, "Unable to open VCD file '%s'\n", filename.c_str());
        perror("fopen(..., r) failed");
        abort();
    }

    const longname *stack = NULL;
    int module_level = 0;

    char buffer[LINE_MAX];
    size_t line = 0;
    bool waiting_for_end = false;
    while (fgets(buffer, LINE_MAX, _file) != NULL) {
        line++;

        /* Check to see if we're inside some multi-line block that I
         * just don't bother with and should therefor ignore pretty
         * much everything. */
        if (waiting_for_end == true) {
            if (str_start(buffer, "$end"))
                waiting_for_end = false;
            continue;
        }

        bool needs_end = false;
        if (str_start(buffer, "$comment")) {
            continue;
        } else if (str_start(buffer, "$var")) {
            int w;
            char t[LINE_MAX], s[LINE_MAX],
                 l[LINE_MAX], b[LINE_MAX];

            if (sscanf(buffer, "$var %s %d %s %s %s $end",
                        t, &w, s, l, b) != 5) {
                fprintf(stderr, "Unable to parse variable from '%s'\n", buffer);
                abort();
            }

            if (strcmp(t, "reg") == 0) {
            } else if (strcmp(t, "wire") == 0) {
            } else {
                fprintf(stderr, "Unexpected token in '%s'\n", buffer);
                fprintf(stderr, "  Expected 'reg' or 'wire' as first token\n");
                abort();
            }

            if (stack == NULL) {
                fprintf(stderr, "Cannot add signal %s at level %d\n",
                        l, module_level);
                abort();
            }

            /* Adds this newly-discovered datum to both maps for later
             * use by step() and diff(). */
            std::string lname = stack->c_str(l);
            auto bit = parse_bitsel(b);
            auto a = new alias(lname, bit);
            this->_short_name.insert(std::make_pair(strdup(s), a));
            if (w != 1 || !bit.valid()) {
                /* if this is the whole signal and not just a single
                 * bit in it, create an empty datum */
                this->_long_name[lname] = new datum();
            } else if (this->_long_name.find(lname) == this->_long_name.end()) {
                /* If it is a single bit in a larger signal and this
                 * is the first bit found, pre-allocate the bit string,
                 * setting the bits to X. */
                std::string str(bit.value() + 2, 'X');
                str[0] = 'b';
                this->_long_name[lname] = new datum(str, false);
            }
        } else if (str_start(buffer, "$scope ")) {
            char module[LINE_MAX];

            if (sscanf(buffer, "$scope module %s $end\n", module) != 1) {
                fprintf(stderr, "Couldn't parse line '%s'\n", buffer);
                abort();
            }

            module_level++;
            if (module_level > raise_signals)
                stack = new longname(strdup(module), stack);
        } else if (str_start(buffer, "$upscope ")) {
            if (module_level <= 0) {
                fprintf(stderr, "Too many upscope at line " SIZET_FORMAT "\n",
                        line);
                abort();
            }

            if (stack != NULL)
                stack = stack->parent();
            module_level--;
        } else if (str_start(buffer, "#")) {
            if (sscanf(buffer, "#" SIZET_FORMAT, &this->_next_cycle) != 1) {
                fprintf(stderr, "Unable to parse cycle from '%s'\n", buffer);
                abort();
            }
            this->_has_more_cycles = true;
            this->_cycle = this->_next_cycle - 1;
            goto done;
        } else if (str_start(buffer, "$timescale")) {
            needs_end = true;
        } else if (str_start(buffer, "$enddefinitions")) {
            needs_end = true;
        } else if (str_start(buffer, "$dumpvars")) {
            needs_end = true;
        } else if (str_start(buffer, "$date")) {
            needs_end = true;
        } else if (str_start(buffer, "$version")) {
            needs_end = true;
        } else if (str_start(buffer, "$end")) {
            needs_end = false;
        } else if (all_white_p(buffer) == true) {
        } else {
            fprintf(stderr, "Unknown line in '%s'\n", filename.c_str());
            fprintf(stderr, "  line: '%s'", buffer);
            fprintf(stderr, "  line number: " SIZET_FORMAT "\n", line);
            abort();
        }

        if ((needs_end == true) && (strstr(buffer, "$end") == NULL))
            waiting_for_end = true;
    }

done:
    return;
}

vcd::~vcd(void)
{
    fclose(_file);
    _file = NULL;
}

void vcd::step(void)
{
    /* Make sure there's a valid cached next cycle -- if not that
     * means the parse ended and we can't do anything sane at all! */
    if (this->_has_more_cycles != true) {
        fprintf(stderr, "vcd::step(void) called with no next_cycle");
        fprintf(stderr, "check vcd::has_more_cycles(void) first!");
        abort();
    }

    /* Move to the next cycle, checking to see if the VCD file has
     * skipped a cycle.  If it has skipped a cycle then we simply
     * return here, as that just means nothing has changed at all. */
    this->_cycle++;
    if (this->_cycle != this->_next_cycle)
        return;
    this->_has_more_cycles = false;

    /* Read the input file until we find another cycle deliminator.  If
     * we don't find one then that means the file is over, which is OK
     * because we've just invalidated our cache. */
    char buffer[LINE_MAX];
    while (fgets(buffer, LINE_MAX, _file) != NULL) {
        /* We've found a cycle deliminator, which means we've finished
         * our step. */
        if (str_start(buffer, "#")) {
            this->_has_more_cycles = true;
            if (sscanf(buffer, "#" SIZET_FORMAT, &this->_next_cycle) != 1) {
                fprintf(stderr, "Unable to parse cycle from '%s'\n", buffer);
                abort();
            }
            return;
        }

        /* Check if this looks like a comment and if so skip the
         * line. */
        if (str_start(buffer, "$comment"))
            continue;

        /* Here we just silently ignore dumpvars/end pairs. */
        if (str_start(buffer, "$dumpvars"))
            continue;
        if (str_start(buffer, "$end"))
            continue;

        /* If there's no special case to deal with then just go ahead
         * and assume this is a name/value pair and parse it as
         * such. */
        char name[LINE_MAX];
        char value[LINE_MAX];

        /* Apparently there's a few formats of name/value pairs in VCD
         * files. */
        if (buffer[0] == 'b') {
            /* A 'b' indicates a space-seperated list. */
            if (sscanf(buffer, "%s %s", value, name) != 2) {
                fprintf(stderr, "Unable to parse name/value pair from '%s'\n",
                        buffer);
                abort();
            }
        } else if (buffer[0] == '1' || buffer[0] == '0'
                   || buffer[0] == 'x' || buffer[0] == 'z'
                   || buffer[0] == 'X' || buffer[0] == 'Z') {
            /* Some 1-bit signals just omit the space. */
            value[0] = 'b';
            value[2] = '\0';

            if (sscanf(buffer, "%c%s", &value[1], name) != 2) {
                fprintf(stderr, "Unable to parse name/value pair from '%s'\n",
                        buffer);
                abort();
            }
        }

        /* Ensure that there's a matching value for this short name. */
        auto entry = this->_short_name.find(name);
        if (entry == this->_short_name.end()) {
            fprintf(stderr, "Found unknown short name: '%s'\n", name);
            fprintf(stderr, "  line: '%s'\n", buffer);

            fprintf(stderr, "Short names found:\n");
            for (auto it = _short_name.begin(); it != _short_name.end(); it++) {
                fprintf(stderr, "%s\n", (*it).first.c_str());
            }

            abort();
        }

        /* Overwrite the current value with the updated value. */
        for (size_t i = 0; i < this->_short_name.count(name); ++i, ++entry) {
            auto alias = entry->second;
            auto datum = this->_long_name[alias->fullname()];
            if (!alias->bit().valid()) {
                *datum = value;
            } else if (alias->bit().value() < (int) datum->width()) {
                auto ind = datum->width() - alias->bit().value() - 1;
                datum->setbit(ind, value[1]);
            } else {
                fprintf(stderr, "Bit index %d outside width "
                            SIZET_FORMAT " for shortname %s\n",
                            alias->bit().value(), datum->width(), name);
                abort();
            }
        }
    }
}

bool vcd::diff_this_cycle(const vcd &a, const vcd &b,
                     void (*val)(const std::string s,
                                 const std::string a,
                                 const std::string b,
                                 size_t a_cycle,
                                 size_t b_cycle))
{
    bool out = true;

    for (auto it = a._long_name.begin(); it != a._long_name.end(); it++) {
        /* Lookup the cooresponding value in the other VCD -- this
         * won't fail because it's required to have already checked
         * that the signal map is the same. */
        auto lookup = b._long_name.find((*it).first);
        if (lookup == b._long_name.end()) {
            /* Any signal that doesn't have a value yet isn't even looked
             * at -- this prevents spurious signals (ones that are defined
             * in the VCD file but never assigned to) from breaking the
             * diff a whole lot. */
            if (!(*it).second->valid())
                continue;

            /* Allow mismatched signals when neither value has ever
             * changed.  Some implementations will optimize away
             * constants and not stick them in the VCD at all, this
             * deals with that fact. */
            if (!(*it).second->changed())
                continue;

            fprintf(stderr, "Mismatched signal '%s'\n", (*it).first.c_str());
            fprintf(stderr, " a_cycle: " SIZET_FORMAT "\n", a._cycle);
            fprintf(stderr, " b_cycle: " SIZET_FORMAT "\n", b._cycle);
            fprintf(stderr, " a_chg:   %d\n",  (*it).second->changed());
            abort();
        }

        auto va = (*it).second;
        auto vb = (*lookup).second;

        if (*va != *vb) {
            if (val != NULL)
                val((*it).first, va->text(), vb->text(), a._cycle, b._cycle);

            out = false;
        }
    }

    return out;
}

std::vector<std::string> vcd::all_long_names(void) const
{
    std::vector<std::string> out;

    for (const auto& pair : this->_long_name)
        out.push_back(pair.first);

    return out;
}

std::string vcd::long_name_to_bits(const std::string& long_name)
{
    auto l = _long_name.find(long_name);
    if (l == _long_name.end())
        return "";
    return l->second->text();
}

bool str_start(std::string h, std::string n)
{
    return (strncmp(h.c_str(), n.c_str(), strlen(n.c_str())) == 0);
}

bool all_white_p(const char *s)
{
    for (size_t i = 0; i < strlen(s); ++i)
        if (isspace(s[i]) == false)
            return false;

    return true;
}
