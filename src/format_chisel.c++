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

#include "format_chisel.h++"

format_chisel::format_chisel(FILE *circuit, FILE *vcd)
    : format(circuit, vcd)
{
}

format_chisel::~format_chisel(void)
{
    auto mangle_io_name = [&](const node_ptr& n) -> std::string
        {
            auto name = n->short_chisel_name();
            if (_io_names.find(n) != _io_names.end())
                name = _io_names[n];

            if (strncmp(name.c_str(), "io_", 3) == 0)
                return name.c_str() + 3;

            return n->name();
        };

    fprintf(_circuit, "import Chisel._;\n");

    /* Generate a test harness. */
    fprintf(_circuit, "object Torture {\n");
    fprintf(_circuit, "  def main(args: Array[String]): Unit = {\n");
    fprintf(_circuit, "    chiselMain(args, () => Module(new Torture()));\n");
    fprintf(_circuit, "  }\n");
    fprintf(_circuit, "}\n");

    fprintf(_circuit, "class Torture extends Module {\n");

    /* Write all the input and output into a class.  Everything is
     * emitted as a Bits because */
    fprintf(_circuit, "  class IO extends Bundle {\n");
    for (const auto& n: _inputs) {
        fprintf(_circuit, "    val %s = Bits(INPUT, width=%lu);\n",
                mangle_io_name(n).c_str(),
                n->width()
            );
    }
    for (const auto& n: _outputs) {
        fprintf(_circuit, "    val %s = Bits(OUTPUT, width=%lu);\n",
                mangle_io_name(n).c_str(),
                n->width()
            );
    }
    fprintf(_circuit, "  }\n");

    /* Emit that class that we just generated. */
    fprintf(_circuit, "  val io = new IO();\n");

    /* Now go ahead and try to emit every computation. */
    for (const auto& in: _inputs) {
        /* While input nodes don't _actually_ need to do anything, if
         * we don't do this then we'll need to prefix all input nodes
         * with "io.".  Essentially this is just name mangling. */
        fprintf(_circuit, "  val %s = Bits(width = %lu);\n",
                in->short_chisel_name().c_str(),
                in->width()
            );
        fprintf(_circuit, "  %s := io.%s;\n",
                in->short_chisel_name().c_str(),
                mangle_io_name(in).c_str()
            );
    }

    /* This first pass through the operations list creates all the
     * wires and state elements so they can be referenced later. */
    for (const auto& op: _compute) {
        switch (op->op()) {
        case libflo::opcode::IN:
        case libflo::opcode::OUT:
            break;

        case libflo::opcode::REG:
            fprintf(_circuit, "  val %s = Reg(init = Bits(0, width = %lu));\n",
                    op->d()->short_chisel_name().c_str(),
                    op->d()->width()
                );
            break;

        case libflo::opcode::ADD:
        case libflo::opcode::AND:
        case libflo::opcode::CAT:
        case libflo::opcode::DIV:
        case libflo::opcode::LOG2:
        case libflo::opcode::LSH:
        case libflo::opcode::MOV:
        case libflo::opcode::MUX:
        case libflo::opcode::NEG:
        case libflo::opcode::NOT:
        case libflo::opcode::RSH:
        case libflo::opcode::XOR:
            fprintf(_circuit, "  val %s = Bits(width = %lu);\n",
                    op->d()->short_chisel_name().c_str(),
                    op->d()->width()
                );
            break;

        case libflo::opcode::EQ:
        case libflo::opcode::GTE:
        case libflo::opcode::LT:
        case libflo::opcode::NEQ:
            fprintf(_circuit, "  val %s = Bits(width = 1);\n",
                    op->d()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::ARSH:
        case libflo::opcode::CATD:
        case libflo::opcode::EAT:
        case libflo::opcode::INIT:
        case libflo::opcode::LD:
        case libflo::opcode::LIT:
        case libflo::opcode::MSK:
        case libflo::opcode::MUL:
        case libflo::opcode::NOP:
        case libflo::opcode::OR:
        case libflo::opcode::RD:
        case libflo::opcode::RND:
        case libflo::opcode::RSHD:
        case libflo::opcode::RST:
        case libflo::opcode::ST:
        case libflo::opcode::SUB:
        case libflo::opcode::WR:
        case libflo::opcode::MEM:
            fprintf(stderr, "Unimplemented operation: ");
            op->writeln_debug(stderr);
            abort();
            break;
        }
    }

    /* Here's the real operation generation code. */
    for (const auto& op: _compute) {
        switch (op->op()) {
        case libflo::opcode::IN:
            /* Input nodes won't ever exist. */
            fprintf(stderr, "IN nodes can't exist: ");
            op->writeln_debug(stderr);
            abort();
            break;

        case libflo::opcode::OUT:
            /* Output nodes won't ever exist. */
            fprintf(stderr, "OUT nodes can't exist: ");
            op->writeln_debug(stderr);
            abort();
            break;

        case libflo::opcode::ADD:
            fprintf(_circuit, "  %s := (UInt(%s) + UInt(%s)).toBits;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::AND:
            fprintf(_circuit, "  %s := %s & %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::CAT:
            fprintf(_circuit, "  %s := Cat(%s, %s);\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::DIV:
            fprintf(_circuit, "  %s := %s / %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::EQ:
            fprintf(_circuit, "  %s := %s === %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::GTE:
            fprintf(_circuit, "  %s := %s >= %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::LOG2:
            fprintf(_circuit, "  %s := Log2(%s);\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::LT:
            fprintf(_circuit, "  %s := %s < %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::LSH:
            fprintf(_circuit, "  %s := (UInt(%s) << UInt(%s)).toBits;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::MOV:
            fprintf(_circuit, "  %s := %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::MUX:
            fprintf(_circuit, "  %s := Mux(%s === UInt(1), %s, %s);\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str(),
                    op->u()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::NEG:
            fprintf(_circuit, "  %s := -%s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::NEQ:
            fprintf(_circuit, "  %s := %s != %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::NOT:
            fprintf(_circuit, "  %s := ~%s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::REG:
            fprintf(_circuit, "  %s := %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::RSH:
            fprintf(_circuit, "  %s := (UInt(%s) >> UInt(%s)).toBits;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::XOR:
            fprintf(_circuit, "  %s := %s ^ %s;\n",
                    op->d()->short_chisel_name().c_str(),
                    op->s()->short_chisel_name().c_str(),
                    op->t()->short_chisel_name().c_str()
                );
            break;

        case libflo::opcode::ARSH:
        case libflo::opcode::CATD:
        case libflo::opcode::EAT:
        case libflo::opcode::INIT:
        case libflo::opcode::LD:
        case libflo::opcode::LIT:
        case libflo::opcode::MEM:
        case libflo::opcode::MSK:
        case libflo::opcode::MUL:
        case libflo::opcode::NOP:
        case libflo::opcode::OR:
        case libflo::opcode::RD:
        case libflo::opcode::RND:
        case libflo::opcode::RSHD:
        case libflo::opcode::RST:
        case libflo::opcode::ST:
        case libflo::opcode::SUB:
        case libflo::opcode::WR:
            fprintf(stderr, "Unimplemented operation: ");
            op->writeln_debug(stderr);
            abort();
            break;
        }
    }

    for (const auto& out: _outputs) {
        fprintf(_circuit, "  io.%s := %s;\n",
                mangle_io_name(out).c_str(),
                out->short_chisel_name().c_str()
            );
    }

    fprintf(_circuit, "}\n");
}

void format_chisel::write(const op_ptr& op)
{
    _compute.push_back(op);
}

void format_chisel::input(const node_ptr& node)
{
    _inputs.push_back(node);
}

void format_chisel::output(const node_ptr& node)
{
    _outputs.push_back(node);
}
