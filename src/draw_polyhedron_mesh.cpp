#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

#include <CGAL/draw_polyhedron.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>

#include <fstream>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Polyhedron = CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3>;

namespace PMP = CGAL::Polygon_mesh_processing;

void read_polyhedron_mesh_from_stl(std::istream& in, Polyhedron* out)
{
  std::vector< CGAL::cpp11::array<double,3> > in_points;
  std::vector< CGAL::cpp11::array<size_t,3> > triangles;

  CGAL::read_STL(in, in_points, triangles, false);

  std::cout << "points: " << in_points.size() << "\n"
            << "triangles: " << triangles.size() << std::endl;

  std::vector<K::Point_3> points;
  points.resize(in_points.size());

  for (std::size_t i = 0, l = in_points.size(); i < l; i++) {
    points[i] = K::Point_3(in_points[i][0], in_points[i][1], in_points[i][2]);
  }

  std::cout << "num points: " << points.size() << std::endl;

  std::vector<std::vector<std::size_t> > polygons;
  polygons.resize(triangles.size());

  for (std::size_t i = 0, l = triangles.size(); i < l; i++) {
    polygons[i] = { triangles[i][0], triangles[i][1], triangles[i][2] };
  } 

  std::cout << "num polygons: " << polygons.size() << std::endl;

  PMP::orient_polygon_soup(points, polygons);

  std::cout << "Oriented." << std::endl;

  PMP::polygon_soup_to_polygon_mesh(points, polygons, *out);

  std::cout << "Soup to mesh." << std::endl;
}

void usage()
{
  std::cerr << "mouthguard [infile] [fit] [thickness] [outfile]\n\n"
            << "  Creates a mesh for a mouthguard for [infile] saving\n"
            << "  in STL format to [outfile]. Mouthguard will have a\n"
            << "  [thickness] specified in millimeters with an offset\n"
            << "  of [fit] from the [infile] surface, also in millimeters.\n\n"
            << "  for example, mouthguard 0000.stl 0000-design.stl"
            << std::endl;
}

const std::size_t NUM_ARGS = 5;

int main(int argc, char* argv[]) {

  if (argc != NUM_ARGS) {
    usage();
    exit(-1);
  }

  std::ifstream ifs(argv[1], std::ios::binary);

  Polyhedron res;
  try {
    read_polyhedron_mesh_from_stl(ifs, &res);
  } catch (std::exception& e) {
    std::cerr << "ERROR " << &e << std::endl;
  }

  ifs.close();

  CGAL::draw(res);

  return 0;
}
