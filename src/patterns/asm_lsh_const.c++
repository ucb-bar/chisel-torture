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

class asm_lsh_const: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::node> in;
    std::shared_ptr<libcgraph::node> out;
    const size_t out_width;
    const size_t offset;

public:
    asm_lsh_const(size_t _in_width, size_t _out_width, size_t _offset)
        : in(std::make_shared<libcgraph::node>()),
          out(std::make_shared<libcgraph::node>()),
          out_width(_out_width),
          offset(_offset)
        {
            in->update_width(_in_width);
            out->update_width(_out_width);

            auto offsetn = std::make_shared<libcgraph::node>(_offset);
            offsetn->update_width(_in_width);

            _inputs = {in};
            _outputs = {out};

            auto op = std::make_shared<libcgraph::operation>(
                out,
                _out_width,
                libflo::opcode::LSH,
                std::vector<std::shared_ptr<libcgraph::node>>({in, offsetn})
                );
            _compute = {op};

#ifdef DEBUG_PRINT_BIT_EXTRACT
            fprintf(stderr, "asm_lsh_const %lu-%lu-%lu: ",
                    _in_width, _out_width, _offset);
            op->writeln_debug(stderr);
#endif
        }

    void step(ssize_t cycle)
        {
            mpz_class mask = (mpz_class(1) << out_width) - 1;

            out->update((in->value() << offset) & mask, cycle);
        }
};

class asm_lsh_const_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "asm_lsh_const";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int iw, ow, o;
            if (sscanf(args.c_str(), "%d-%d-%d", &iw, &ow, &o) != 3) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<asm_lsh_const>(iw, ow, o);
        }

    std::vector<std::string> examples(void) const
        {
            return {"32-32-0", "32-32-1", "32-1-1",
                    "128-32-0", "128-32-1", "128-32-61",
                    "128-64-0", "128-64-1", "128-64-63", "128-64-64",
                    "32-128-0", "32-128-1", "64-128-0", "64-128-63"};
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<asm_lsh_const_factory>();
    libcgraph::pattern_store::link(factory);
}
