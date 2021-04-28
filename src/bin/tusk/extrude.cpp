#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

#include <CGAL/Polygon_mesh_processing/extrude.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include "catalog.h"
#include "extrude.h" // class implemented

#include <fstream>
#include <algorithm>

using Vector_3 = K::Vector_3;

namespace PMP = CGAL::Polygon_mesh_processing;

using namespace tusk;

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
  
void
Extrude::usage()
{
  std::cerr << "tusk extrude [y-value] [infile] [outfile]\n\n"
            << "  Extrusion will be along the y-axis to [y-value].\n"
            << "  Y-value in millimeters.\n"
            << "  Creates an extruded mesh from [infile] saving\n"
            << "  to [outfile]. Input in binary PLY, output in binary STL.\n\n"
            << "  for example, tusk extrude 20 0000.ply model-0000.stl\n"
            << std::endl;
}// usage

int
Extrude::run(double offset, const char* infile, const char* outfile)
{
  try {
    std::vector<Point> points;
    std::vector<std::vector<size_t> > polygons;

    Catalog catalog;
    Polyhedron impression, model;

    std::cout << "Load impression from " << infile << std::endl;
    catalog.read(infile, &impression);
    std::cout << "Done." << std::endl;

    EXTRUDE = offset;
    std::cout << "Extruding mesh to y=" << EXTRUDE << " ..." << std::endl;
    typedef typename boost::property_map<Polyhedron, CGAL::vertex_point_t>::type VPMap;
    Bot<VPMap> bot(get(CGAL::vertex_point, model));
    Top<VPMap> top(get(CGAL::vertex_point, model));
    PMP::extrude_mesh(impression, model, bot, top);
    std::cout << "Done." << std::endl;

    std::cout << "Write to " << outfile << std::endl;
    catalog.write(model, outfile);
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
