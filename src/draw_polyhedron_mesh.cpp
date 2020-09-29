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

int main(int argc, char* argv[])
{
  Polyhedron polyhedron;

  const char* filename = (argc > 1) ? argv[1] : "badfile";

  std::ifstream input(filename, std::ios::binary);
  std::vector< CGAL::cpp11::array<double,3> > in_points;
  std::vector< CGAL::cpp11::array<int,3> > triangles;

  CGAL::read_STL(input, in_points, triangles);

  input.close();

  std::cout << "points: " << in_points.size() << "\n"
            << "triangles: " << triangles.size() << std::endl;

  std::vector<K::Point_3> points;
  for (CGAL::cpp11::array<double,3> p : in_points) {
    points.push_back(K::Point_3(p[0], p[1], p[2]));
  }

  std::cout << "num points: " << points.size() << std::endl;

  std::vector<std::vector<std::size_t> > polygons;

  for (CGAL::cpp11::array<int,3> t : triangles) {
    std::vector<std::size_t> p = {(std::size_t) t[0],
                                  (std::size_t) t[1],
                                  (std::size_t) t[2]};
    polygons.push_back(p);
  } 

  std::cout << "num polygons: " << polygons.size() << std::endl;

  PMP::orient_polygon_soup(points, polygons);

  Polyhedron m;
  PMP::polygon_soup_to_polygon_mesh(points, polygons, m);

  CGAL::draw(m);
  return EXIT_SUCCESS;
}
