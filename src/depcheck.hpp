#ifndef depcheck
#define depcheck

#include <list>
#include <string>
#include <set>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

/**
 * get all dependencies for a file this will call get_includes
 */
list<path> get_deps(path startfile);

/**
 * get all dependencies for a file; this will be recursive
 * @param file the file whose dependencies will be read
 * @param touched_files a set of all opened files to prevent getting cought by 
 * circular dependencies
 */
list<path> get_includes(path file, set<path>& touched_files);


#endif
