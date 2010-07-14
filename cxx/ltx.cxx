//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     C++ version of the program "lintex", born as an exercise
//     in programming.
//
//     $Id: ltx.cxx,v 1.9 2006/09/11 12:25:41 loreti Exp $
//
// -------------------------------------------------------------------

#include <algorithm>
#include <list>
#include <cstring>
#include "ltx.hh"               // Includes: functional, iostream, string
#include "cleandir.hh"          // Includes: string

extern "C" {
  #include <getopt.h>
  #include <unistd.h>
}

using std::cout;
using std::endl;
using std::string;

// Global variables (definition)

namespace ltx {
  string            progname;
  string            trailEd("~");
  string::size_type lTrailEd;
  bool              confirm(false);
  bool              recurse(false);
}

using namespace ltx;

// Local procedures

namespace {
  char *basename(char *);
  void  syntax();
}

int main(
  int   argc,
  char *argv[]
) {
  std::list<string> targets;

  // Gets the executable name

  progname = argv[0] = basename(argv[0]);

  // Decodes the command line options and arguments

  char          shortOpts[] = "irb::";
  struct option longOpts[]  = {
    {"interactive", no_argument,       0, 'i'},
    {"recursive",   no_argument,       0, 'r'},
    {"backup",      optional_argument, 0, 'b'},
    { 0,            0,                 0,  0}
  };

  int c;
  while ((c = getopt_long(argc, argv, shortOpts, longOpts, 0)) != -1) {
    switch (c) {
      case 'i':
        confirm = true;
        break;

      case 'r':
        recurse = true;
        break;

      case 'b':
        trailEd = optarg ? optarg : "";
        break;

      case 'h':
      case '?':
        syntax();
        return 0;

      default:
        std::cerr << "Can't happen: " << __FILE__ << ' '
                  << __LINE__ << endl;
        return 0;
    }
  }

  while (optind < argc) targets.push_back( argv[optind++] );

  // Computes the length of the extension identifying the backup
  // files, and, if no target directories were explicitly given,
  // scans the current one.

  lTrailEd = trailEd.size();

  if (targets.empty()) targets.push_back(".");

#if defined(DEBUG)
  cout << "--------------------Argument analysis\n";
  cout << "Confirm = " << confirm << endl;
  cout << "Recurse = " << recurse << endl;
  cout << "Trailing editor extension = \"" << trailEd
       << "\" (length " << lTrailEd << ")\n";
  cout << "Target directories:\n";

  for_each(targets.begin(), targets.end(), printBefore("  "));
#endif // DEBUG

  // Scans in turn all the wanted directories

  for_each(targets.begin(), targets.end(), std::ptr_fun(scan_dir));

  return 0;
}

namespace {
  char *basename(
    char *pc
  ) {
    // Strips the (eventual) path name from the full file name pointed
    // to by "pc"; if no file name is given, returns an empty string.

    char *p = std::strrchr(pc, '/');
    if (p == 0) return pc;
    return ++p;
  }

  void syntax()
  {
    cout <<
      "\nUsage:\t " << progname << " [options] [dir [dir ... ]]\n";
    cout <<
      "Purpose: removes TeX auxiliary files and editor backup files from "
      "the\n";
    cout
      << "\t given directories (default: from the current directory).  TeX\n";
    cout <<
      "\t auxiliary files are actually removed only if their modification\n";
    cout <<
      "\t time is more recent than the one of the related TeX source file.\n";
    cout <<
      "Options: -i     | --interactive : asks before removing files;\n";
    cout <<
      "\t -r     | --recursive   : scans recursively the given directories;\n";
    cout <<
      "\t -b=ext | --backup=ext  : \"ext\" is the trailing string "
      "identifying\n";
    cout <<
      "\t\t\t\t  editor backup files.\n";
    cout <<
      "Notes:\t \"ext\" defaults to \"~\"; -b \"\" avoids the unconditional "
      "cleanup of\n";
    cout <<
      "\t any special file.\n" << endl;
  }
}
