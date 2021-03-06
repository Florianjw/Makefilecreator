#include "output.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <cassert>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace logging{

using namespace std;

//in fact, those are like private members:
namespace impl{
	static int verbose_level = 0;
#ifdef DEBUG
	static int debug_level = 0;
#endif
	static int logfile = 0; //filedescriptor
}

void set_verbose_level(int v){
	impl::verbose_level=v;
}

void set_logfile(const string& filename){
	if(impl::logfile){
		int tmp = impl::logfile;
		impl::logfile = 0;
		close(tmp);
	}
	if(!filename.empty()){
		if( (impl::logfile = open(filename.c_str(),O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1){
			impl::logfile = 0;
			error("Could not open logfile.");
		}
		else{
		}
	}
}

#ifdef DEBUG 

void print_debug(string filename, int line, int level, string text){
	if(level<=impl::debug_level){
		string str = "DEBUG(" + to_string(level) + ") [“"  + filename + "”, " + to_string(line) + "]: "
			+ text;
		print_and_log(str);
	}
}

void set_debug_level(int d){
	impl::debug_level=d;
}

#endif

void _note(int level, string text){
	if(level<=impl::verbose_level){
		print_and_log("NOTE: " + text);
	}
}

void _warn(string text){
	print_and_log("WARNING: " + text, false);
}

void _error(string text){
	print_and_log("ERROR: " + text, false);
}

void _fatal(string text){
	print_and_log("FATAL: " + text, false);
	exit(1);
}


string get_timestamp(){
	char buffer[30];
	time_t t = time(NULL);
	
	strftime(buffer, 28, "%x %X", localtime(&t));
	
	string returnstr = buffer;
	return returnstr;
}

void print_and_log(const string& msg, bool normal){
	if( normal ){
		cout << msg << std::endl;
	}
	else{
		cerr << msg << std::endl;
	}
	if( impl::logfile ){
		string str = "[" + get_timestamp() + "] " + msg + '\n';
		if( write( impl::logfile , str.c_str(), str.size()) == -1 ){
			cerr << "Error: Could not write to logfile, Errorcode:" << errno << endl;
		}
	}
}

string _textf_impl(const string& formatstring,const std::vector<string>& strings){
	assert(!strings.empty());
	
	std::ostringstream resultstream;
	std::istringstream formatstream(formatstring);
	string tmp;
	unsigned int unspecified_inserts = 0;
	while(getline(formatstream, tmp, '%')){
		resultstream << tmp;
		if( formatstream.eof() ){
			break;
		}
		switch(formatstream.peek()){
			case '%': 
				resultstream << '%'; 
				formatstream.ignore();
				break;
			case 's':
				try{
					resultstream << strings.at(unspecified_inserts);
					++unspecified_inserts;
					formatstream.ignore();
				}
				catch(std::out_of_range &e){
					throw std::invalid_argument("invalid formatstring");
				}
				break;
			default:
				getline(formatstream, tmp, 's');
				try{
					resultstream << strings.at(static_cast<std::size_t>(stoull(tmp)));
				}
				catch(std::out_of_range &e){
					throw std::invalid_argument("invalid formatstring");
				}
		}
	}
	return resultstream.str();
}

} //namespace logging
