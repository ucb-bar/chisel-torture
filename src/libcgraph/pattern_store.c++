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

#include "pattern_store.h++"
using namespace libcgraph;

typedef std::shared_ptr<pattern_factory> pattern_factory_ptr;

std::vector<pattern_factory_ptr> pattern_store::_factories;
std::map<std::string, pattern_factory_ptr> pattern_store::_name_map;

void pattern_store::link(const pattern_factory_ptr& factory)
{
    auto l = _name_map.find(factory->name());
    if (l != _name_map.end()) {
        fprintf(stderr, "Duplicate factory '%s'\n", factory->name().c_str());
        abort();
    }

    _name_map[factory->name()] = factory;
    _factories.push_back(factory);
}

const std::vector<pattern_factory_ptr>& pattern_store::list(void)
{
    return _factories;
}
