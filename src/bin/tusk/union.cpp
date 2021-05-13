#include "catalog.h"
#include "union.h" // class implemented

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/draw_polyhedron.h>

typedef CGAL::Nef_polyhedron_3<K> Nef_polyhedron;

using namespace tusk;

void
Union::usage()
{
  std::cerr << "union [a] [b] [out]\n\n"
            << "  Boolean union of watertight mesh [a] and [b].\n"
            << "  Result stored in [out] file. Supported extensions (PLY, STL).\n\n"
            << "  for example, union a.stl b.stl output.stl\n"
            << std::endl;
}// usage

int
Union::run(const std::string& afile,
           const std::string& bfile,
           const std::string& outfile)
{
  try {
    Catalog catalog;

    Polyhedron A, B;
    
    std::cout << "Load mesh A from " << afile << "... ";
    catalog.read(afile, &A);
    std::cout << "done." << std::endl;
    CGAL::draw(A);

    std::cout << "Load mesh B from " << bfile << "... ";
    catalog.read(bfile, &B);
    std::cout << "done." << std::endl;
    CGAL::draw(B);

    std::cout << "Make Nef 1... ";
    Nef_polyhedron N1(A);
    std::cout << "done." << std::endl;
    std::cout << "Make Nef 2... ";
    Nef_polyhedron N2(B);
    std::cout << "done." << std::endl;
    
    std::cout << "Unite Nef 1 and Nef 2... ";
    N1 += N2;
    std::cout << "done." << std::endl;
    
    std::cout << "Convert to polyhedron... ";
    Polyhedron R;
    N1.convert_to_polyhedron(R);
    std::cout << "done." << std::endl;

    CGAL::draw(R);

    std::cout << "Write result to " << outfile << "... ";
    catalog.write(R, outfile);
    std::cout << "done." << std::endl;
  
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
