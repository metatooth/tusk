#ifndef TUSK_CATALOG_H
#define TUSK_CATALOG_H

#include "utils.h"

#include <string>

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
