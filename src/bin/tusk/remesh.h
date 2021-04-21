#ifndef TUSK_REMESH_H
#define TUSK_REMESH_H

#include "app.h"

namespace tusk {

  class Remesh: public App {
  public:
    void usage();
    int run(double length, const std::string& in, const std::string& out);
  };

}

#endif // TUSK_REMESH_H
