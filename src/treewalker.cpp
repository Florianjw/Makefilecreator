#include "treewalker.hpp"

#include <algorithm>
using namespace std;

#include "tools.hpp"
#include "output.hpp"
#include "settings.hpp"



filetype analyse_file(path file){
	if(!is_regular_file(file)){
		return OTHER;
	}
	string filename=file.filename().string();
	string filename_extension=cut(filename,".").back();
	if(in(filename_extension,settings::header_endings)){
		return HEADER;
	}
	else if(in(filename_extension,settings::implementation_endings)){
		return IMPL;
	}
	else return OTHER;
}

void get_code_files(){
	for(auto source_dir: settings::source_dirs){
		recursive_directory_iterator it(source_dir);
		while(it!=recursive_directory_iterator()){
			debug(3, "Checking filetype of "+path(*it).string());
			if(find(settings::ignore_files.begin(),
				settings::ignore_files.end(),*it)
				!= settings::ignore_files.end()
			){
				debug(2, "ignored "+path(*it).string());
				++it;
				continue;
			}
			switch(analyse_file(*it)){
				case HEADER:
					settings::header_files.push_back(clean_path(*it));
					break;
				case IMPL:
					settings::implementation_files.push_back(clean_path(*it));
					break;
				case OTHER:
					break;
			}
			++it;
		}
	}
}
