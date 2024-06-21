#include "catalog.h"
#include "trim.h" // class implemented

#include <CGAL/Polygon_mesh_processing/clip.h>

using namespace tusk;

void
Trim::usage()
{
  std::cerr << "tusk trim [hex] [infile] [outfile]\n\n"
            << "  Trim the [infile] mesh by removing faces not marked by.\n"
            << "  the [hex] color. Saves to [outfile].\n"
            << "  File format by extension (stl|ply).\n\n"
            << "  for example, tusk trim ff33bb 0000.ply 0000-trimmed.ply\n"
            << std::endl;
}// usage

int
Trim::run(const char* hex, const char* infile, const char* outfile)
{
  try {
    Catalog catalog;
    Polyhedron A;

    std::cout << "Load polyhedron from " << infile << "... " << std::flush;
    catalog.read(infile, &A);
    std::cout << "done." << std::endl;

    Polyhedron B;

    Polyhedron R;

    std::cout << "Write to " << outfile << "... " << std::flush;
    catalog.write(R, outfile);
    std::cout << "done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
