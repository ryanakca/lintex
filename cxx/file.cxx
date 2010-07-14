//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: file.cxx,v 1.7 2002/09/12 10:15:44 loreti Exp $
//
// -------------------------------------------------------------------

#include <algorithm>
#include <functional>
#include <iostream>
#include "file.hh"              // Includes: list, map, string, utility, ctime

using std::string;

// Methods for the class fileFamily

void fileFamily::addExtension(
  time_t         mTime,
  const string * pExtName
) {
  // Insert the file informations in a fileFamily.  "mTime" is the
  // modification time, "pExtName" points to the file extension or is
  // NULL for an extension of ".tex" .

  if (pExtName) {
    _extInfo.push_back( std::make_pair(*pExtName, mTime) );
  } else {
    _hasTex   = true;
    _texMtime = mTime;
  }
}

// Auxiliary function object for currDir objects

struct releaseFileFamily
  : public std::unary_function<fileCollectionElement, void> {
  void operator() (fileCollectionElement & p) {
    delete p.second; }
};

// Methods for the class currDir

currDir::~currDir()
{
  for_each(_dirContent.begin(), _dirContent.end(), releaseFileFamily());
}

fileFamily & currDir::getFileFamily(
  const string & base
) {
  // Gets the file family related to the basename "base".  If this is
  // the first file found, a fileFamily instance is allocated with
  // "operator new" (and will be later deleted in the currDir class
  // destructor).

  fileFamily * retval = _dirContent[ base ];

  if (retval == 0) {
    retval              = new fileFamily;
    _dirContent[ base ] = retval;
  }

  return *retval;
}
