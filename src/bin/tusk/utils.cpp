#include "utils.h" // class implemented

#include <CGAL/IO/STL_reader.h>

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
tusk::read_PLY(const char* filename, 
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
}// read_PLY

int
tusk::write_PLY(std::vector<Point>& points,
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
}// write_PLY
