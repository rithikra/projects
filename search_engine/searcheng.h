#ifndef SEARCHENG_H
#define SEARCHENG_H

#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "pageparser.h"
#include "wpscombiner.h"







class SearchEng {
public:
    
    SearchEng(PageParser* noExtensionParser);

    
    ~SearchEng();

    
    void register_parser(const std::string& extension, PageParser* parser);

    

    void read_pages_from_index(const std::string& index_file);

    
    WebPage* retrieve_page(const std::string& page_name) const;

    
    void display_page(std::ostream& ostr, const std::string& page_name) const;

    
    WebPageSet search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const;
    std::vector<std::pair<WebPage*, double>> pageRank(WebPageSet& in_pages);

private:
    void read_page(const std::string& filename);
    std::map<std::string, PageParser*> extension_and_parser;
    std::map<std::string, WebPage*> name_and_webpagepointer;
    std::map<std::string, WebPageSet> name_and_webpageset;
    PageParser* noExtensionParser_;
    


};

#endif
