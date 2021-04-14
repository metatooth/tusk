#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/Polygon_mesh_processing/extrude.h>

#include "utils.h"

#include <fstream>
#include <algorithm>

using K = CGAL::Simple_cartesian<float>;

using Polyhedron = CGAL::Polyhedron_3<K>;
using Vector_3 = K::Vector_3;

namespace PMP = CGAL::Polygon_mesh_processing;

double EXTRUDE = 0;

template<typename MAP>
struct Bot {
  Bot(MAP map) : map(map) { }

  template<typename T, typename VD>
  void operator() (const T&, VD vd) const
  {
    boost::put(map, vd, boost::get(map, vd));
  }

  MAP map;
};

template<typename MAP>
struct Top {
  Top(MAP map) : map(map) { }

  template<typename T, typename VD>
  void operator() (const T&, VD vd) const
  {
    auto v = boost::get(map, vd); 
    boost::put(map, vd, v+Vector_3(0,EXTRUDE-v.y(),0));
  }

  MAP map;
};
  
void extrude_usage()
{
  std::cerr << "tusk extrude [y-value] [infile] [outfile]\n\n"
            << "  Extrusion will be along the y-axis to [y-value].\n"
            << "  Y-value in millimeters.\n"
            << "  Creates an extruded mesh from [infile] saving\n"
            << "  to [outfile]. Files to be in binary STL format.\n\n"
            << "  for example, tusk extrude 20 0000.stl output.stl\n"
            << std::endl;
}

int extrude(double offset, const char* infile, const char* outfile)
{
  try {
    Polyhedron impression, base;

    std::cout << "Load impression from " << infile << std::endl;
    std::ifstream input(infile);
    read_polyhedron(input, &impression);
    input.close();
    std::cout << "Done." << std::endl;

    EXTRUDE = offset;
    std::cout << "Extruding mesh to y=" << EXTRUDE << " ..." << std::endl;

    typedef typename boost::property_map<Polyhedron, CGAL::vertex_point_t>::type VPMap;
    Bot<VPMap> bot(get(CGAL::vertex_point, base));
    Top<VPMap> top(get(CGAL::vertex_point, base));
    PMP::extrude_mesh(impression, base, bot, top);
    std::cout << "Done." << std::endl;

    std::cout << "Write to " << outfile << std::endl;
    std::ofstream output(outfile, std::ios::out | std::ios::binary);
    write_polyhedron(base, output);
    output.close();
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
