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

#ifndef LIBCGRAPH__OPERATION_HXX
#define LIBCGRAPH__OPERATION_HXX

#include <libflo/opcode.h++>
#include <libflo/operation.h++>
#include <memory>
#include <string>
#include <vector>
#include "node.h++"

namespace libcgraph {
    /* Holds an operation, which can operate on nodes. */
    class operation: public libflo::operation<node> {
        typedef std::shared_ptr<operation> op_ptr;

    public:
        operation(const std::shared_ptr<node>& dest,
                  const libflo::unknown<size_t>& width,
                  const libflo::opcode& op,
                  const std::vector<std::shared_ptr<node>>& s)
            : libflo::operation<node>(dest, width, op, s)
            {
            }

    public:
        /* These operators allow us to more easily deal with computing
         * nodes. */
        friend op_ptr operator+(const op_ptr& a, const op_ptr& b);
        friend op_ptr operator+(const op_ptr& a, size_t b);
        friend op_ptr operator+(size_t a, const op_ptr& b);

        /* Updates a register, which itself is an operation (it
         * requires a mov).  Note that this is an error for anything
         * that's not a register. */
        op_ptr update(const op_ptr& val);
    };
}

#endif
