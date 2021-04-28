#include <CGAL/Aff_transformation_3.h>

#include "catalog.h"
#include "convert.h" // class implemented

using namespace tusk;

void
Convert::usage()
{
  std::cerr << "tusk convert (--mesh|--polyhedron) [infile] [outfile]\n\n"
            << "  Convert the [infile] mesh to [outfile] mesh\n"
            << "    --mesh          Use a surface mesh (default)\n"
            << "    --polyhedron    Use a polyhedron\n"
            << "  Supported file extensions (.stl, .ply)\n\n"
            << "  for example, tusk convert input.ply output.stl\n"
            << std::endl;
}// usage

int
Convert::run(const char* infile, const char* outfile)
{
  try {
    Catalog catalog;

    if (type().compare("polyhedron") == 0) {
      Polyhedron A;
      catalog.read(infile, &A);
      catalog.write(A, outfile);
    } else if (type().compare("mesh") == 0) {
      Mesh M;
      std::cout << "Read mesh from " << infile << "... ";
      catalog.read(infile, &M);
      std::cout << "done." << std::endl;
      std::cout << "Write mesh to " << outfile << "... ";
      catalog.write(M, outfile);
      std::cout << "done." << std::endl;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
