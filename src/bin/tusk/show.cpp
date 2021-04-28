#include "catalog.h"
#include "show.h" // class implemented

#include <CGAL/draw_polyhedron.h>
#include <CGAL/draw_surface_mesh.h>

using namespace tusk;

Show::Show()
{
}// Constructor

void
Show::usage()
{
  std::cerr << "tusk show (--mesh|--polyhedron) [path]\n\n"
            << "  Display the mesh from file at [path].\n"
            << "    --mesh          Use a surface mesh (default)\n"
            << "    --polyhedron    Use a polyhedron\n"
            << "  File may be in binary PLY or STL format.\n\n"
            << "  for example, show impression.stl\n"
            << std::endl;
}// usage

int
Show::run(const std::string& path)
{
  try {
    Catalog catalog;

    if (type().compare("polyhedron") == 0) {
      Polyhedron A;

      std::cout << "Load polyhedron A from " << path << "... ";
      catalog.read(path, &A);
      std::cout << "done." << std::endl;

      CGAL::draw(A);
    } else if (type().compare("mesh") == 0) {
      Mesh A;

      std::cout << "Load mesh A from " << path << "... ";
      catalog.read(path, &A);
      std::cout << "done." << std::endl;

      CGAL::draw(A);
    }

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
