#ifndef TUSK_SHOW_H
#define TUSK_SHOW_H

#include "app.h"

namespace tusk {

  class Show: public App {
  public:
    Show();
    void usage();
    int run(const std::string& path);
  };

}

#endif // TUSK_SHOW_H
