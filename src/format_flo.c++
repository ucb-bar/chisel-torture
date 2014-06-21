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

#include "format_flo.h++"
#include <libcgraph/operation.h++>
#include <libcgraph/node.h++>

format_flo::format_flo(FILE *circuit, FILE *vcd)
    : format(circuit, vcd)
{
    auto reset_node = std::make_shared<libcgraph::node>("reset");
    auto reset_op = std::make_shared<libcgraph::operation>(
        reset_node,
        1,
        libflo::opcode::RST,
        std::vector<libcgraph::node_ptr>()
        );
    reset_op->writeln(_circuit);
}

void format_flo::write(const op_ptr& op)
{
    /* Registers need to be reset to 0.  Currently the only way to do
     * this is to insert a MUX, which is kind of a pain as it requires
     * a graph transformation.  Luckily we can just do it inline and
     * be OK. */
    if (op->op() == libflo::opcode::REG) {
        auto no_reset = op->t();
        auto on_reset = std::make_shared<libcgraph::node>(0);
        auto reset = std::make_shared<libcgraph::node>("reset");
        auto reg_in = std::make_shared<libcgraph::node>();

        /* Here's the MUX we put around reset. */
        auto mux = std::make_shared<libcgraph::operation>(
            reg_in,
            op->width(),
            libflo::opcode::MUX,
            std::vector<libcgraph::node_ptr>({reset, on_reset, no_reset})
            );
        mux->writeln(_circuit);

        /* Here's a register we use for the actual output. */
        auto reg = std::make_shared<libcgraph::operation>(
            op->d(),
            op->width(),
            libflo::opcode::REG,
            std::vector<libcgraph::node_ptr>({op->s(), reg_in})
            );
        reg->writeln(_circuit);
    } else {
        op->writeln(_circuit);
    }
}

void format_flo::input(const node_ptr& node)
{
    write(io_op(node));
}

void format_flo::output(const node_ptr& node)
{
    write(io_op(node));
}
