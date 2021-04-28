#ifndef TUSK_UNION_H
#define TUSK_UNION_H

#include "app.h"

namespace tusk {

  class Union: public App {
    public:
    void usage();
    int run(const std::string& afile,
            const std::string& bfile,
            const std::string& outfile);
  };

}

#endif // TUSK_UNION_H
