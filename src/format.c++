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

#include "format.h++"
#include <libflo/flo.h++>
#include <unordered_map>

format::format(FILE *circuit, FILE *vcd)
    : _circuit(circuit),
      _vcd(vcd),
      _io_names(),
      _io_ops()
{
}

format::~format(void)
{
    fclose(_circuit);
    fclose(_vcd);
}

void format::write(const pattern_ptr& pattern)
{
    for (const auto& node: pattern->inputs()) {
        /* Generate the op that allows this node to be connected to
         * the outside world. */
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

            _io_ops[node] = op;
        }

        /* Generates a slightly shorter version of the name of this
         * operation that. */
        {
            static unsigned long index = 0;
            char in_name[1024];
            snprintf(in_name, 1024, "io_in%lu", index++);

            _io_names[node] = in_name;
        }

        input(node);
    }

    for (const auto& node: pattern->outputs()) {
        /* Generate the op that allows this node to be connected to
         * the outside world. */
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

            _io_ops[node] = op;
        }

        /* Generates a slightly shorter version of the name of this
         * operation that. */
        {
            static unsigned long index = 0;
            char out_name[1024];
            snprintf(out_name, 1024, "io_out%lu", index++);

            _io_names[node] = out_name;
        }

        output(node);
    }

    for (const auto& op: pattern->compute())
        write(op);
}

void format::vcd(const pattern_ptr& pattern, size_t cycles __attribute__((unused)))
{
    /* Create a flo object, which deals with infering widths and
     * such. */
    auto flo = libflo::flo<libcgraph::node, libcgraph::operation>::empty();
    for (const auto& op: pattern->compute())
        flo->add_op(op);
    flo->canon();

    /* Generate a short VCD temporary name for every node that is
     * going to be output. */
    std::unordered_map<node_ptr, std::string> short_names;
    auto gen_short_name = [&short_names](node_ptr n) -> void
        {
            static long unsigned index = 0;
            char short_name[1024];
            snprintf(short_name, 1024, "N%lu", index++);

            short_names[n] = short_name;
        };
    for (const auto& node: pattern->inputs())
        gen_short_name(node);
    for (const auto& node: pattern->outputs())
        gen_short_name(node);

    /* Write the VCD header. */
    fprintf(_vcd, "$scope module Torture\n");

    for (const auto& pair: short_names) {
        fprintf(_vcd, "$var wire %lu %s %s\n",
                pair.first->width(),
                io_name(pair.first).c_str(),
                pair.second.c_str()
            );
    }
}

std::string format::io_name(const node_ptr& node)
{
    auto l = _io_names.find(node);
    if (l == _io_names.end())
        abort();
    return l->second;
}

std::shared_ptr<libcgraph::operation> format::io_op(const node_ptr& op)
{
    auto l = _io_ops.find(op);
    if (l == _io_ops.end())
        abort();
    return l->second;
}
