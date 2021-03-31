#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Vector_3.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>

#include <fstream>
#include <algorithm>

using K = CGAL::Simple_cartesian<float>;

//using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polyhedron = CGAL::Polyhedron_3<K>;
using Point_3 = K::Point_3;
using Vector_3 = K::Vector_3;
using Surface_mesh = CGAL::Surface_mesh<Point_3>;
using Facet_iterator = Polyhedron::Facet_iterator;
using HF_circulator = Polyhedron::Halfedge_around_facet_circulator;

namespace PMP = CGAL::Polygon_mesh_processing;
  
void read_polyhedron(std::ifstream& in, Polyhedron* P)
{
  std::vector< CGAL::cpp11::array<double,3> > in_points;
  std::vector< CGAL::cpp11::array<size_t,3> > triangles;

  CGAL::read_STL(in, in_points, triangles, false);

  std::vector<Point_3> points;
  points.resize(in_points.size());

  for (std::size_t i = 0, l = in_points.size(); i < l; i++) {
    points[i] = Point_3(in_points[i][0], in_points[i][1], in_points[i][2]);
  }

  std::vector<std::vector<std::size_t> > polygons;
  polygons.resize(triangles.size());

  for (std::size_t i = 0, l = triangles.size(); i < l; i++) {
    polygons[i] = { triangles[i][0], triangles[i][1], triangles[i][2] };
  } 

  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *P);
}

void write_polyhedron(Polyhedron& P, std::ofstream& out)
{
  std::string info = "Source: Metatooth LLC, Format: STL, Type: Binary";
  char head[80];
  std::strncpy(head, info.c_str(), sizeof(head)-1);
  char attribute[2] = "0";
  unsigned long number = P.size_of_facets();

  out.write(head, sizeof(head));
  out.write((char*)&number, 4);
            
  Facet_iterator last_f = P.facets_end();
  -- last_f;

  Facet_iterator f = P.facets_begin();
  do {
    HF_circulator h = f->facet_begin();
    Point_3 p = h->vertex()->point();
    Point_3 q = h->next()->vertex()->point();
    Point_3 r = h->next()->next()->vertex()->point();
    
    Vector_3 n = CGAL::collinear(p, q, r) ?
      Vector_3(1, 0, 0) : CGAL::unit_normal(p, q, r);
    
    float coords[12] =
      {
       n.x(),
       n.y(),
       n.z(),
       p.x(),
       p.y(),
       p.z(),
       q.x(),
       q.y(),
       q.z(),
       r.x(),
       r.y(),
       r.z()
      };
    
    for (int i = 0; i < 12; ++i) {
      out.write((char*)&coords[i], 4);
    }

    out.write(attribute, 2);

  } while (f++ != last_f);
}

