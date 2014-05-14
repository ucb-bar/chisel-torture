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

class counter: public libcgraph::pattern {
private:

public:
    counter(size_t width)
        {
            auto r = libcgraph::reg(width);
            auto sum = r + 1;
            auto up = r->update(sum);

            _compute = {r, sum, up};
            _outputs = {r->d()};
        }
};

class counter_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "counter";
        }

    const pattern_ptr create(void) const
        {
            return std::make_shared<counter>(32);
        }

    const pattern_ptr create(const std::string& args) const
        {
            return std::make_shared<counter>(atoi(args.c_str()));
        }

    std::vector<std::string> examples(void) const
        {
            return {"32", "64"};
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<counter_factory>();
    libcgraph::pattern_store::link(factory);
}
