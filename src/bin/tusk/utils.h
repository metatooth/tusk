#ifndef TUSK_UTILS_H
#define TUSK_UTILS_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_3.h>

#include <fstream>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Simple_cartesian<float> K;
typedef K::Point_3 Point;

namespace tusk {

  const double PI = 3.141592653589793238463; //value of pi

  int read_ply(const char*,
               std::vector<Point>&,
               std::vector<std::vector<std::size_t> >&);
  
  int read_stl(const char*,
               std::vector<Point>&,
               std::vector<std::vector<std::size_t> >&);
  
  int write_ply(std::vector<Point>&,
                std::vector<std::vector<std::size_t> >&,
                const char*);

  int write_stl(std::vector<Point>&,
                std::vector<std::vector<std::size_t> >&,
                const char*);
}

#endif // TUSK_UTILS_H
