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

class reg: public libcgraph::pattern {
private:
    std::shared_ptr<libcgraph::operation> r;
    size_t width;
    mpz_class delay;

public:
    reg(size_t _width)
        : r(libcgraph::reg(_width)),
          width(_width)
        {
            auto in = std::make_shared<libcgraph::node>();
            in->update_width(width);

            auto update = r->update(in);

            _inputs = {in};
            _compute = {r, update};
            _outputs = {r->d()};

            delay = 0;
        }

    void step(ssize_t cycle)
        {
            _outputs[0]->update(delay, cycle);
            delay = _inputs[0]->value();
        }
};

class reg_factory: public libcgraph::pattern_factory {
    typedef std::shared_ptr<libcgraph::pattern> pattern_ptr;

    const std::string name(void) const
        {
            return "reg";
        }

    const pattern_ptr create(const std::string& args) const
        {
            int width;
            if (sscanf(args.c_str(), "%d", &width) != 1) {
                fprintf(stderr, "Unable to parse args '%s'\n", args.c_str());
                abort();
            }

            return std::make_shared<reg>(width);
        }

    std::vector<std::string> examples(void) const
        {
            return {"32", "64"};
        }
};

static void cons(void) __attribute__((constructor));
void cons(void)
{
    auto factory = std::make_shared<reg_factory>();
    libcgraph::pattern_store::link(factory);
}
