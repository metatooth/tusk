#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Vector_3.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>

#include "utils.h"
#include "rply-1.1.4/rply.h"

#include <fstream>
#include <algorithm>

using K = CGAL::Simple_cartesian<float>;
using Polyhedron = CGAL::Polyhedron_3<K>;
using Point_3 = K::Point_3;
using Vector_3 = K::Vector_3;
using Facet_iterator = Polyhedron::Facet_iterator;
using Vertex_iterator = Polyhedron::Vertex_iterator;
using HF_circulator = Polyhedron::Halfedge_around_facet_circulator;

namespace PMP = CGAL::Polygon_mesh_processing;

std::size_t VERTEX_SIZE = 10;
std::size_t VERTEX_CURRENT = 0;

std::size_t FACES_SIZE = 300;
std::size_t FACES_CURRENT = 0;

static int vertex_cb(p_ply_argument argument) {
    void* pdata; 
    long eol;
    ply_get_argument_user_data(argument, &pdata, &eol);

    std::vector<double>* vertices = reinterpret_cast<std::vector<double>*> (pdata);
    vertices->push_back(ply_get_argument_value(argument));

    return 1;
}

static int face_cb(p_ply_argument argument) {
  void* pdata; 
  long eol;
  ply_get_argument_user_data(argument, &pdata, &eol);
  
  std::vector<std::size_t>* faces
    = reinterpret_cast<std::vector<std::size_t>*> (pdata);
  
  long length, value_index;
  ply_get_argument_property(argument, NULL, &length, &value_index);
  switch (value_index) {
    case 0:
    case 1:
    case 2:
      faces->push_back(ply_get_argument_value(argument));
      break;
    default:
      break;
  }

  return 1;
}

int read_PLY(const char* filename, 
             std::vector<Point_3>& points,
             std::vector<std::vector<std::size_t> >& polygons)
{
  p_ply ply = ply_open(filename, NULL, 0, NULL);
  if (!ply) return 1;
  if (!ply_read_header(ply)) return 1;

  std::vector<double> vertices;
  std::vector<std::size_t> faces;

  long nvertices, ntriangles;
  nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, &vertices, 0);
  ply_set_read_cb(ply, "vertex", "y", vertex_cb, &vertices, 0);
  ply_set_read_cb(ply, "vertex", "z", vertex_cb, &vertices, 1);
  ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, &faces, 0);
  if (!ply_read(ply)) return 1;

  ply_close(ply);

  points.resize(nvertices);

  for (std::size_t i = 0; i < nvertices; i++) {
    points[i] = Point_3(vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
  }

  polygons.resize(ntriangles);

  for (std::size_t i = 0; i < ntriangles; i++) {
    polygons[i] = { faces[3*i], faces[3*i+1], faces[3*i+2] };
  } 

  return 0;
}

int write_PLY(std::vector<Point_3>& points,
              std::vector<std::vector<std::size_t> >& polygons,
              const char* filename) {
  p_ply ply = ply_create(filename, PLY_LITTLE_ENDIAN, NULL, 0, NULL);
  if (!ply) return 1;

  unsigned long nvertices = points.size();
  unsigned long nfaces = polygons.size();
  
  if (!ply_add_element(ply, "vertex", nvertices)) return 1;
  if (!ply_add_scalar_property(ply, "x", PLY_FLOAT)) return 1;
  if (!ply_add_scalar_property(ply, "y", PLY_FLOAT)) return 1;
  if (!ply_add_scalar_property(ply, "z", PLY_FLOAT)) return 1;

  if (!ply_add_element(ply, "face", nfaces)) return 1;
  if (!ply_add_list_property(ply, "vertex_indices", PLY_UCHAR, PLY_INT)) return 1;

  if (!ply_write_header(ply)) return 1;

  for (std::size_t i = 0; i < nvertices; i++) {
    ply_write(ply, points[i].x());
    ply_write(ply, points[i].y());
    ply_write(ply, points[i].z());
  }

  for (std::size_t i = 0; i < nfaces; i++) {
    ply_write(ply, 3);
    ply_write(ply, polygons[i][0]);
    ply_write(ply, polygons[i][1]);
    ply_write(ply, polygons[i][2]);
  } 

  if (!ply_close(ply)) return 1;
  
  return 0;
}

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


typedef CGAL::Exact_predicates_inexact_constructions_kernel EK;
typedef CGAL::Surface_mesh<EK::Point_3> Mesh;
typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;

void
read_mesh(std::ifstream& in, Mesh* mesh)
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
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *mesh);
}

void
write_mesh(Mesh& mesh, std::ofstream& out)
{
  std::string info = "Source: Metatooth LLC, Format: STL, Type: Binary";
  char head[80];
  std::strncpy(head, info.c_str(), sizeof(head)-1);
  char attribute[2] = "0";
  unsigned long number = num_faces(mesh);

  out.write(head, sizeof(head));
  out.write((char*)&number, 4);
  
  BOOST_FOREACH(face_descriptor f, faces(mesh)) {
    CGAL::Vertex_around_face_iterator<Mesh> vbegin, vend;

    float coords[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };
    int i = 0;    
    for (boost::tie(vbegin, vend) = vertices_around_face(mesh.halfedge(f), mesh);
         vbegin != vend;
         ++vbegin) {
      coords[3*i+3] = mesh.point(*vbegin).x();
      coords[3*i+4] = mesh.point(*vbegin).y();
      coords[3*i+5] = mesh.point(*vbegin).z();
      i++;       
    }


    for (int i = 0; i < 12; ++i) {
      std::cout << "coord " << i << " " << coords[i] << std::endl;
      out.write((char*)&coords[i], 4);
    }
    out.write(attribute, 2);

    i++;
  }
}
