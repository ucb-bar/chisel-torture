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

#ifndef LIBCGRAPH__NODE_HXX
#define LIBCGRAPH__NODE_HXX

#include <libflo/node.h++>
#include <memory>
#include <string>
#include <vector>

namespace libcgraph {
    /* Holds a single circuit node. */
    class node: public libflo::node {
    public:
        /* Generates a new node with a unique name. */
        node(void);

        /* Generates a new node that is a constant, with the given
         * value. */
        node(size_t value);

        /* Generates a new explicitly-named node. */
        node(const std::string& name);
    };
}

#endif
