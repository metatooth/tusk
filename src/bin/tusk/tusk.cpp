#include <iostream>
#include <string>

#include "convention.h"
#include "extrude.h"
#include "normalize.h"
#include "remesh.h"
#include "subtract.h"

void usage()
{
  std::cerr << "usage: tusk [--version] [--help] <command> [<args>]\n\n"
            << "These are the common Tusk commands used in various situations:\n\n"
            << "  convention       Transform mesh to conventional orientation\n"
            << "  extrude          Extrudes a mesh\n"
            << "  normalize        Transform mesh to \"normal\" dental position\n"
            << "  subtract         Boolean subtraction of two watertight meshes.\n"
            << "  remesh           Isotropic remeshing\n"
            << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2) {
    usage();
    return 1;
  }

  std::string first(argv[1]);

  if (first.compare("--version") == 0) {
    std::cout << "tusk version 0.1.0" << std::endl;
    return 0;
  } else if (first.compare("--help") == 0) {
    usage();
    return 0;
  }

  std::cout << "do " << first << std::endl;

  if (first.compare("convention") == 0) {
    if (argc < 4) {
      convention_usage();
      return 1;
    }
    convention(argv[2], argv[3]);
  } else if (first.compare("extrude") == 0) {
    if (argc < 5) {
      extrude_usage();
      return 1;
    }
    extrude(atof(argv[2]), argv[3], argv[4]);
  } else if (first.compare("normalize") == 0) {
    if (argc < 4) {
      normalize_usage();
      return 1;
    }
    normalize(argv[2], argv[3]);
  } else if (first.compare("remesh") == 0) {
    if (argc < 5) {
      remesh_usage();
      return 1;
    }
    remesh(atof(argv[2]), argv[3], argv[4]);
  } else if (first.compare("subtract") == 0) {
    if (argc < 5) {
      subtract_usage();
      return 1;
    }
    subtract(argv[2], argv[3], argv[4]);
  }

  return 0;
}

/* ----------------------------------------------------------------------
 * Copyright (C) 2021 Metatooth LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ---------------------------------------------------------------------- */
