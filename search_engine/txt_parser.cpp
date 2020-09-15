#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "txt_parser.h"
#include "util.h"

using namespace std;


void TXTParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    allSearchableTerms.clear();
    allOutgoingLinks.clear();
    string term = "";
    string link = "";
    char c = wfile.get();
    while(!wfile.fail())
    {
        if (!isalnum(c))
        {
            if(term != "")
            {
                term = conv_to_lower(term);
                allSearchableTerms.insert(term);
            }
            term = "";
        }
        else
        {
            term += c;
        }
        c = wfile.get();
    }
    if(term != "")
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }
    wfile.close();
}

std::string TXTParser::display_text(std::string filename)
{
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    char c = wfile.get();
    while (!wfile.fail()) {
        retval += c;
        c = wfile.get();
    }
    return retval;
}


void TXTParser::crawl( const std::map<std::string, PageParser*>& parsers,
    std::string filename, std::set<std::string>& processed, std::ostream& ofile){

    if (processed.find(filename) == processed.end()){
        processed.insert(filename);
        ofile << filename << endl;
    }
    else{
        return;
    } 
}




