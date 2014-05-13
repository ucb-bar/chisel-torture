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

#ifndef LIBCGRAPH__PATTERN_FACTORY_HXX
#define LIBCGRAPH__PATTERN_FACTORY_HXX

#include <string>
#include "pattern.h++"

namespace libcgraph {
    /* This class is capable of generating patterns. */
    class pattern_factory {
    public:
        /* Returns the name of this pattern.  Note that names have to
         * be unique everywhere in the system! */
        virtual const std::string name(void) const = 0;
    };
}

#endif
