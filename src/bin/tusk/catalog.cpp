#include "catalog.h" // class implemented
#include "utils.h"

#include <CGAL/IO/STL_reader.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <fstream>
#include <vector>

typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

using namespace tusk;

Catalog::Catalog()
{
}// Constructor

Catalog::~Catalog()
{
}// Destructor

void
Catalog::read(const std::string& path, Mesh* M)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  if (path.find(".ply") != std::string::npos) {
    read_ply(path.c_str(), points, polygons);
  } else if (path.find(".stl") != std::string::npos) {
    read_stl(path.c_str(), points, polygons);
  }
 
  PMP::repair_polygon_soup(points, polygons);
  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *M);
}// read

void
Catalog::read(const std::string& path, Polyhedron* P)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  if (path.find(".ply") != std::string::npos) {
    read_ply(path.c_str(), points, polygons);
  } else if (path.find(".stl") != std::string::npos) {
    read_stl(path.c_str(), points, polygons);
  }

  PMP::repair_polygon_soup(points, polygons);
  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *P);
}// read

void
Catalog::write(const Mesh& M, const std::string& path)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  std::size_t i = 0;
  BOOST_FOREACH(face_descriptor f, faces(M)) {
    CGAL::Vertex_around_face_iterator<Mesh> vbegin, vend;

    for (boost::tie(vbegin, vend) = vertices_around_face(M.halfedge(f), M);
         vbegin != vend;
         ++vbegin) {
      double x = M.point(*vbegin).x();
      double y = M.point(*vbegin).y();
      double z = M.point(*vbegin).z();
      points.push_back(Point(x, y, z));
    }

    std::vector<std::size_t> polygon { i++, i++, i++ };

    polygons.push_back(polygon);
  }

  if (path.find(".ply") != std::string::npos) {
    write_ply(points, polygons, path.c_str());
  } else if (path.find(".stl") != std::string::npos) {
    write_stl(points, polygons, path.c_str());
  }
}// write

void
Catalog::write(const Polyhedron& P, const std::string& path)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  auto iter = P.facets_begin();
  auto nter = P.facets_end();
  std::size_t i = 0;
  while (iter != nter) {
    auto circ = iter->facet_begin();
    for (std::size_t j = 0; j < 3; j++) {
      double x = circ->vertex()->point().x();
      double y = circ->vertex()->point().y();
      double z = circ->vertex()->point().z();
      points.push_back(Point(x, y, z));
      circ++;
    }

    std::vector<std::size_t> polygon { i++, i++, i++ };
    polygons.push_back(polygon);

    iter++;
  }
  
  if (path.find(".ply") != std::string::npos) {
    write_ply(points, polygons, path.c_str());
  } else if (path.find(".stl") != std::string::npos) {
    write_stl(points, polygons, path.c_str());
  }
}// write

