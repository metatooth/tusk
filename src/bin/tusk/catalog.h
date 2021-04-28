#ifndef TUSK_CATALOG_H
#define TUSK_CATALOG_H

#include "utils.h"

#include <string>

#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>

typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef CGAL::Surface_mesh<Point> Mesh;

namespace tusk {

  class Catalog {
  public:
    Catalog();

    ~Catalog();

    void read(const std::string& file, Mesh* M);
    void read(const std::string& file, Polyhedron* P);

    void write(const Mesh& M, const std::string& file);
    void write(const Polyhedron& P, const std::string& file);
  };
  
}

#endif // TUSK_CATALOG_H
