#ifndef TUSK_UTILS_H
#define TUSK_UTILS_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>

#include <fstream>

using K = CGAL::Simple_cartesian<float>;
using Point = K::Point_3;

namespace tusk {

  const double PI = 3.141592653589793238463; //value of pi

  int read_PLY(const char*,
               std::vector<Point>&,
               std::vector<std::vector<std::size_t> >&);
  
  int write_PLY(std::vector<Point>&,
                std::vector<std::vector<std::size_t> >&,
                const char*);
 }

#endif // TUSK_UTILS_H
