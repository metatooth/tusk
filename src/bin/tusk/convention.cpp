#include <CGAL/Aff_transformation_3.h>

#include "catalog.h"
#include "convention.h" // class implemented
#include "utils.h"

#include <algorithm>

using Affine = CGAL::Aff_transformation_3<K>;
using Vector_3 = K::Vector_3;

using namespace tusk;

void
Convention::usage()
{
  std::cerr << "tusk convention [infile] [outfile]\n\n"
            << "  Transform the [infile] mesh to meet Tusk conventional\n"
            << "  orientation, saving to [outfile]. Files to be in binary\n"
            << "  PLY or STL format.\n\n"
            << "  for example, tusk convention input.ply output.ply\n"
            << std::endl;
}// usage

int
Convention::run(const char* infile, const char* outfile)
{
  try {
    std::vector<Point> points;
    std::vector<std::vector<size_t> > polygons;
    
    std::cout << "Loading mesh from " << infile << "..." << std::endl;
    Polyhedron P;
    Catalog catalog;
    catalog.read(infile, &P);
    std::cout << "Done." << std::endl;
   
    Affine x90(1,          0,         0,
               0, cos(PI/2), -sin(PI/2),
               0, sin(PI/2),  cos(PI/2),
               1);
    
    Affine y90( cos(PI/2), 0, sin(PI/2),
                        0, 1,         0,
               -sin(PI/2), 0, cos(PI/2), 
               1);

    Affine z180(cos(PI), -sin(PI), 0,
                sin(PI),  cos(PI), 0,
                       0,       0, 1,
                1);
    
    Affine A = z180 * y90 * x90;

    std::transform( P.points_begin(), P.points_end(), P.points_begin(), A );

    std::cout << "Write to " << outfile << std::endl;
    catalog.write(P, outfile);
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
