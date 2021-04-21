#include "catalog.h" // class implemented
#include "utils.h"

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>

#include <fstream>

namespace PMP = CGAL::Polygon_mesh_processing;

using face_descriptor = boost::graph_traits<Mesh>::face_descriptor;

using namespace tusk;

Catalog::Catalog()
{
}// Constructor

Catalog::~Catalog()
{
}// Destructor

void
Catalog::read(const std::string& file, Mesh* M)
{
  if (file.find(".ply") != std::string::npos) {
    readPLY(file, M);
  } else if (file.find(".stl") != std::string::npos) {
    readSTL(file, M);
  }
}// read


void
Catalog::read(const std::string& file, Polyhedron* P)
{
  Mesh m;

  if (file.find(".ply") != std::string::npos) {
    readPLY(file, &m);
  } else if (file.find(".stl") != std::string::npos) {
    readSTL(file, &m);
  }
}// read

void
Catalog::write(const Mesh& M, const std::string& file)
{
  if (file.find(".ply") != std::string::npos) {
    writePLY(M, file);
  } else if (file.find(".stl") != std::string::npos) {
    writeSTL(M, file);
  }
}// write

void
Catalog::write(const Polyhedron& P, const std::string& file)
{
  Mesh m;

  if (file.find(".ply") != std::string::npos) {
    writePLY(m, file);
  } else if (file.find(".stl") != std::string::npos) {
    writeSTL(m, file);
  }
}// write

void
Catalog::readSTL(const std::string& file, Mesh* mesh)
{
  std::vector< CGAL::cpp11::array<double,3> > in_points;
  std::vector< CGAL::cpp11::array<size_t,3> > triangles;

  std::ifstream ifs(file.c_str());
  CGAL::read_STL(ifs, in_points, triangles, false);

  std::vector<Point> points;
  points.resize(in_points.size());

  for (std::size_t i = 0, l = in_points.size(); i < l; i++) {
    points[i] = Point(in_points[i][0], in_points[i][1], in_points[i][2]);
  }

  std::vector<std::vector<std::size_t> > polygons;
  polygons.resize(triangles.size());

  for (std::size_t i = 0, l = triangles.size(); i < l; i++) {
    polygons[i] = { triangles[i][0], triangles[i][1], triangles[i][2] };
  } 

  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *mesh);

  ifs.close();
}// readSTL

void
Catalog::readPLY(const std::string& file, Mesh* mesh)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  read_PLY(file.c_str(), points, polygons);

  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *mesh);
}// readPLY

void
Catalog::writeSTL(const Mesh& mesh, const std::string& file)
{
  std::string info = "Source: Metatooth LLC, Format: STL, Type: Binary";
  char head[80];
  std::strncpy(head, info.c_str(), sizeof(head)-1);
  char attribute[2] = "0";
  unsigned long number = num_faces(mesh);

  std::ofstream ofs(file.c_str(), std::ios::out | std::ios::binary);
  ofs.write(head, sizeof(head));
  ofs.write((char*)&number, 4);
  
  BOOST_FOREACH(face_descriptor f, faces(mesh)) {
    CGAL::Vertex_around_face_iterator<Mesh> vbegin, vend;

    double coords[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };
    int i = 0;    
    for (boost::tie(vbegin, vend) = vertices_around_face(mesh.halfedge(f), mesh);
         vbegin != vend;
         ++vbegin) {
      coords[3*i+3] = CGAL::to_double(mesh.point(*vbegin).x());
      coords[3*i+4] = CGAL::to_double(mesh.point(*vbegin).y());
      coords[3*i+5] = CGAL::to_double(mesh.point(*vbegin).z());
      i++;       
    }


    for (int i = 0; i < 12; ++i) {
      std::cout << "coord " << i << " " << coords[i] << std::endl;
      ofs.write((char*)&coords[i], 4);
    }
    ofs.write(attribute, 2);

    i++;
  }

  ofs.close();
}// writeSTL

void
Catalog::writePLY(const Mesh& mesh, const std::string& file)
{
  std::vector<Point> points;
  std::vector<std::vector<std::size_t> > polygons;

  write_PLY(points, polygons, file.c_str());
}// writePLY
