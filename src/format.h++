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

#ifndef FORMAT__HXX
#define FORMAT__HXX

#include <libcgraph/node.h++>
#include <libcgraph/operation.h++>
#include <libcgraph/pattern.h++>
#include <stdio.h>
#include <unordered_map>

class format {
protected:
    typedef std::shared_ptr<libcgraph::node> node_ptr;
    typedef std::shared_ptr<libcgraph::operation> op_ptr;
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

protected:
    /* The file that we're going to output this circuit into. */
    FILE *_circuit;
    FILE *_vcd;

    /* This stores a mapping from nodes to their cooresponding name in
     * the IO bundle. */
    std::unordered_map<node_ptr, std::string> _io_names;
    std::unordered_map<node_ptr, op_ptr> _io_ops;

public:
    /* Creates a new output formatter with the given file as the
     * target. */
    format(FILE *circut, FILE *vcd);
    ~format(void);

public:
    /* Writes a single pattern out as a circuit to the circuit output
     * file. */
    virtual void write(const pattern_ptr& pattern);

    /* Writes the values computed for a single pattern to the VCD
     * output file. */
    virtual void vcd(const pattern_ptr& pattern, size_t cycles);

    /* We need to generate another name for all the disconnected
     * inputs and outputs in the circuit. */
    std::string io_name(const node_ptr& node);
    op_ptr io_op(const node_ptr& op);

protected:
    /* Helper functions that allow different sorts of formats to write
     * out single elements. */
    virtual void write(const op_ptr& op) = 0;
    virtual void input(const node_ptr& node) = 0;
    virtual void output(const node_ptr& node) = 0;

};

#endif
