#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }
    map<string, PageParser*> parsers;
    parsers.insert(make_pair("md", new MDParser));
    parsers.insert(make_pair("txt", new TXTParser));
    ifstream index_file(argv[1]);
    string current;
    ofstream out_file;
    out_file.open(argv[2]);
    set<string> processed;
    while (getline(index_file, current)){
    	string ext = extract_extension(current);
    	if (parsers.find(ext) == parsers.end()){
    		throw invalid_argument("Not a valid file extension");
    		return 1;
    	}
    	else{
    		(parsers.find(ext))->second->crawl(parsers, current, processed, out_file);
    	}
    }
    std::map<std::string, PageParser*>::iterator it;
    for (it = parsers.begin(); it != parsers.end(); ++it){
        delete it->second;
    }

    return 0;
}
