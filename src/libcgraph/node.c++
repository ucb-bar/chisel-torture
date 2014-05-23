/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of chisel-torture.
 *
 * chisel-torture is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * chisel-torture is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with flo-llvm.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "node.h++"
#include <sstream>
using namespace libcgraph;

static std::string unique_name(void);

node::node(void)
    : libflo::node(unique_name()),
      _changed_cycle(-1),
      _value()
{
}

node::node(size_t value)
    : libflo::node(std::to_string(value),
                   libflo::unknown<size_t>(),
                   libflo::unknown<size_t>(),
                   false,
                   true,
                   libflo::unknown<size_t>(),
                   libflo::unknown<std::string>()
        )
{
}


node::node(const std::string& name)
    : libflo::node(name,
                   libflo::unknown<size_t>(),
                   libflo::unknown<size_t>(),
                   false,
                   true,
                   libflo::unknown<size_t>(),
                   libflo::unknown<std::string>()
        )
{
}

std::string node::vcd_string(void) const
{
    auto buffer = new char[width() + 5];

    std::stringstream s;
    s <<  _value.get_str(2);

    snprintf(buffer, width() + 4, "b%*s", (int)(width()), s.str().c_str());
    for (size_t i = 0; i < strlen(buffer); ++i)
        if (isspace(buffer[i]))
            buffer[i] = '0';

    std::string out = buffer;
    delete[] buffer;
    return out;
}

std::string unique_name(void)
{
    static unsigned long index = 0;

    char buffer[1024];
    snprintf(buffer, 1024, "Torture%lu", index);
    index++;

    return buffer;
}

void node::update(const mpz_class& value, ssize_t cycle)
{
    if (cycle < _changed_cycle) {
        fprintf(stderr, "Last changed on %ld, now %ld\n",
                _changed_cycle,
                cycle
            );
        abort();
    }

    _changed_cycle = cycle;
    _value = value;
}
