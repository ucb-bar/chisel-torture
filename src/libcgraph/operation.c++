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
using namespace libcgraph;
typedef std::shared_ptr<operation> op_ptr;

op_ptr libcgraph::operator+(const op_ptr& a, const op_ptr& b)
{
    auto out = std::make_shared<node>();

    return std::make_shared<operation>(
        out,
        libflo::unknown<size_t>(),
        libflo::opcode::ADD,
        std::vector<std::shared_ptr<node>>({a->d(), b->d()})
        );
}

op_ptr libcgraph::operator+(const op_ptr& a, size_t b)
{
    return b + a;
}

op_ptr libcgraph::operator+(size_t a, const op_ptr& b)
{
    auto out = std::make_shared<node>();
    auto ca = std::make_shared<node>(a);

    return std::make_shared<operation>(
        out,
        libflo::unknown<size_t>(),
        libflo::opcode::ADD,
        std::vector<std::shared_ptr<node>>({ca, b->d()})
        );
}

op_ptr operation::update(const op_ptr& val)
{
    if (op() != libflo::opcode::REG) {
        fprintf(stderr, "Only register can be updated!\n");
        abort();
    }

    return std::make_shared<operation>(
        this->s(1),
        libflo::unknown<size_t>(),
        libflo::opcode::MOV,
        std::vector<std::shared_ptr<node>>({val->d()})
        );
}
