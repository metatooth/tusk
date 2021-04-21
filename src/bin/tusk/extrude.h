#ifndef TUSK_EXTRUDE_H
#define TUSK_EXTRUDE_H

#include "app.h"

namespace tusk {

  class Extrude: public App {
  public:
    void usage();
    int run(double offset, const char* infile, const char* outfile);
  };

}

#endif // TUSK_EXTRUDE_H
