#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Nef_polyhedron_3.h>

#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/extrude.h>
#include <CGAL/Polygon_mesh_processing/refine.h>

#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>

#include <CGAL/IO/STL_reader.h>
#include <CGAL/IO/STL_writer.h>

#include <fstream>
#include <algorithm>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polyhedron = CGAL::Polyhedron_3<K>;
using Vector_3 = K::Vector_3;
using Point_3 = K::Point_3;
using Aff_transformation_3 = CGAL::Aff_transformation_3<K>;
using Vertex_handle = Polyhedron::Vertex_handle;
using Nef_Polyhedron = CGAL::Nef_polyhedron_3<K>;
using Surface_mesh = CGAL::Surface_mesh<Point_3>;

namespace PMP = CGAL::Polygon_mesh_processing;

const double EXTRUDE = 20;

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
  
void read_polyhedron_mesh(std::ifstream& in, Polyhedron* out)
{
  std::vector< CGAL::cpp11::array<double,3> > in_points;
  std::vector< CGAL::cpp11::array<size_t,3> > triangles;

  CGAL::read_STL(in, in_points, triangles, false);

  std::vector<K::Point_3> points;
  points.resize(in_points.size());

  for (std::size_t i = 0, l = in_points.size(); i < l; i++) {
    points[i] = K::Point_3(in_points[i][0], in_points[i][1], in_points[i][2]);
  }


  std::vector<std::vector<std::size_t> > polygons;
  polygons.resize(triangles.size());

  for (std::size_t i = 0, l = triangles.size(); i < l; i++) {
    polygons[i] = { triangles[i][0], triangles[i][1], triangles[i][2] };
  } 

  PMP::orient_polygon_soup(points, polygons);
  PMP::polygon_soup_to_polygon_mesh(points, polygons, *out);
}

void write_surface_mesh(Surface_mesh& mesh, std::ofstream& out)
{
  std::cout << "Surface mesh verts " << mesh.number_of_vertices() << std::endl;
  std::cout << "Surface mesh faces " << mesh.number_of_faces() << std::endl;
}

void usage()
{
  std::cerr << "mouthguard impression.stl shell.stl 0.030 1.5 output.stl\n\n"
            << "  Creates a mesh for a mouthguard for [impression] saving\n"
            << "  in STL format to [outfile]. Mouthguard will have a\n"
            << "  [thickness] specified in millimeters with an offset\n"
            << "  of [fit] from the [impression] surface, also in mm.\n\n"
            << "  for example, mouthguard 0000.stl 0000-design.stl"
            << std::endl;
}

const std::size_t NUM_ARGS = 6;

int main(int argc, char* argv[])
{
  if (argc != NUM_ARGS) {
    usage();
    return 1;
  }

  try {
    Polyhedron impression, base, shell;

    std::cout << "Load impression from " << argv[1] << std::endl;
    std::ifstream input(argv[1]);
    read_polyhedron_mesh(input, &impression);
    input.close();
    std::cout << "Done." << std::endl;

    std::cout << "Load shell from " << argv[2] << std::endl;
    input.open(argv[2]);
    read_polyhedron_mesh(input, &shell);
    input.close();
    std::cout << "Done." << std::endl;

    std::cout << "Extruding impression to form base..." << std::endl;
    typedef typename boost::property_map<Polyhedron, CGAL::vertex_point_t>::type VPMap;
    Bot<VPMap> bot(get(CGAL::vertex_point, base));
    Top<VPMap> top(get(CGAL::vertex_point, base));
    PMP::extrude_mesh(impression, base, bot, top);
    std::cout << "Done." << std::endl;
    
    if (base.is_closed() && shell.is_closed()) {
      std::cout << "Make Nef Poly 1..." << std::endl;
      Nef_Polyhedron N1(shell);
      std::cout << "Make Nef Poly 2..." << std::endl;
      Nef_Polyhedron N2(base);
      std::cout << "Done." << std::endl;

      double offset = std::stof(argv[3]);
      double step = std::stof(argv[4]);
 
      Aff_transformation_3 aff(CGAL::TRANSLATION, Vector_3(0,offset,0));
      N2.transform(aff);

      while (offset >= 0) {
        std::cout << "Subtracting at... " << offset << std::endl;
        N1 -= N2;
        std::cout << "Done." << std::endl;
       
        std::cout << "Convert to surface mesh..." << std::endl;
        Surface_mesh M;
        CGAL::convert_nef_polyhedron_to_polygon_mesh(N1, M); 
        std::cout << "Done." << std::endl;
        
        std::string filename("step-" + std::to_string(offset) + ".stl");
        std::cout << "Write to " << filename << std::endl;
        std::ofstream ofs(filename);
        write_surface_mesh(M, ofs);
        ofs.close();
        std::cout << "Done." << std::endl;
                
        offset -= step;
        
        Aff_transformation_3 aff(CGAL::TRANSLATION, Vector_3(0,-step,0));
        N2.transform(aff);
      }

    }

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
