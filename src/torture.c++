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
#include <stdlib.h>

#ifdef FLO
#include "format_flo.h++"
#endif

#ifdef CHISEL
#include "format_chisel.h++"
#endif

class torture_args {
private:
    unsigned int _seed;
    size_t _max_count, _min_count;
    size_t _count;

public:
    torture_args(int argc, const char **argv)
        : _seed(0),
          _max_count(5),
          _min_count(1)
        {
            /* Parse the command-line arguments. */
            for (int i = 1; i < argc; ++i) {
                if (strcmp(argv[i], "--seed") == 0) {
                    this->_seed = atoi(argv[i+1]);
                    fprintf(stderr, "seed: %u\n", this->_seed);
                    i++;
                }
            }

            /* Most of the class variables need to be generated from
             * the random seed. */
            this->_count = rand(this->_min_count, this->_max_count);
        }

    size_t rand(void)
        {
            size_t out = rand_r(&(this->_seed));
            fprintf(stderr, "rand: %lu\n", out);
            return out;
        }

    size_t rand(size_t max)
        {
            return rand(0, max);
        }

    size_t rand(size_t min, size_t max)
        {
            return (rand() % (max - min)) + min;
        }

    size_t count(void)
        {
            return this->_count;
        }
};

int main(int argc, const char **argv)
{
#if defined(FLO)
    FILE *cir = fopen("Torture.flo", "w");
#elif defined(CHISEL)
    FILE *cir = fopen("Torture.scala", "w");
#endif
    FILE *vcd = fopen("Torture.vcd", "w");

#if defined(FLO)
    std::shared_ptr<format> format = std::make_shared<format_flo>(cir, vcd);
#elif defined(CHISEL)
    std::shared_ptr<format> format = std::make_shared<format_chisel>(cir, vcd);
#else
#error "Define an output format"
#endif

    auto args = torture_args(argc, argv);

    /* This special sort of pattern actually allows us to merge
     * together other patterns. */
    auto merge = std::make_shared<libcgraph::pattern_merge>();

    /* Walk through the list of all patterns we know how to build,
     * generating a big array of all the (factory, example)
     * pairings. */
    auto pairings = std::vector<std::pair<std::shared_ptr<libcgraph::pattern_factory>, std::string>>();
    for (const auto& factory: libcgraph::pattern_store::list())
        for (const auto& example: factory->examples())
            pairings.push_back(std::make_pair(factory, example));

    /* Randomly select a number of configurations to be part of this
     * torture circuit. */
    for (auto i = args.count(); i > 0; --i) {
        auto index = args.rand() % pairings.size();
        auto pair = pairings[index];
        auto instance = pair.first->create(pair.second);
        /* FIXME: Don't just insert patterns in parallel, do them in
         * series as well! */
        merge->parallel(instance);
    }

    /* Now that we've got the super-pattern, just go ahead and write
     * it out. */
    format->write(merge);
    format->vcd(merge, 100);

    return 0;
}
