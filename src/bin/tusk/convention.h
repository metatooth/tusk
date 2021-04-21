#ifndef TUSK_CONVENTION_H
#define TUSK_CONVENTION_H

#include "app.h"

namespace tusk {
  
  class Convention: public App {
  public:
    void usage();
    int run(const char* infile, const char* outfile);
  };
  
}

#endif // TUSK_CONVENTION_H
