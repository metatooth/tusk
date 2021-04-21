#ifndef TUSK_NORMALIZE_H
#define TUSK_NORMALIZE_H

#include "app.h"

namespace tusk {
 
  class Normalize: public App {
  public:
    void usage();
    int run(const char* infile, const char* outfile);
  };

}

#endif // TUSK_NORMALIZE_H

