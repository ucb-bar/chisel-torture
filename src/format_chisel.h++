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

#ifndef FORMAT__CHISEL_HXX
#define FORMAT__CHISEL_HXX

#include "format_flo.h++"

class format_chisel: public format {
private:
    /* Chisel needs to have all the IO bundled together at the top of
     * a module. */
    std::vector<op_ptr> _compute;
    std::vector<node_ptr> _inputs;
    std::vector<node_ptr> _outputs;

public:
    format_chisel(FILE *circuit, FILE *vcd);
    ~format_chisel(void);

public:
    /* Overrides for format virtual methods. */
    virtual void write(const op_ptr& op);
    virtual void input(const node_ptr& node);
    virtual void output(const node_ptr& node);
};

#endif
