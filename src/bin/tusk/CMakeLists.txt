project("tusk")

include_directories("../../lib")

create_single_source_cgal_program(tusk.cpp
  utils.cpp
  extrude.cpp
  normalize.cpp
  remesh.cpp)
    
set_property(TARGET tusk PROPERTY CXX_CLANG_TIDY clang-tidy;--checks=-isystem)

set_property(TARGET tusk PROPERTY CXX_CLANG_FORMAT clang-format)

target_link_libraries(tusk PUBLIC CGAL::CGAL_Qt5 rply)
