//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: ltx.hh,v 1.2 2002/09/12 10:15:44 loreti Exp $
//
// -------------------------------------------------------------------

#include <functional>
#include <iostream>
#include <string>

// Auxiliary function object used to print (on an output stream) a
// leading text followed by the argument string and an end-of-line.

class printBefore : public std::unary_function< std::string, void > {
private:
  const std::string  & _leader;
        std::ostream & _os;

public:
  printBefore(const std::string & b = "", std::ostream & o = std::cout)
    : _leader(b), _os(o) {}
  ~printBefore() {}
  void operator() (const std::string & s) { _os << _leader << s << std::endl; }
};

// Global variables (declaration)

namespace ltx {
  extern std::string            progname;
  extern std::string            trailEd;
  extern std::string::size_type lTrailEd;
  extern bool                   confirm;
  extern bool                   recurse;
}
