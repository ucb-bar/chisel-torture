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

class asm_lsh_var: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::node> in;
    std::shared_ptr<libcgraph::node> out;
    std::shared_ptr<libcgraph::node> offset;
    const size_t in_width;
    const size_t out_width;
    const size_t offset_width;

public:
    asm_lsh_var(size_t _in_w, size_t _out_w, size_t _offset_w)
        : in(std::make_shared<libcgraph::node>()),
          out(std::make_shared<libcgraph::node>()),
          offset(std::make_shared<libcgraph::node>()),
          in_width(_in_w),
          out_width(_out_w),
          offset_width(_offset_w)
        {
            in->update_width(in_width);
            out->update_width(out_width);
            offset->update_width(offset_width);

            _inputs = {in, offset};
            _outputs = {out};

            auto op = std::make_shared<libcgraph::operation>(
                out,
                out_width,
                libflo::opcode::LSH,
                std::vector<std::shared_ptr<libcgraph::node>>({in, offset})
                );
            _compute = {op};
        }

    void step(ssize_t cycle)
        {
            mpz_class mask = (mpz_class(1) << out_width) - 1;
            size_t offset_value = offset->value().get_ui();

            out->update((in->value() << offset_value) & mask, cycle);
        }
};

class asm_lsh_var_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "asm_lsh_var";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int iw, ow, tw;
            if (sscanf(args.c_str(), "%d-%d-%d", &iw, &ow, &tw) != 3) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<asm_lsh_var>(iw, ow, tw);
        }

    std::vector<std::string> examples(void) const
        {
#if 0
            /* FIXME: It's undefined what Chisel does if you shift by
             * more bits than there are. */
            return {"32-32-4", "32-32-5", "32-32-6"};
#else
            return {"32-32-4", "32-32-5",
                    "128-32-7", "256-1-8", "256-65-7"};
#endif
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<asm_lsh_var_factory>();
    libcgraph::pattern_store::link(factory);
}
