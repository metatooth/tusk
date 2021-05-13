#ifndef TUSK_UTILS_H
#define TUSK_UTILS_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/Surface_mesh.h>


#include <fstream>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_3 Point;
typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Polyhedron;
typedef CGAL::Surface_mesh<Point> Mesh;
typedef Polyhedron::Halfedge_around_vertex_const_circulator HV_circulator;
typedef Polyhedron::Vertex_const_handle V_handle;

namespace tusk {

  const double PI = 3.141592653589793238463; //value of pi
  const double EPSILON = 0.000001;

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

  template <class HDS>
  class ExtrudeBorderAlongY : public CGAL::Modifier_base<HDS> {
  private:
    Polyhedron _body;
    double _offset;
    
  public:
    ExtrudeBorderAlongY(const Polyhedron& body, const double& offset) {
      _body = body;
      _offset = offset;
    }
  
    void operator()( HDS& hds ) {
      std::cout << "Normalize border... " << std::flush;
      _body.normalize_border();
      std::cout << "done." << std::endl;
      
    auto size = std::distance(_body.border_halfedges_begin(),
                              _body.halfedges_end());

    std::cout << "Found " << size << " halfedges." << std::endl;
    
    std::size_t counter = 0;
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(3*2*(size+1), 2*size);
    std::cout << "B.begin_surface(" << 3*2*(size+1) << ", " << 2*size << ")."
              << std::endl;
    
    V_handle active = _body.border_halfedges_begin()->vertex();
    Point prev = active->point();
    
    for (std::size_t i = 0; i < size; ++i) {
      std::size_t n = 0;                                                     
      CGAL::Container_from_circulator<HV_circulator> container(active->vertex_begin());
      for (auto iter = container.begin(); iter != container.end(); ++iter) {
        if (iter->opposite()->is_border()) {
          Point curr = iter->opposite()->vertex()->point();

          Point a(prev);
          Point b(a.x(), a.y() + _offset, a.z());
          
          Point c(curr);
          Point d(c.x(), c.y() + _offset, c.z());

          B.add_vertex(a);      
          B.add_vertex(d);      
          B.add_vertex(b);      
          B.begin_facet();
          B.add_vertex_to_facet(counter++);
          B.add_vertex_to_facet(counter++);
          B.add_vertex_to_facet(counter++);
          B.end_facet();
          
          B.add_vertex(a);      
          B.add_vertex(c);      
          B.add_vertex(d);      
          B.begin_facet();
          B.add_vertex_to_facet(counter++);
          B.add_vertex_to_facet(counter++);
          B.add_vertex_to_facet(counter++);
          B.end_facet();
          
          prev = curr;
          active = iter->opposite()->vertex();

          break;
        }
      }
    }
    
    std::cout << "Processed " << counter << " vertices." << std::endl;

    B.end_surface();
  }
};

}

#endif // TUSK_UTILS_H
