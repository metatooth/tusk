#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh.h>

#include <fstream>

using K = CGAL::Simple_cartesian<float>;
using Polyhedron = CGAL::Polyhedron_3<K>;
using Point_3 = K::Point_3;

const double PI = 3.141592653589793238463;    //value of pi

int read_PLY(const char*,
             std::vector<Point_3>&,
             std::vector<std::vector<std::size_t> >&);

int write_PLY(std::vector<Point_3>&,
              std::vector<std::vector<std::size_t> >&,
              const char*);
 
void read_polyhedron(std::ifstream& in, Polyhedron* poly);

void write_polyhedron(Polyhedron& poly, std::ofstream& out);

using EK = CGAL::Exact_predicates_inexact_constructions_kernel;
using Mesh = CGAL::Surface_mesh<EK::Point_3>;

void read_mesh(std::ifstream& in, Mesh* mesh);

void write_mesh(Mesh& poly, std::ofstream& out);
