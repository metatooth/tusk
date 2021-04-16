#include "utils.h"

void subtract_usage()
{
  std::cerr << "subtract [a] [b] [out]\n\n"
            <<"   Boolean subtraction of watertight mesh [b] from [a].\n"
            << "  Result stored in [out] file.\n"
            << "  Files in binary STL format.\n\n"
            << "  for example, subtract a.stl b.stl output.stl\n"
            << std::endl;
}


int subtract(const std::string& afile,
             const std::string& bfile,
             const std::string& outfile)
{
  try {
    Mesh A, B;

    {
      std::cout << "Load mesh A from " << afile << "...";
      std::ifstream ifs(afile.c_str());
      read_mesh(ifs, &A);
      ifs.close();
      std::cout << "done." << std::endl;
    }

    {
      std::cout << "Load mesh B from " << bfile << "...";
      std::ifstream ifs(bfile.c_str());
      read_mesh(ifs, &B);
      ifs.close();
      std::cout << "done." << std::endl;
    }

    Mesh result;

    std::ofstream ofs(outfile.c_str(), std::ios::out | std::ios::binary);
    write_mesh(result, ofs);
    ofs.close();
    std::cout << "done." << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
