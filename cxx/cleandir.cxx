//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: cleandir.cxx,v 1.11 2002/09/25 12:19:41 loreti Exp $
//
// -------------------------------------------------------------------

#include <algorithm>
#include <iterator>
#include <vector>
#include <cstring>
#include "ltx.hh"               // Includes: functional, iostream, string
#include "cleandir.hh"          // Includes: string
#include "cleanup.hh"           // Includes: string
#include "file.hh"              // Includes: list, map, string, utility, ctime

extern "C" {
  #include <dirent.h>
  #include <sys/stat.h>
  #include <sys/types.h>
}

using std::cerr;
using std::cout;
using std::strcmp;
using std::string;

// Local variables

namespace {
  // In "re" are defined the extensions of the files relevant for
  // LaTeX; ".tex" is missing, being handled separately.  THIS ARRAY
  // MUST BE SORTED IN ASCENDING ORDER.

  const char * re[] = {
    ".aux", ".dvi", ".idx", ".ilg", ".ind",
    ".lof", ".log", ".lot", ".pdf", ".ps",
    ".toc"
  };
  const size_t nRE = sizeof(re) / sizeof(re[0]);
  std::vector<string> texExts(re, re + nRE);

  const string tex(".tex");
  const string dot(".");

  const size_t filename_max = 256;
}

// Local functions (declarations)

namespace {
  void check_file(const string &, const time_t, currDir &);
}

// Code

void scan_dir(
  const string & name
) {
  // Scans the directory "name", building the related instantiation of
  // the class "currDir" containing all the informations for the
  // relevant files; then calls "clean_dir" to perform the actual
  // cleanup.  If the "-r" options has been specified, recurses over
  // all the directories under the current one.

#if defined(DEBUG)
  static bool firstTime(true);

  if (firstTime) {
    cout << "--------------------Relevant extensions ("
         << nRE << ")\n";
    copy(texExts.begin(), texExts.end(),
         std::ostream_iterator<string>(cout, " "));
    cout << std::endl;
    firstTime = false;
  }

  cout << "--------------------scan_dir called for \""
       << name << "\"\n";
#endif // DEBUG

  DIR * pDir;

  if ((pDir = opendir( name.c_str() )) != 0) {

    string fullName(name);
    if (*(fullName.rbegin()) != '/') fullName.append("/");

    currDir             thisDir(fullName);
    std::list<string>   subDirs;
    struct dirent     * pDe;

    // Reads every file: skips null inodes (already deleted
    // files), and the two special files "." and ".." .

    while ((pDe = readdir(pDir)) != 0) {
      if (pDe->d_ino == 0) continue;

#if defined(DEBUG)
      cout << "Next file: " << pDe->d_name << " - ";
#endif // DEBUG

      if (strcmp(pDe->d_name, ".")  == 0) {
#if defined(DEBUG)
        cout << "skipped\n";
#endif // DEBUG
        continue;
      }

      if (strcmp(pDe->d_name, "..") == 0) {
#if defined(DEBUG)
        cout << "skipped\n";
#endif // DEBUG
        continue;
      }

      // Gets the file related informations with stat(2) (we need file
      // type and modification time).  If the call to "stat" fails,
      // the file is not considered.

      char tName[filename_max];
      std::strcpy(tName, fullName.c_str());
      std::strcat(tName, pDe->d_name);

      struct stat sStat;

      if (stat(tName, &sStat) != 0) {
#if defined(DEBUG)
        cout << "got error from stat()\n";
#else
        cerr << ltx::progname << ": error calling stat("
             << tName << ")\n";
#endif // DEBUG

      } else {
        if (S_ISDIR(sStat.st_mode) != 0) {
#if defined(DEBUG)
          cout << "is a directory\n";
#endif // DEBUG

          // If needed, push the subdirectory names in the dedicated
          // list, for future recursion; plain files are handled by
          // the local procedure check_file().

          if (ltx::recurse) subDirs.push_back(tName);

        } else {
          check_file(pDe->d_name, sStat.st_mtime, thisDir);
        }
      }
    }

    // Looks if some cleanup has to be performed

    clean_files(thisDir);

    if (ltx::recurse) {
      for_each(subDirs.begin(), subDirs.end(), std::ptr_fun(scan_dir));
    }

    closedir(pDir);
  } else {
    cerr << ltx::progname << ": \"" << name
         << "\" could not be opened (or is not a directory)\n";
  }
}

namespace {
  void check_file(
    const string & name,
    const time_t   mTime,
    currDir      & CDir
  ) {
    // - If the file "name" matches the trailing string identifying
    //   backup editor files, is removed;
    // - if it matches a relevant extension, is inserted in the
    //   "currDir" instance.

    string::size_type where;

    if (ltx::lTrailEd > 0) {
      if ((where = name.rfind(ltx::trailEd)) != string::npos) {
        if ((where + ltx::lTrailEd) == name.size()) {
  #if defined(DEBUG)
          cout << "matches the default editor extension\n";
  #endif // DEBUG
          nuke(CDir.getName(), name);
          return;
        }
      }
    }

    // Breaks the file name in "basename" and "extension"; and checks
    // the latter against the known ones (".tex" files are handled
    // separately).

    if ((where = name.find_last_of(dot)) != string::npos) {
      string       extension = name.substr(where);
      string       basename  = name.substr(0, where);
      fileFamily & fF        = CDir.getFileFamily(basename);

      if (extension == tex) {
        fF.addExtension(mTime, 0);
  #if defined(DEBUG)
        cout << "inserted\n";
  #endif // DEBUG

      } else if (binary_search(texExts.begin(), texExts.end(), extension)) {
        fF.addExtension(mTime, &extension);
  #if defined(DEBUG)
        cout << "extension " << extension << " - inserted\n";
      } else {
        cout << "extension not relevant\n";
  #endif // DEBUG

      }

  #if defined(DEBUG)
    } else {
      cout << "no extension\n";
  #endif // DEBUG
    }
  }
}
