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

#ifndef LIBCGRAPH__PATTERN_MERGE_HXX
#define LIBCGRAPH__PATTERN_MERGE_HXX

#include "pattern.h++"

namespace libcgraph {
    /* Stores a special sort of pattern that actually serves to merge
     * other patterns together. */
    class pattern_merge: public pattern {
        std::vector<std::shared_ptr<pattern>> _patterns;

    public:
        /* Adds a new pattern in parallel with the current pattern. */
        void parallel(const std::shared_ptr<pattern>& to_add);

    public:
        /* libcgraph::pattern virtual overrides */
        void step(void);
    };
}

#endif
