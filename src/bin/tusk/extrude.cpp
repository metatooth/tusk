#include <CGAL/circulator.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include "catalog.h"
#include "extrude.h" // class implemented

typedef Polyhedron::HalfedgeDS HalfedgeDS;
typedef Polyhedron::Halfedge_around_facet_const_circulator HF_circulator;

namespace PMP = CGAL::Polygon_mesh_processing;

using namespace tusk;

void
Extrude::usage()
{
  std::cerr << "tusk extrude [y-value] [infile] [outfile]\n\n"
            << "  Extrusion will be along the y-axis to [y-value].\n"
            << "  Y-value in millimeters.\n"
            << "  Creates an extruded mesh from [infile] saving\n"
            << "  to [outfile]. File format by extension (stl|ply).\n\n"
            << "  for example, tusk extrude 20 0000.ply 0000-model.ply\n"
            << std::endl;
}// usage

int
Extrude::run(double offset, const char* infile, const char* outfile)
{
  try {
    Catalog catalog;
    Polyhedron A;

    std::cout << "Load polyhedron from " << infile << "... " << std::flush;
    catalog.read(infile, &A);
    std::cout << "done." << std::endl;

    Polyhedron B;
    ExtrudeBorderAlongY<HalfedgeDS> extrude(A, offset);
    B.delegate(extrude);

    std::vector<Point> points;
    std::vector<std::vector<std::size_t> > polygons;

    std::size_t counter = 0;
    for (auto iter = A.facets_begin(); iter != A.facets_end(); ++iter) {
      CGAL::Container_from_circulator<HF_circulator> container(iter->facet_begin());
      std::vector<std::size_t> polygon;
      for (auto jter = container.begin(); jter != container.end(); ++jter) {
        points.push_back(jter->vertex()->point());
        polygon.push_back(counter++);
      }

      polygons.push_back(polygon);
    }

    for (auto iter = B.facets_begin(); iter != B.facets_end(); ++iter) {
      CGAL::Container_from_circulator<HF_circulator> container(iter->facet_begin());
      std::vector<std::size_t> polygon;
      for (auto jter = container.begin(); jter != container.end(); ++jter) {
        points.push_back(jter->vertex()->point());
        polygon.push_back(counter++);
      }

      polygons.push_back(polygon);
    }

    Polyhedron R;
    PMP::repair_polygon_soup(points, polygons);
    PMP::orient_polygon_soup(points, polygons);
    PMP::polygon_soup_to_polygon_mesh(points, polygons, R);

    std::cout << "Write to " << outfile << "... " << std::flush;
    catalog.write(R, outfile);
    std::cout << "done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}// run
