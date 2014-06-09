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

#include "pattern_merge.h++"
using namespace libcgraph;

typedef std::shared_ptr<node> node_ptr;

void pattern_merge::parallel(const std::shared_ptr<pattern>& to_add)
{
    _patterns.push_back(to_add);

    for (const auto& in: to_add->inputs())
        _inputs.push_back(in);

    for (const auto& out: to_add->outputs()) {
        _outputs.push_back(out);
        _free_outputs.push_back(out);
    }

    for (const auto& op: to_add->compute())
        _compute.push_back(op);
}

void pattern_merge::series(const std::shared_ptr<pattern>& to_add)
{
    _patterns.push_back(to_add);

    for (const auto& in: to_add->inputs()) {
        auto source = find_output_for_input(in);

        /* It's possible there's no source that is capable of
         * providing this sort of input, in which case we should just
         * bail out and provide this as a top-level input. */
        if (source == NULL) {
            _inputs.push_back(in);
            continue;
        }

        /* If we ended up able to copy the node then we'll need to
         * make a link.  Note that this is a two part process: first
         * we create a link in software: */
        source->link_to(in);

        /* And then we create the link in the generate circuit. */
        auto zero = std::make_shared<node>(0);
        auto op = std::make_shared<operation>(
            in,
            in->width(),
            libflo::opcode::RSH,
            std::vector<node_ptr>({source, zero})
            );
        _compute.push_back(op);
    }

    for (const auto& out: to_add->outputs()) {
        _outputs.push_back(out);
        _free_outputs.push_back(out);
    }

    for (const auto& op: to_add->compute())
        _compute.push_back(op);
}

void pattern_merge::step(ssize_t cycle)
{
    for (const auto& pattern: _patterns)
        pattern->step(cycle);
}

node_ptr pattern_merge::find_output_for_input(const node_ptr& input)
{
    /* Attempt to find an otherwise unused output node with the same
     * width of the node that we're trying to fill. */
    for (auto it = _free_outputs.begin(); it != _free_outputs.end(); ++it) {
        auto source = *it;

        /* If someone has left an unknown width lying around then
         * don't bother at all. */
        if (input->known_width() == false)
            continue;
        if (source->known_width() == false)
            continue;

        if (input->width() == source->width()) {
            _free_outputs.erase(it);
            return source;
        }
    }

    /* If the input node doesn't have a known width then there's
     * really nothing we can do -- it's going to be impossible to
     * generate any sort of width-extended node at all. */
    if (input->known_width() == false) {
        fprintf(stderr,
                "Attempted to match input of unknown width\n"
                "Some pattern has generated an invalid input node\n"
            );
        abort();
    }

    /* If there's no node of matching width then just return
     * anything. */
    for (auto it = _free_outputs.begin(); it != _free_outputs.end(); ++it) {
        auto source = *it;

        _free_outputs.erase(it);
        return source;
    }

    /* Only when there are no nodes do we end up returning NULL. */
    return NULL;
}
