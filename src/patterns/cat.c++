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

#include <libcgraph/pattern.h++>
#include <libcgraph/pattern_factory.h++>
#include <libcgraph/pattern_store.h++>
#include <libcgraph/reg.h++>
#include <gmpxx.h>
#include <sstream>

class cat: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::node> left, right;
    std::shared_ptr<libcgraph::node> out;
    ssize_t cycle;
    const size_t left_width;
    const size_t right_width;

public:
    cat(size_t _left_width, size_t _right_width)
        : left(std::make_shared<libcgraph::node>()),
          right(std::make_shared<libcgraph::node>()),
          out(std::make_shared<libcgraph::node>()),
          cycle(0),
          left_width(_left_width),
          right_width(_right_width)
        {
            left->update_width(left_width);
            right->update_width(right_width);
            out->update_width(left_width + right_width);

            _inputs = {left, right};
            _outputs = {out};

            auto op = std::make_shared<libcgraph::operation>(
                out,
                right_width,
                libflo::opcode::CAT,
                std::vector<std::shared_ptr<libcgraph::node>>({left, right})
                );
            _compute = {op};

#ifdef DEBUG_PRINT_CAT
            fprintf(stderr, "cat %lu-%lu: ",
                    _in_width, _out_width, _offset);
            op->writeln_debug(stderr);
#endif
        }

    void step(void)
        {
            out->update((left->value() << right_width) | right->value(),
                        cycle);

            cycle++;
        }
};

class cat_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "cat";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int lw, rw;
            if (sscanf(args.c_str(), "%d-%d", &lw, &rw) != 2) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<cat>(lw, rw);
        }

    std::vector<std::string> examples(void) const
        {
#if 0
            return {"1-1", "64-64", "65-65", "64-1", "1-64"};
#else
            /* FIXME: cat-64-1 doesn't work because of an LLVM
             * optimizer bug. */
            return {"1-1", "64-64", "65-65", "1-64"};
#endif
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<cat_factory>();
    libcgraph::pattern_store::link(factory);
}
