//     Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
//     Work:   University of Padova - Department of Physics
//             Via F. Marzolo, 8 - 35131 PADOVA - Italy
//     Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
//     EMail:  loreti@pd.infn.it
//     WWW:    http://www.pd.infn.it/~loreti/mlo.html
//
// -------------------------------------------------------------------
//
//     $Id: file.hh,v 1.3 2002/09/12 10:15:44 loreti Exp $
//
// -------------------------------------------------------------------

#ifndef FILE_H_
#define FILE_H_

#include <list>
#include <map>
#include <string>
#include <utility>
#include <ctime>

// Classes for the handling of directories and files.
//
// - The files are abstracted as a basename, an extension and a
//   modification time: the basename being defined as all the file
//   name characters up to (but not including) the last "."; and the
//   extension as the remainder.  A file may have an empty basename or
//   an empty extension.
//
// - A "file family" is a set of files having all the same basename
//   and different extensions.  In this context, an extension of
//   ".tex" is considered 'special' and is managed separately; the
//   class "fileFamily" actually contains informations about the
//   existence of a .tex member and its modification time, plus a list
//   of all the related files with different extensions.
//
// - For the file families, methods are provided to test for the
//   existence of a .tex; to get its modification time; to add a
//   member to the family; and to retrieve the list of all the
//   extensions found.

typedef std::pair< std::string, time_t > extInfo;

class fileFamily {
private:
  bool               _hasTex;
  time_t             _texMtime;
  std::list<extInfo> _extInfo;

  // Prevents any use of the copy constructor and of the assignment
  // operator

  fileFamily & operator = (const fileFamily & rhs);
  fileFamily(const fileFamily & rhs);

public:
  fileFamily() : _hasTex(false), _texMtime(0) {}
  ~fileFamily() {}

  bool   hasTex()   const { return _hasTex;   }
  time_t texMtime() const { return _texMtime; }

  void addExtension(time_t, const std::string *);

  // Const-iterators over all the found extensions

  std::list<extInfo>::const_iterator begin() const {
    return _extInfo.begin(); }
  std::list<extInfo>::const_iterator end() const {
    return _extInfo.end(); }
};

// A directory is seen as a directory name plus a collection of file
// families; that collection is implemented as an STL map.  Methods
// are provided to add a file, to retrieve the directory name, and to
// iterate over the file families.

typedef std::pair< const std::string, fileFamily * > fileCollectionElement;
typedef std::map< const std::string, fileFamily * >  fileCollection;

class currDir {
private:
  std::string    _name;
  fileCollection _dirContent;

  // Prevents any use of the copy constructor and of the assignment
  // operator

  currDir & operator = (const currDir & rhs);
  currDir(const currDir & rhs);

public:
  currDir(const std::string & dirName) : _name(dirName) { }
  ~currDir();

  const std::string & getName() const { return _name; }
  fileFamily & getFileFamily(const std::string &);

  // Iterators over all the found file families

  fileCollection::const_iterator begin() const {
    return _dirContent.begin(); }
  fileCollection::const_iterator end() const {
    return _dirContent.end(); };
};

#endif // FILE_H_
