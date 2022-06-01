#include "catalog.h"
#include "clip.h" // class implemented

#include <CGAL/Polygon_mesh_processing/clip.h>

using namespace tusk;

void
Clip::usage()
{
  std::cerr << "tusk clip [y-value] [infile] [outfile]\n\n"
            << "  Clip the [infile] mesh by a plane normal to the Y axis\n"
            << "  at [y-value] (millimeters). Saves to [outfile].\n"
            << "  File format by extension (stl|ply).\n\n"
            << "  for example, tusk clip 20 0000.ply 0000-model.ply\n"
            << std::endl;
}// usage

int
Clip::run(double offset, const char* infile, const char* outfile)
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
