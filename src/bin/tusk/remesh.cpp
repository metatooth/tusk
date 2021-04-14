#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>

#include "utils.h"

#include <boost/function_output_iterator.hpp>
#include <fstream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EK;
typedef CGAL::Surface_mesh<EK::Point_3> Mesh;

typedef boost::graph_traits<Mesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Mesh>::edge_descriptor     edge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

struct halfedge2edge
{
  halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges)
    : m_mesh(m), m_edges(edges)
  {}
  void operator()(const halfedge_descriptor& h) const
  {
    m_edges.push_back(edge(h, m_mesh));
  }
  const Mesh& m_mesh;
  std::vector<edge_descriptor>& m_edges;
};

void remesh_usage()
{
  std::cerr << "remesh [target_edge_length] [infile] [outfile]\n\n"
            <<"   [target_edge_length] in millimters.\n"
            << "  Isotropic remeshing of [infile] saving to [outfile].\n"
            << "  Files in binary STL format.\n\n"
            << "  for example, remesh 0.04 0000.stl output.stl\n"
            << std::endl;
}


int remesh(double length, const std::string& infile, const std::string& outfile)
{
  try {
    Mesh mesh;

    std::cout << "Load mesh from " << infile << "...";
    std::ifstream ifs(infile.c_str());
    read_mesh(ifs, &mesh);
    ifs.close();
    std::cout << "done." << std::endl;

    unsigned int nb_iter = 3;
    std::cout << "Split border...";
    std::vector<edge_descriptor> border;
    PMP::border_halfedges(faces(mesh),
                          mesh,
                          boost::make_function_output_iterator(halfedge2edge(mesh, border)));
    PMP::split_long_edges(border, length, mesh);
    std::cout << "done." << std::endl;

    std::cout << "Remeshing (" << num_faces(mesh) << " faces) by target edge length " << length << "mm ...";
    PMP::isotropic_remeshing(faces(mesh), length, mesh, PMP::parameters::number_of_iterations(nb_iter));
    std::cout << "done." << std::endl;

    std::cout << "Write to " << outfile << "...";
    std::ofstream ofs(outfile.c_str(), std::ios::out | std::ios::binary);
    write_mesh(mesh, ofs);
    ofs.close();
    std::cout << "done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
