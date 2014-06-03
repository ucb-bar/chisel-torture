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
#include <gmpxx.h>

namespace libcgraph {
    /* Holds a single circuit node. */
    class node: public libflo::node {
    private:
        ssize_t _changed_cycle;
        mpz_class _value;

        /* These are the nodes that are "downstream" from this one,
         * which means they'll get updated whenever this node gets
         * updated. */
        std::vector<std::shared_ptr<node>> _downstream;

    public:
        /* Generates a new node with a unique name. */
        node(void);

        /* Generates a new node that is a constant, with the given
         * value. */
        node(size_t value);

        /* Generates a new explicitly-named node. */
        node(const std::string& name);

    public:
        /* Returns TRUE if this node changed on the given cycle. */
        bool changed_on_cycle(ssize_t cycle) { return _changed_cycle == cycle; }

        /* Returns a string that represents the current value of this
         * node as a VCD string. */
        std::string vcd_string(void) const;

        /* Updates the value of this node, also updating the cycle it
         * cooresponds to. */
        void update(const std::shared_ptr<node>& n, ssize_t cycle)
            { this->update(n->_value, cycle); }
        void update(const mpz_class& value, ssize_t cycle);

        /* Returns the value of this node. */
        const mpz_class& value(void) const { return _value; }

        /* Links this node to another node.  This is exactly the same
         * as just MOVing this node to the given node every time it is
         * updated. */
        void link_to(const std::shared_ptr<node>& target);
    };
}

#endif
