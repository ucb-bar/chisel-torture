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

#include "pattern_merge.h++"
using namespace libcgraph;

void pattern_merge::parallel(const std::shared_ptr<pattern>& to_add)
{
    _patterns.push_back(to_add);

    for (const auto& in: to_add->inputs())
        _inputs.push_back(in);
    for (const auto& out: to_add->outputs())
        _outputs.push_back(out);
    for (const auto& op: to_add->compute())
        _compute.push_back(op);
}

void pattern_merge::step(void)
{
    for (const auto& pattern: _patterns)
        pattern->step();
}
