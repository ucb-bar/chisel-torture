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

#ifndef LIBCGRAPH__PATTERN_STORE_HXX
#define LIBCGRAPH__PATTERN_STORE_HXX

#include <map>
#include <vector>
#include <memory>
#include "pattern_factory.h++"

namespace libcgraph {
    /* Implements a "pattern store", which contains a list of all the
     * patterns in the system.  Everything here is static because it
     * needs to be able to be poked by library constructors! */
    class pattern_store {
        typedef std::shared_ptr<pattern_factory> pattern_factory_ptr;

    private:
        /* This stores a global list of every pattern factory in the
         * whole system. */
        static std::vector<pattern_factory_ptr> _factories;
        static std::map<std::string, pattern_factory_ptr> _name_map;

    public:
        /* Registers a new pattern factory into the system. */
        static void link(const pattern_factory_ptr& factory);

        /* Lists all the pattern factories currently known to the
         * system. */
        static const std::vector<pattern_factory_ptr>& list(void);

        /* Looks up a pattern by name in the list. */
        static const pattern_factory_ptr lookup(const std::string& name);
    };
}

#endif
