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

#include <libcgraph/pattern_merge.h++>
#include <libcgraph/pattern_store.h++>

#ifdef FLO
#include "format_flo.h++"
#endif

#ifdef CHISEL
#include "format_chisel.h++"
#endif

int main(int argc __attribute__((unused)),
         const char **argv __attribute__((unused)))
{
#if defined(FLO)
    std::shared_ptr<format> format = std::make_shared<format_flo>();
#elif defined(CHISEL)
    std::shared_ptr<format> format = std::make_shared<format_chisel>();
#else
#error "Define an output format"
#endif

    /* This special sort of pattern actually allows us to merge
     * together other patterns. */
    auto merge = std::make_shared<libcgraph::pattern_merge>();

    /* Walk through the list of all patterns we know how to build. */
    for (const auto& factory: libcgraph::pattern_store::list()) {
        for (const auto& example: factory->examples()) {
            /* Create a new instance of that sort of pattern from the
             * given factory and then proceed to add it in parallel
             * with the rest of the patterns to the big one we're
             * going to output. */
            auto pattern = factory->create(example);
            merge->parallel(pattern);
        }
    }

    /* Now that we've got the super-pattern, just go ahead and write
     * it out. */
    format->write(stdout, merge);

    return 0;
}
