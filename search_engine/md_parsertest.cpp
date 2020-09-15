
#include <iostream>
#include <fstream>
#include "md_parser.h"
#include "util.h"

using namespace std;

void printSet(const std::set<std::string>& set);
int main(int argc, char* argv[])
{
    string filename;
    if (argc < 2) {
        cout << "No index file, defaulting to assignmentexample.md" << endl;
        filename = "test-small/assignmentexample.md";
    }
    if (argc >= 2) {
        filename = argv[1];
    }
    std::set<std::string> allSearchableTerms = std::set<std::string>();
    std::set<std::string> allOutgoingLinks = std::set<std::string>();
    MDParser parserBoy = MDParser();
    parserBoy.parse(filename, allSearchableTerms, allOutgoingLinks);
    cout << "TERMS" << endl;
    printSet(allSearchableTerms);
    cout << "LINKS" << endl;
    printSet(allOutgoingLinks);
    return false;
}

void printSet(const std::set<std::string>& set){
    for (std::set<std::string>::const_iterator it = set.begin(); it != set.end(); ++it)
    {
        cout << *it << endl;
    }
}