#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Nef_polyhedron_3.h>

#include <CGAL/draw_polyhedron.h>

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/extrude.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>

#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>
#include <CGAL/IO/STL_writer.h>

#include "utils.h"

#include <fstream>
#include <algorithm>

using K = CGAL::Simple_cartesian<float>;
using Polyhedron = CGAL::Polyhedron_3<K>;
using Vector_3 = K::Vector_3;
using Point_3 = K::Point_3;
using Aff_transformation_3 = CGAL::Aff_transformation_3<K>;
using Vertex_handle = Polyhedron::Vertex_handle;
using Nef_Polyhedron = CGAL::Nef_polyhedron_3<K>;
using Surface_mesh = CGAL::Surface_mesh<Point_3>;

namespace PMP = CGAL::Polygon_mesh_processing;

double EXTRUDE = 0;
double TARGET_EDGE_LENGTH = 0.04;

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
  
void usage()
{
  std::cerr << "extrude [impression] [extrude] [outfile]\n\n"
            << "  Creates an extruded mesh from an [impression] saving\n"
            << "  to [outfile]. Extrusion will be to [offset].\n\n"
            << "  for example, mouthguard 0000.stl 20 output.stl\n"
            << std::endl;
}

const std::size_t NUM_ARGS = 4;

int main(int argc, char* argv[])
{
  if (argc != NUM_ARGS) {
    usage();
    return 1;
  }

  try {
    Polyhedron impression, base;

    std::cout << "Load impression from " << argv[1] << std::endl;
    std::ifstream input(argv[1]);
    read_polyhedron(input, &impression);
    input.close();
    std::cout << "Done." << std::endl;

    std::cout << "Extruding impression to form base..." << std::endl;
    EXTRUDE = atof(argv[2]);
    std::cout << "Amount: " << EXTRUDE << std::endl;

    typedef typename boost::property_map<Polyhedron, CGAL::vertex_point_t>::type VPMap;
    Bot<VPMap> bot(get(CGAL::vertex_point, base));
    Top<VPMap> top(get(CGAL::vertex_point, base));
    PMP::extrude_mesh(impression, base, bot, top);
    std::cout << "Done." << std::endl;

    std::cout << "Remeshing of " << num_faces(base) << " faces ..." << std::endl;
    PMP::isotropic_remeshing(faces(base),
                             TARGET_EDGE_LENGTH,
                             base);

    std::cout << "Done." << std::endl;

    std::cout << "Write to " << argv[3] << std::endl;
    std::ofstream output(argv[3], std::ios::out | std::ios::binary);
    write_polyhedron(base, output);
    output.close();
    std::cout << "Done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
