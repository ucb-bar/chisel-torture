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

#include <libcgraph/pattern_store.h++>
#include <string.h>

#ifdef FLO
#include "format_flo.h++"
#endif

#ifdef CHISEL
#include "format_chisel.h++"
#endif

int main(int argc, const char **argv)
{
#if defined(FLO)
    FILE *cir = fopen("Torture.flo", "w");
#elif defined(CHISEL)
    FILE *cir = fopen("Torture.scala", "w");
#endif
    FILE *vcd = fopen("Torture.vcd", "w");

#if defined(FLO)
    std::shared_ptr<format> format = std::make_shared<format_flo>(cir, vcd);
#elif defined(CHISEL)
    std::shared_ptr<format> format = std::make_shared<format_chisel>(cir, vcd);
#else
#error "Define an output format"
#endif

    if ((argc == 1) || ((argc == 2) && (strcmp(argv[1], "--help") == 0))) {
        fprintf(stderr, "%s: A set of Chisel circuit patterns\n", argv[0]);
        fprintf(stderr, "  --list: Lists all the patterns by name\n");
        fprintf(stderr, "  --list <pattern>: Lists examples of a pattern\n");
        fprintf(stderr, "  --show <pattern> <args>: Creates and prints\n");
        return 0;
    }

    if ((argc == 2) && (strcmp(argv[1], "--list") == 0)) {
        for (const auto& pattern: libcgraph::pattern_store::list())
            printf("%s\n", pattern->name().c_str());
    }

    if ((argc == 3) && (strcmp(argv[1], "--list") == 0)) {
        auto factory = libcgraph::pattern_store::lookup(argv[2]);
        if (factory == NULL) {
            fprintf(stderr, "No such pattern: '%s'\n", argv[2]);
            abort();
        }

        for (const auto& example: factory->examples())
            printf("%s\n", example.c_str());
    }

    if ((argc == 4) && (strcmp(argv[1], "--show") == 0)) {
        auto factory = libcgraph::pattern_store::lookup(argv[2]);
        if (factory == NULL) {
            fprintf(stderr, "No such pattern: '%s'\n", argv[2]);
            abort();
        }

        auto pattern = factory->create(argv[3]);
        format->write(pattern);
        format->vcd(pattern, 100);
    }

    return 0;
}
