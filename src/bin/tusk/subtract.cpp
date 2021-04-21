#include <CGAL/Nef_polyhedron_3.h>

#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

#include "catalog.h"
#include "subtract.h" // class implemented

#include <iostream>
#include <sstream>
#include <fstream>

using Nef_polyhedron = CGAL::Nef_polyhedron_3<K>;

using namespace tusk;

void
Subtract::usage()
{
  std::cerr << "subtract [a] [b] [out]\n\n"
            << "  Boolean subtraction of watertight mesh [b] from [a].\n"
            << "  Result stored in [out] file. Files in binary STL format.\n\n"
            << "  for example, subtract a.stl b.stl output.stl\n"
            << std::endl;
}// usage

int
Subtract::run(const std::string& afile,
              const std::string& bfile,
              const std::string& outfile)
{
  try {
    Catalog catalog;

    Polyhedron A, B;

    std::cout << "Load mesh A from " << afile << "... ";
    catalog.read(afile, &A);
    std::cout << "done." << std::endl;

    std::cout << "Load mesh B from " << bfile << "... ";
    catalog.read(afile, &B);
    std::cout << "done." << std::endl;

    std::cout << "A is closed? " << A.is_closed() << std::endl;
    std::cout << "A is pure bivalent? " << A.is_pure_bivalent() << std::endl;
    std::cout << "A is pure trivalent? " << A.is_pure_trivalent() << std::endl;
    std::cout << "A is pure triangle? " << A.is_pure_triangle() << std::endl;
    std::cout << "A is pure quad? " << A.is_pure_quad() << std::endl;
    std::cout << "A is valid? " << A.is_valid() << std::endl;

    std::cout << "B is closed? " << B.is_closed() << std::endl;
    std::cout << "B is pure bivalent? " << B.is_pure_bivalent() << std::endl;
    std::cout << "B is pure trivalent? " << B.is_pure_trivalent() << std::endl;
    std::cout << "B is pure triangle? " << B.is_pure_triangle() << std::endl;
    std::cout << "B is pure quad? " << B.is_pure_quad() << std::endl;
    std::cout << "B is valid? " << B.is_valid() << std::endl;

    std::cout << "Make Nef1... ";
    Nef_polyhedron nef1(A);
    std::cout << "done." << std::endl;

    std::cout << "Make Nef2... ";
    Nef_polyhedron nef2(B);
    std::cout << "done." << std::endl;

    std::cout << "Nef difference... ";
    Nef_polyhedron nef = nef1 - nef2;
    std::cout << "done." << std::endl;

    Mesh result;

    std::cout << "Convert to mesh... ";
    CGAL::convert_nef_polyhedron_to_polygon_mesh(nef, result);
    std::cout << "done." << std::endl;

    std::cout << "Save result to " << outfile << "... ";
    catalog.write(result, outfile);
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
