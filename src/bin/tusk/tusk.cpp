#include <iostream>
#include <string>

#include "extrude.h"
#include "normalize.h"
#include "remesh.h"

void usage()
{
  std::cerr << "usage: tusk [--version] [--help] <command> [<args>]\n\n"
            << "These are the common Tusk commands used in various situations:\n\n"
            << "  extrude          Extrudes a mesh\n"
            << "  normalize        Transform mesh to Tusk convention\n"
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

  if (first.compare("extrude") == 0) {
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
  }

  return 0;
}
