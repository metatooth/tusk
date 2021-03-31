#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

#include <fstream>

using K = CGAL::Simple_cartesian<float>;
using Polyhedron = CGAL::Polyhedron_3<K>;
 
void read_polyhedron(std::ifstream& in, Polyhedron* poly);

void write_polyhedron(Polyhedron& poly, std::ofstream& out);
