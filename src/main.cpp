#include <iostream>
#include <list>
#include <string>
#include <map>
#include <cstdlib>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "treewalker.hpp"
#include "depcheck.hpp"
#include "settings.hpp"
#include "mf.hpp"
#include "tools.hpp"
#include "output.hpp"
#include "help.hpp"


int main(int argc, char **argv){
	if(argc == 2 && ( argv[1] == string("-h") || argv[1] == string("--help") ) ){
		print_help(argv[0]);
		return 0;
	}
	settings::init(argc, argv);
	debug(1, "settings read");
	get_code_files();
	debug(1, "got list of code-files");
	
	map<path,list<path>> dependencies;
	debug(1, "starting to read dependencies");
	for(auto x : settings::implementation_files){
		debug(2, "reading dependencies for "+x.string());
		dependencies[x]=get_deps(x);
		debug(4, "done reading dependencies for "+x.string());
	}
	debug(1, "done reading all dependencies");
	debug(1, "creating makefile");
	create_makefile(dependencies);
	if(settings::compile){
		debug(1, "compiling");
		system("make");
	}
	
	return 0;
}
