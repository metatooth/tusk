#ifndef TUSK_CLIP_H
#define TUSK_CLIP_H

#include "app.h"

namespace tusk {

  class Clip: public App {
  public:
    void usage();
    int run(double y, const char* infile, const char* outfile);
  };

}

#endif // TUSK_CLIP_H
