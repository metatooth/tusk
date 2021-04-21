#ifndef TUSK_CATALOG_H
#define TUSK_CATALOG_H

#include <string>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>

using K = CGAL::Simple_cartesian<float>;
using Point = K::Point_3;
using Mesh = CGAL::Surface_mesh<Point>;
using Polyhedron = CGAL::Polyhedron_3<K>;

namespace tusk {

  class Catalog {
  public:
    Catalog();

    ~Catalog();

    void read(const std::string& file, Mesh* M);
    void read(const std::string& file, Polyhedron* P);

    void write(const Mesh& M, const std::string& file);
    void write(const Polyhedron& P, const std::string& file);

    private:
    void readSTL(const std::string& file, Mesh* P);
    void readPLY(const std::string& file, Mesh* P);
    void writeSTL(const Mesh& P, const std::string& file);
    void writePLY(const Mesh& P, const std::string& file);
  };
  
}

#endif // TUSK_CATALOG_H
