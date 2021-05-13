#include <iostream>
#include <string>

#include "convention.h"
#include "convert.h"
#include "clip.h"
#include "extrude.h"
#include "normalize.h"
#include "remesh.h"
#include "show.h"
#include "subtract.h"
#include "union.h"

void usage()
{
  std::cerr << "usage: tusk [--version] [--help] <command> [<args>]\n\n"
            << "These are the common Tusk commands used in various situations:\n\n"
            << "  convention       Transform mesh to conventional orientation\n"
            << "  convert          Convert mesh file format\n"
            << "  clip             Clip a mesh by a plane\n"
            << "  extrude          Extrudes a mesh\n"
            << "  normalize        Transform mesh to \"normal\" dental position\n"
            << "  subtract         Difference of two watertight meshes.\n"
            << "  union            Union of two watertight meshes.\n"
            << "  remesh           Isotropic remeshing\n"
            << "  show             Displays a mesh\n"
            << std::endl;
}

using namespace tusk;

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

  if (first.compare("clip") == 0) {
    Clip clip;
    if (argc < 5) {
      clip.usage();
      return 1;
    }
    clip.run(atof(argv[2]), argv[3], argv[4]);
  } else if (first.compare("convention") == 0) {
    Convention convention;
    if (argc < 4) {
      convention.usage();
      return 1;
    }
    convention.run(argv[2], argv[3]);
  } else if (first.compare("convert") == 0) {
    Convert convert;
    if (argc < 4) {
      convert.usage();
      return 1;
    }

    std::size_t path = 2;
    if (argc > 4) {
      path = 3;
      std::string second(argv[2]);

      if (second.compare("--polyhedron") == 0) {
        convert.type("polyhedron");
      }
    }

    convert.run(argv[path], argv[path+1]);
  } else if (first.compare("extrude") == 0) {
    Extrude extrude;
    if (argc < 5) {
      extrude.usage();
      return 1;
    }
    extrude.run(atof(argv[2]), argv[3], argv[4]);
  } else if (first.compare("normalize") == 0) {
    Normalize normalize;
    if (argc < 4) {
      normalize.usage();
      return 1;
    }
    normalize.run(argv[2], argv[3]);
  } else if (first.compare("remesh") == 0) {
    Remesh remesh;
    if (argc < 5) {
      remesh.usage();
      return 1;
    }
    remesh.run(atof(argv[2]), argv[3], argv[4]);
  } else if (first.compare("show") == 0) {
    Show show;
    if (argc < 3) {
      show.usage();
      return 1;
    }

    std::size_t path = 2;
    if (argc > 3) {
      path = 3;
      std::string second(argv[2]);
      if (second.compare("--polyhedron") == 0) {
        show.type("polyhedron");
      }
    }

    show.run(argv[path]);
  } else if (first.compare("subtract") == 0) {
    Subtract subtract;
    if (argc < 5) {
      subtract.usage();
      return 1;
    }
    subtract.run(argv[2], argv[3], argv[4]);
  } else if (first.compare("union") == 0) {
    Union unn;
    if (argc < 5) {
      unn.usage();
      return 1;
    }
    unn.run(argv[2], argv[3], argv[4]);
  } else {
    usage();
    return 1;
  }

  return 0;
}
