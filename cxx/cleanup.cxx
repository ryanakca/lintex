//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: cleanup.cxx,v 1.8 2005/01/21 08:16:49 loreti Exp $
//
// -------------------------------------------------------------------

#include <cstdio>
#include <cctype>
#include "ltx.hh"               // Includes: functional, iostream, string
#include "file.hh"              // Includes: list, map, string, utility, ctime
#include "cleanup.hh"           // Includes: string

using std::cin;
using std::cout;
using std::string;

namespace {
  const int answerLength(64);
}

void clean_files(
  const currDir & dir
) {
  // Loops over all the file families stored in "dir", then loops over
  // all the extensions in this file family; if a ".tex" file with a
  // modification time former than the modification time of the target
  // file exists, the file is removed.

  fileCollection::const_iterator iter, iterEnd = dir.end();

  for (iter = dir.begin();  iter != iterEnd;  iter++) {

    const fileFamily                   * pFF = iter->second;
    std::list<extInfo>::const_iterator   jter;
    std::list<extInfo>::const_iterator   jterEnd = pFF->end();

    for (jter = pFF->begin();  jter != jterEnd;  jter++) {

      string fullName = iter->first + jter->first;

      if (pFF->hasTex()) {
        if (difftime(jter->second, pFF->texMtime()) > 0.0) {

          if (ltx::confirm) {
            char answer[answerLength], c;

            do {
              cout << "Remove " << dir.getName()
                   << fullName << " (y|n) ? ";
              cin.get(answer, answerLength);
              if (cin.gcount() < answerLength-1 ) {
                cin.ignore();
              }
              c = tolower(static_cast<unsigned char>(answer[0]));
            } while (c != 'y'  &&  c != 'n');
            if (c != 'y') continue;
          }
          nuke(dir.getName(), fullName);

        } else {
          cout << dir.getName() << fullName
               << " not removed; " << iter->first
               << ".tex is newer\n";
        }
      } else {
        cout << dir.getName() << fullName
             << " not removed; " << iter->first
             << ".tex does not exist\n";
      }
    }
  }
}

void nuke(
  const string & dirName,
  const string & fileName
) {
  // Removes the file "fileName" from the directory "dirName".  If the
  // preprocessor symbol 'DEBUG' is defined, the file is not actually
  // removed: but a message is printed on the standard output stream,
  // informing that the Finger Of Death has been raised to him.

  string target = dirName + fileName;

#if defined(DEBUG)
  cout << "FOD: " << target << std::endl;
#else
  remove(target.c_str());
  cout << target << " has been removed.\n";
#endif // DEBUG
}
