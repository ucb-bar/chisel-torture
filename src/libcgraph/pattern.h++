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

#ifndef LIBCGRAPH__PATTERN_HXX
#define LIBCGRAPH__PATTERN_HXX

#include <memory>
#include <string>
#include <vector>
#include "node.h++"
#include "operation.h++"

namespace libcgraph {
    /* This class is sub-classed by all circuit patterns, which are
     * generators for circuits of different types.  The idea is that
     * patterns get wired together to form a top-level circuit graph,
     * which is then run through chisel.  Patterns must be able to
     * produce some output given their inputs, which means  */
    class pattern {
        typedef std::shared_ptr<node> node_ptr;
        typedef std::shared_ptr<operation> op_ptr;

    protected:
        /* These specify the inputs and outputs from this particular
         * pattern. */
        std::vector<node_ptr> _inputs;
        std::vector<node_ptr> _outputs;

        /* Here we have a list of all the nodes required to compute
         * the output from the inputs. */
        std::vector<op_ptr> _compute;

    public:
        /* Lists all the nodes required to compute a particular
         * pattern. */
        const std::vector<op_ptr>& compute(void) const { return _compute; }
        const std::vector<node_ptr>& inputs(void) const { return _inputs; }
        const std::vector<node_ptr>& outputs(void) const { return _outputs; }
    };
}

#endif
