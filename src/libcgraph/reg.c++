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

#include "operation.h++"
#include "node.h++"
#include "reg.h++"

std::shared_ptr<libcgraph::operation> libcgraph::reg(size_t width)
{
    auto regout = std::make_shared<libcgraph::node>();
    auto regin = std::make_shared<libcgraph::node>();

    auto c1 = std::make_shared<libcgraph::node>(1);

    return std::make_shared<libcgraph::operation>(
        regout,
        libflo::unknown<size_t>(width),
        libflo::opcode::REG,
        std::vector<std::shared_ptr<libcgraph::node>>({c1, regin})
        );
}
