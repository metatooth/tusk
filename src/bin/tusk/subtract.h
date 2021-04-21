#ifndef TUSK_SUBTRACT_H
#define TUSK_SUBTRACT_H

#include "app.h"

namespace tusk {

  class Subtract: public App {
    public:
    void usage();
    int run(const std::string& afile,
            const std::string& bfile,
            const std::string& outfile);
  };

}

#endif // TUSK_SUBTRACT_H
