#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"
#include <stack>

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;


void MDParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    ifstream wfile(filename.c_str());
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::parse()");
    }
    allSearchableTerms.clear();
    allOutgoingLinks.clear();
    PARSE_STATE_T state = NORMALTEXT;
    string term = "";
    string link = "";
    char c = wfile.get();
    while(!wfile.fail())
    {
        if(state == NORMALTEXT)
        {
             if (!isalnum(c)){
                if(term != ""){
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                }
                if (c == '['){
                    state = LINKTEXT;
                }
                term = "";
            }
            else{
                term += c;
            }


        }
        else if (state == LINKTEXT)
        {
            if (!isalnum(c)){
                if(term != ""){
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                }
                if (c == ']'){
                    state = ISLINK;
                }
                term = "";
            }
                
            else{
                term += c;
            }
        }
        else if( state == ISLINK )
        {
            if (c == '('){
                state = LINKURL;
            }
            else{
                state = NORMALTEXT;
            }
        }
        else
        {
            if (c == ')'){
                allOutgoingLinks.insert(link);
                link = "";
                state = NORMALTEXT;
            }
            else{
                link += c;
            }

        }
        c = wfile.get();
    }
    if (term != ""){
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }
    wfile.close();
}

std::string MDParser::display_text(std::string filename)
{
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;
    PARSE_STATE_T state = NORMALTEXT;

    char c = wfile.get();
    while (!wfile.fail()) {
        if (state == NORMALTEXT)
        {
            if (c == '[')
            {
                state = LINKTEXT;
            }
            retval += c;
        }
        else if (state == LINKTEXT)
        {
            if (c == ']')
            {
                state = ISLINK;
            }
            retval += c;
        }
        else if (state == ISLINK) {
            if (c == '(') {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
                retval += c;
            }
        }
        else
        {
            if (c == ')')
            {
                state = NORMALTEXT;
            }
        }
        c = wfile.get();
    }
    return retval;
}


void MDParser::crawl( const std::map<std::string, PageParser*>& parsers,
    std::string filename, std::set<std::string>& processed, std::ostream& ofile){
    if (processed.find(filename) != processed.end()){
        return;
    } 

    ifstream wfile(filename);
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::parse()");
    }
    wfile.close();
    stack<string> files_to_be_processed;
    files_to_be_processed.push(filename);
    while (!files_to_be_processed.empty()){
        string current_file = files_to_be_processed.top();
        string current_ext = extract_extension(current_file);
        files_to_be_processed.pop();
        if (parsers.find(current_ext) == parsers.end()){
            continue;
        }
        else if (parsers.find(current_ext) != parsers.find("md")){
            parsers.find("txt")->second->crawl(parsers, current_file, processed, ofile);
            continue;
        }
        set<string> allSearchableTerms; 
        set<std::string> allOutgoingLinks;
        parse(current_file, allSearchableTerms, allOutgoingLinks);
        if (!allOutgoingLinks.empty()){
            set<string>::reverse_iterator it = allOutgoingLinks.rbegin();
            for (it = it; it != allOutgoingLinks.rend(); ++it){
 
                string file_name = *it;
                if (processed.find(file_name) == processed.end()){
                    files_to_be_processed.push(file_name);
                }
            }
        }
        processed.insert(current_file);
        ofile << current_file << endl;
    }  
}


