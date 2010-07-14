//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: cleanup.hh,v 1.2 2002/09/12 10:15:44 loreti Exp $
//
// -------------------------------------------------------------------

#ifndef CLEANUP_H_
#define CLEANUP_H_

#include <string>
#include "file.hh"

void clean_files(const currDir &);
void nuke(const std::string &, const std::string &);

#endif // CLEANUP_H_
