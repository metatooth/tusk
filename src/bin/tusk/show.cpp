#include "catalog.h"
#include "show.h" // class implemented

#include <CGAL/draw_surface_mesh.h>

using namespace tusk;

void
Show::usage()
{
  std::cerr << "show [path]\n\n"
            << "  Display the mesh from file at [path].\n"
            << "  File may be in binary PLY or STL format.\n\n"
            << "  for example, show impression.stl\n"
            << std::endl;
}// usage

int
Show::run(const std::string& path)
{
  try {
    Catalog catalog;

    Mesh A;

    std::cout << "Load mesh A from " << path << "... ";
    catalog.read(path, &A);
    std::cout << "done." << std::endl;

    CGAL::draw(A);

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
