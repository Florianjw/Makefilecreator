#ifndef settings_hpp
#define settings_hpp

#include <vector>
#include <string>
#include <map>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <getopt.h>

/*
 * This file contains the settings class and everything that is needed to read
 * settings from a file, save them for the runtime of the programm and recieve
 * the from everywhere
 */

/**
 * commandline-options:
 */
const option OPTIONS[]={
	{"source-dir",required_argument,NULL,'s'},
	{"build-dir",required_argument,NULL,'b'},
	{"target",required_argument,NULL,'t'},
	{"output",required_argument,NULL,'o'},
	{"config-file",required_argument,NULL,'f'},
	{"verbose",required_argument,NULL,'v'},
	{"debug",required_argument,NULL,'d'},
	{"use",required_argument,NULL,'u'},
	{"ignore",required_argument,NULL,'w'},
	{"header",required_argument,NULL,'H'},
	{"header-ending",required_argument,NULL,'e'},
	{"impl-ending",required_argument,NULL,'E'},
	//compiler-options:
	{"compiler",required_argument,NULL,'c'},
	{"compile",no_argument,NULL,'C'},
	{"link",required_argument,NULL,'l'},
	{"link-dir",required_argument,NULL,'L'},
	{"include-dir",required_argument,NULL,'I'},
	{"compiler-opts",required_argument,NULL,'O'},
	{"pkg-libs", required_argument,NULL,'p'},
	{0,0,0,0}
};

/**
 * interface to access and modify the settings easily from everwhere, must be 
 * initiated with settings::init
 */
struct settings{
	/**
	 * Disabled constructor, because there is no reason to create an 
	 * instance
	 */
	settings() = delete;
	
	/**
	 * Init the settings with the commandline-arguments
	 */
	static void init(int argc, char **argv);
	
	/**
	 * Parse the given file as configfile
	 * @param filename name of the configfile
	 */
	static void parse_file(const string& filename);
	
	/**
	 * change the state described by opt with val
	 * @param opt state to be changed
	 * @param val value used to change the state
	 */
	static void set_opt(char opt, const string& val);
	
	/**
	 * List of all header-files
	 */
	static vector<path> header_files;
	/**
	 * List of all code-files
	 */
	static vector<path> implementation_files;
	
	/**
	 * Target of the project
	 */
	static string target;
	/**
	 * place where the makefile should be written
	 */
	static string output;
	
	/**
	 * root-dirs of the source:
	 */
	static vector<path> source_dirs;
	/**
	 * directory for all the object-files:
	 */
	static path build_dir;
	/**
	 * Files that will be completly ignored:
	 */
	static vector<path> ignore_files;
	/**
	 * named sets of settings that can be used with one command
	 */
	static map<string,vector<pair<char, string>>> conditional_settings;
	
	/**
	 * endings of headerfiles
	 */
	static vector<string> header_endings;
	/**
	 * endings of files, that contain the actual code
	 */
	static vector<string> implementation_endings;
	
	//compiler-options:
	
	/**
	 * command that starts the compiler:
	 */
	static string compiler;
	/**
	 * should make be called after the makefile is created?
	 */
	static bool compile;
	
	/**
	 * Directories that contain used header-files
	 */
	static vector<string> include_dirs;
	/**
	 * directories, that contain the libraries
	 */
	static vector<string> lib_dirs;
	/**
	 * libraries to be linked
	 */
	static vector<string> libs;
	/**
	 * all further compiler-options
	 */
	static vector<string> compiler_opts;

	/**
	 * libs for pkg-config
	 */
	static vector<string> pkg_libs;
};



#endif
