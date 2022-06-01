#include "utils.h" // class implemented

#include <CGAL/IO/STL/STL_reader.h>

#include "rply-1.1.4/rply.h"

#include <fstream>
#include <algorithm>

using Vector = K::Vector_3;

using namespace tusk;

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

int
tusk::read_ply(const char* filename, 
               std::vector<Point>& points,
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
    points[i] = Point(vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
  }

  polygons.resize(ntriangles);

  for (std::size_t i = 0; i < ntriangles; i++) {
    polygons[i] = { faces[3*i], faces[3*i+1], faces[3*i+2] };
  } 

  return 0;
}// read_ply

char buffer[4];

float parse_float(std::ifstream& s) {
  s.read(buffer, 4);
  return *(float*) buffer;
}

Point parse_point(std::ifstream& s) {
  float x = parse_float(s);
  float y = parse_float(s);
  float z = parse_float(s);
  return Point(x, y, z);
}
  
int
tusk::read_stl(const char* filename, 
               std::vector<Point>& points,
               std::vector<std::vector<std::size_t> >& polygons)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);

  if (!in) return 1;

  char header[80] = "";
  char number[4];

  in.read(header, 80);
  in.read(number, 4);
  
  unsigned int triangles = *(unsigned int*) number;
  char pad[2];

  for (std::size_t i = 0; i < triangles; i++) {
    parse_point(in); // normal
    points.push_back(parse_point(in));
    points.push_back(parse_point(in));
    points.push_back(parse_point(in));

    std::vector<std::size_t> polygon { 3*i, 3*i+1, 3*i+2};
    polygons.push_back( polygon );

    in.read(pad, 2);
  }
  
  return 0;
}// read_stl

int
tusk::write_ply(std::vector<Point>& points,
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
    ply_write(ply, CGAL::to_double(points[i].x()));
    ply_write(ply, CGAL::to_double(points[i].y()));
    ply_write(ply, CGAL::to_double(points[i].z()));
  }

  for (std::size_t i = 0; i < nfaces; i++) {
    ply_write(ply, 3);
    ply_write(ply, polygons[i][0]);
    ply_write(ply, polygons[i][1]);
    ply_write(ply, polygons[i][2]);
  } 

  if (!ply_close(ply)) return 1;
  
  return 0;
}// write_ply

int
tusk::write_stl(std::vector<Point>& points,
                std::vector<std::vector<std::size_t> >& polygons,
                const char* filename)
{
  char head[80] = "Source: Metatooth LLC, Format: STL, Type: Binary                               ";
  char pad[2] = "0";
  float zero = 0;
  unsigned int number = polygons.size();

  std::ofstream ofs(filename, std::ios::out | std::ios::binary);

  if (!ofs) return 1;

  ofs.write(head, 80);
  ofs.write((char*)&number, 4);

  for (unsigned int i = 0; i < number; i++) {
    ofs.write((char*)&zero, 4);
    ofs.write((char*)&zero, 4);
    ofs.write((char*)&zero, 4);

    for (unsigned int j = 0; j < 3; j++) {
      float x = CGAL::to_double(points[polygons[i][j]].x());
      float y = CGAL::to_double(points[polygons[i][j]].y());
      float z = CGAL::to_double(points[polygons[i][j]].z());

      ofs.write((char*)&x, 4);
      ofs.write((char*)&y, 4);
      ofs.write((char*)&z, 4);
    }

    ofs.write(pad, 2);
  }
  
  ofs.close();

  return 0;
}// write_stl
