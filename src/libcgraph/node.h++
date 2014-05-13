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

#include <memory>
#include <string>
#include <vector>

namespace libcgraph {
    /* Holds a single circuit node. */
    class node {
        typedef std::shared_ptr<node> node_ptr;

    private:
        const std::string _op;
        const std::vector<node_ptr> _inputs;

    public:
        node(std::string op);
        node(node_ptr a, std::string op);
        node(node_ptr a, node_ptr b, std::string op);

    public:
        /* Returns the operation associated with this node. */
        const std::string& op(void) const { return _op; }
    };
}

#endif
