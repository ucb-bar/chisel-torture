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

class asm_log2: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::node> in, out;

public:
    asm_log2(size_t _width)
        : in(std::make_shared<libcgraph::node>()),
          out(std::make_shared<libcgraph::node>())
        {
            in->update_width(_width);
            out->update_width(_width);

            _inputs = {in};
            _outputs = {out};

            auto op = std::make_shared<libcgraph::operation>(
                out,
                _width,
                libflo::opcode::LOG2,
                _inputs
                );
            _compute = {op};
        }

    void step(ssize_t cycle)
        {
            if (in->value() != 0) {
                mpz_class test = 1;
                mpz_class log = 0;
                while (test < in->value()) {
                    test *= 2;
                    log++;
                }

                if (test != in->value())
                    log--;

                out->update(log, cycle);
            } else {
                mpz_class zero = 0;
                out->update(zero, cycle);
            }
        }
};

class asm_log2_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "asm_log2";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int width;
            if (sscanf(args.c_str(), "%d", &width) != 1) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<asm_log2>(width);
        }

    std::vector<std::string> examples(void) const
        {
            return {"3", "5", "19", "32", "64", "65", "127"};
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<asm_log2_factory>();
    libcgraph::pattern_store::link(factory);
}
