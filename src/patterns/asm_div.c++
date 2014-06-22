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

class asm_div: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::node> inA, inB, out;

public:
    asm_div(size_t _width)
        : inA(std::make_shared<libcgraph::node>()),
          inB(std::make_shared<libcgraph::node>()),
          out(std::make_shared<libcgraph::node>())
        {
            inA->update_width(_width);
            inB->update_width(_width);
            out->update_width(_width);

            _inputs = {inA, inB};
            _outputs = {out};

            auto zero = std::make_shared<libcgraph::node>(0);
            auto one  = std::make_shared<libcgraph::node>(1);

            auto eqz = std::make_shared<libcgraph::node>();
            eqz->update_width(1);

            auto eqz_op = std::make_shared<libcgraph::operation>(
                eqz,
                _width,
                libflo::opcode::EQ,
                std::vector<libcgraph::node_ptr>({inB, zero})
                );

            auto chkB = std::make_shared<libcgraph::node>();
            chkB->update_width(_width);
            auto mux_op = std::make_shared<libcgraph::operation>(
                chkB,
                _width,
                libflo::opcode::MUX,
                std::vector<libcgraph::node_ptr>({eqz, one, inB})
                );

            auto op = std::make_shared<libcgraph::operation>(
                out,
                _width,
                libflo::opcode::DIV,
                std::vector<libcgraph::node_ptr>({inA, chkB})
                );

            _compute = {eqz_op, mux_op, op};
        }

    void step(ssize_t cycle)
        {
            if (inB->value() == 0)
                out->update(inA->value(), cycle);
            else
                out->update(inA->value() / inB->value(), cycle);
        }
};

class asm_div_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "asm_div";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int width;
            if (sscanf(args.c_str(), "%d", &width) != 1) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<asm_div>(width);
        }

    std::vector<std::string> examples(void) const
        {
            return {"3", "5", "19", "32", "64", "65", "127"};
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<asm_div_factory>();
    libcgraph::pattern_store::link(factory);
}
