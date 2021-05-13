#include <CGAL/Aff_transformation_3.h>

#include "utils.h"
#include "normalize.h" // class implemented

using Affine = CGAL::Aff_transformation_3<K>;
using Vector = K::Vector_3;

using namespace tusk;

void
Normalize::usage()
{
  std::cerr << "tusk normalize [infile] [outfile]\n\n"
            << "  Transform the [infile] mesh to meet Tusk orientation\n"
            << "  conventions, saving to [outfile]. Files to be in binary\n"
            << "  PLY or STL format.\n\n"
            << "  for example, tusk normalize input.ply output.ply\n"
            << std::endl;
}// usage

int
Normalize::run(const char* infile, const char* outfile)
{
  try {
    std::vector<Point> points;
    std::vector<std::vector<size_t> > polygons;
    
    std::cout << "Loading mesh from " << infile << "... ";
    if (std::string(infile).find(".ply") != std::string::npos) {
      read_ply(infile, points, polygons);
    } else {
      read_stl(infile, points, polygons);
    }
    std::cout << "done." << std::endl;

    double avgx = CGAL::to_double(points[0].x());
    double avgy = CGAL::to_double(points[0].y());
    double avgz = CGAL::to_double(points[0].z());
    for (std::size_t i = 1, l = points.size(); i < l; i++) {
      avgx = (i*avgx + CGAL::to_double(points[i].x())) / (i+1);
      avgy = (i*avgy + CGAL::to_double(points[i].y())) / (i+1);
      avgz = (i*avgz + CGAL::to_double(points[i].z())) / (i+1);
    }
    std::cout << "avgx " << avgx << std::endl;
    std::cout << "avgy " << avgy << std::endl;
    std::cout << "avgz " << avgz << std::endl; 

    Affine center(CGAL::TRANSLATION, Vector(-avgx, -avgy, -avgz));
    
    Affine nY15(cos(PI/12), 0, -sin(PI/12),
                         0, 1,           0,
                sin(PI/12), 0,  cos(PI/12), 
                1);

    Affine normalize = nY15 * center;

    for (std::size_t i = 0, l = points.size(); i < l; i++) {
      points[i] = normalize(points[i]);
    }

    std::cout << "Write to " << outfile << std::endl;
    write_ply(points, polygons, outfile);
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
