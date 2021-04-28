#include "catalog.h"
#include "union.h" // class implemented

#include <CGAL/Polygon_mesh_processing/corefinement.h>

namespace PMP = CGAL::Polygon_mesh_processing;

using namespace tusk;

void
Union::usage()
{
  std::cerr << "union [a] [b] [out]\n\n"
            << "  Boolean unionion of watertight mesh [b] from [a].\n"
            << "  Result stored in [out] file. Files in binary STL format.\n\n"
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

    std::cout << "Load mesh B from " << bfile << "... ";
    catalog.read(afile, &B);
    std::cout << "done." << std::endl;

    Polyhedron out;
    bool valid = PMP::corefine_and_compute_union(A, B, out);

    if (valid) {
      std::cout << "Save result to " << outfile << "... ";
      catalog.write(out, outfile);
      std::cout << "done." << std::endl;
    } else {
      std::cout << "Union could not be computed." << std::endl;
    }
  
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
