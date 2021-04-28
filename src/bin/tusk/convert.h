#ifndef TUSK_CONVERT_H
#define TUSK_CONVERT_H

#include "app.h"

namespace tusk {
  
  class Convert: public App {
  public:
    void usage();
    int run(const char* infile, const char* outfile);
  };
  
}

#endif // TUSK_CONVERT_H
