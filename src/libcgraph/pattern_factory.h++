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

#include <memory>
#include <string>
#include "pattern.h++"

namespace libcgraph {
    /* This class is capable of generating patterns. */
    class pattern_factory {
        typedef std::shared_ptr<pattern> pattern_ptr;

    public:
        /* Returns the name of this pattern.  Note that names have to
         * be unique everywhere in the system! */
        virtual const std::string name(void) const = 0;

        /* Creates a new instance of this pattern using random values
         * for all the parameters. */
        virtual const pattern_ptr create(void) const = 0;

        /* Creates a new instance of this pattern by parsing a string
         * in order to set the parameters.  Note that the exact
         * formatting of this string is entirely up to the subclass!
         * The default implementation for this simply ignores the
         * parameters, which is probably not the smartest of
         * ideas... */
        virtual const pattern_ptr create(const std::string& params) const;

        /* Lists some examples of how to instantiate a particular
         * pattern. */
        virtual std::vector<std::string> examples(void) const = 0;
    };
}

#endif
