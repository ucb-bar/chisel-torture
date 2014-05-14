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

format_flo::format_flo(FILE *circuit, FILE *vcd)
    : format(circuit, vcd)
{
}

void format_flo::write( const op_ptr& op)
{
    op->writeln(_circuit);
}

void format_flo::input(const node_ptr& node)
{
    write(io_op(node));
}

void format_flo::output(const node_ptr& node)
{
    write(io_op(node));
}
