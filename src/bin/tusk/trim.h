#ifndef TUSK_TRIM_H
#define TUSK_TRIM_H

#include "app.h"

namespace tusk {

  class Trim: public App {
  public:
    void usage();
    int run(const char* hex, const char* infile, const char* outfile);
  };

}

#endif // TUSK_TRIM_H
