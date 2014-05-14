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

format_flo::format_flo(FILE *file)
    : format(file)
{
}

void format_flo::write( const op_ptr& op)
{
    op->writeln(_file);
}

void format_flo::input(const node_ptr& node)
{
    static unsigned long index = 0;
    char in_name[1024];
    snprintf(in_name, 1024, "Torture::io_in%lu", index++);

    auto in = std::make_shared<libcgraph::node>(in_name);

    auto op = std::make_shared<libcgraph::operation>(
        node,
        libflo::unknown<size_t>(),
        libflo::opcode::IN,
        std::vector<std::shared_ptr<libcgraph::node>>({in})
        );
    write(op);
}

void format_flo::output(const node_ptr& node)
{
    static unsigned long index = 0;
    char out_name[1024];
    snprintf(out_name, 1024, "Torture::io_out%lu", index++);

    auto out = std::make_shared<libcgraph::node>(out_name);

    auto op = std::make_shared<libcgraph::operation>(
        out,
        libflo::unknown<size_t>(),
        libflo::opcode::OUT,
        std::vector<std::shared_ptr<libcgraph::node>>({node})
        );
    write(op);
}
