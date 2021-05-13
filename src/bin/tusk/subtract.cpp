#include "catalog.h"
#include "subtract.h" // class implemented

#include <CGAL/Polygon_mesh_processing/corefinement.h>

namespace PMP = CGAL::Polygon_mesh_processing;

using namespace tusk;

void
Subtract::usage()
{
  std::cerr << "subtract [a] [b] [out]\n\n"
            << "  Boolean difference of watertight mesh [b] from [a].\n"
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
    
    Mesh A, B;
    
    std::cout << "Load mesh A from " << afile << "... ";
    catalog.read(afile, &A);
    std::cout << "done." << std::endl;

    std::cout << "Load mesh B from " << bfile << "... ";
    catalog.read(bfile, &B);
    std::cout << "done." << std::endl;

    std::cout << "Corefine and compute difference... ";
    if (PMP::corefine_and_compute_difference(A, B, A)) {
      std::cout << "done." << std::endl;

      std::cout << "Write result to " << outfile << "... ";
      catalog.write(A, outfile);
      std::cout << "done." << std::endl;
    } else {
      std::cout << "Difference could not be computed." << std::endl;
      return 1;
    }
  
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
