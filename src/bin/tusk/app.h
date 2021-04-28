#ifndef TUSK_APP_H
#define TUSK_APP_H

namespace tusk {

  class App {
  public:
    App() { _type = "mesh"; };
    ~App() { };
    void usage() { };
    int run() { return 0; };
    const std::string& type() const { return _type; };
    void type(const std::string& type) { _type = type; };
  private:
    std::string _type;
  };
  
}

#endif // TUSK_APP_H
