#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"

using namespace std;




SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;


}


void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}



SearchEng::~SearchEng(){
    
    delete noExtensionParser_;

    std::map<std::string, WebPage*>:: iterator webpage_it;
    for (webpage_it = name_and_webpagepointer.begin(); webpage_it != name_and_webpagepointer.end(); ++webpage_it){
        if ((webpage_it->second) != NULL){
            delete (webpage_it->second);
        }
    } 
}



void SearchEng::register_parser(const std::string& extension, PageParser* parser){
    if (extension_and_parser.find(extension) == extension_and_parser.end()){
        extension_and_parser[extension] = parser;
    }
}

 

WebPage* SearchEng::retrieve_page(const std::string& page_name) const{
    map<string, WebPage*>::const_iterator key_finder = name_and_webpagepointer.find(page_name);
    if(key_finder != name_and_webpagepointer.end()){
        return (key_finder->second);
    }
    else{
        return NULL;
    }
}



 
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const{
    WebPage* current_page = retrieve_page(page_name);
    if (current_page == NULL){
        throw std::invalid_argument("Page does not exist");
    }
    else{
        std::string extension_name = extract_extension(page_name);
        if (extension_and_parser.find(extension_name) == extension_and_parser.end()){
            throw std::logic_error("Parser has not been registered / created");
        }
        else{
            string text;
            text = extension_and_parser.find(extension_name)->second->display_text(page_name);
            ostr << text;

        }
    }
}

    
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const{
    vector<string>::const_iterator terms_iterator = terms.begin();
    WebPageSet final_set;

    if (terms.size() == 0){
      return final_set;
    }

    if (name_and_webpageset.find(*terms_iterator) != name_and_webpageset.end()){
        final_set = name_and_webpageset.find(*terms_iterator)->second;
        terms_iterator++;
    }

    if (terms.size() > 1){
      while (terms_iterator != terms.end()){
          std::map<string,WebPageSet>::const_iterator page_it = name_and_webpageset.find(*terms_iterator);
          if (page_it != name_and_webpageset.end()){
            final_set = combiner->combine(final_set, page_it->second);
            terms_iterator++;
          }
          else{
            WebPageSet any;
            final_set = combiner->combine(final_set, any);
            terms_iterator++;
          }
      }
    }


    return final_set;
}


std::vector<std::pair<WebPage*, double>> SearchEng::pageRank(WebPageSet& in_pages){
  vector<pair<WebPage*, double>> ranked_results;
  set<WebPage*>::iterator it;
  WebPageSet candidate_set = in_pages;
  for (it = in_pages.begin(); it != in_pages.end(); ++it){
    WebPageSet incoming = (*it)->incoming_links();
    WebPageSet outgoing = (*it)->outgoing_links();
    set<WebPage*>::iterator i;
    for (i = incoming.begin(); i != incoming.end(); ++i){
      candidate_set.insert(*i);
    }
    for (i = outgoing.begin(); i != outgoing.end(); ++i){
      candidate_set.insert(*i);
    }
  }
  map<WebPage*, WebPageSet> page_and_incoming;
  WebPageSet temp_incoming;
  for (it = candidate_set.begin(); it != candidate_set.end(); ++it){
    set<WebPage*>::iterator i;
    temp_incoming = (*it)->incoming_links();
    WebPageSet final_incoming;
    for (i = temp_incoming.begin(); i != temp_incoming.end(); ++i){
      if (candidate_set.find(*i) != candidate_set.end()){
        final_incoming.insert(*i);
      }
    }
    final_incoming.insert(*it);
    page_and_incoming[*it] = final_incoming;
    final_incoming.clear();
    temp_incoming.clear();
  }
  map<WebPage*, WebPageSet> page_and_outgoing;
  WebPageSet temp_outgoing;
  for (it = candidate_set.begin(); it != candidate_set.end(); ++it){
    set<WebPage*>::iterator i;
    temp_outgoing = (*it)->outgoing_links();
    WebPageSet final_outgoing;
    for (i = temp_outgoing.begin(); i != temp_outgoing.end(); ++i){
      if (candidate_set.find(*i) != candidate_set.end()){
        final_outgoing.insert(*i);
      }
    }
    final_outgoing.insert(*it);
    page_and_outgoing[*it] = final_outgoing;
    final_outgoing.clear();
    temp_outgoing.clear();
  }
  std::vector<std::pair<WebPage*, double>> temp_pr;
  std::vector<std::pair<WebPage*, double>> actual_pr;
  for (it = candidate_set.begin(); it != candidate_set.end(); ++it){
    pair<WebPage*, double> temp((*it), (double) (1.00000/candidate_set.size()));
    actual_pr.push_back(temp);
  }
  for (int a = 0; a < 20; a++){
    temp_pr.clear();
    for (int b = 0; b < (int) actual_pr.size(); b++){
      double current_rank = (0.15)/ actual_pr.size();
      WebPageSet incoming_temp = (page_and_incoming.find(actual_pr[b].first))->second;
      double summation = 0;
      for (it = incoming_temp.begin(); it !=incoming_temp.end(); ++it){
        WebPage* current_webpage = (*it);
        int outgoing_degrees = (page_and_outgoing.find(current_webpage)->second).size();
        int index;
        for (int i = 0; i < (int) actual_pr.size(); i++){
          if (actual_pr[i].first == current_webpage){
            index = i;
            break;
          }
        }
        double previous_rank = actual_pr[index].second;
        summation += (previous_rank/ (double) outgoing_degrees);
      }
      summation = (0.85) * summation;
      current_rank += summation;
      pair<WebPage*, double> temporary(actual_pr[b].first, (current_rank));
      temp_pr.push_back(temporary);
    }
    actual_pr = temp_pr;
  }
  return actual_pr;

}



void SearchEng::read_page(const std::string& filename){
    std::string extension_name = extract_extension(filename);


    map<string, WebPage*>::iterator key_finder = name_and_webpagepointer.find(filename);
    WebPage* webpage_new;
    if (key_finder == name_and_webpagepointer.end()){
        webpage_new = new WebPage(filename);
        name_and_webpagepointer[filename] = webpage_new;
        
    }
    else{
        webpage_new = key_finder->second;
      
    }
    StringSet search_terms;
    StringSet out_going;
    (extension_and_parser[extension_name])->parse(filename, search_terms, out_going);
    webpage_new->all_terms(search_terms);
    std::map<std::string, WebPageSet>::iterator term_searcher;
    std::set<string>::iterator terms;
    for (terms = search_terms.begin(); terms != search_terms.end(); ++terms){
        std::map<std::string, WebPageSet>::iterator term_searcher = name_and_webpageset.find(*terms);
        if (term_searcher != name_and_webpageset.end()){
            (term_searcher->second).insert(webpage_new);
        }
        else{
            WebPageSet current_term;
            current_term.insert(webpage_new);
            name_and_webpageset[*terms] = current_term;
        }
    }
    set<string>::iterator link_iterator;
    for (link_iterator = out_going.begin(); link_iterator != out_going.end(); ++link_iterator){
        map<string, WebPage*>:: iterator found_webpage_iterator = name_and_webpagepointer.find(*link_iterator);
        if (found_webpage_iterator != name_and_webpagepointer.end()){
            webpage_new->add_outgoing_link(found_webpage_iterator->second);
            (found_webpage_iterator->second)->add_incoming_link(webpage_new);
        }
        else{
            WebPage* ingoing_link_webpage = new WebPage(*link_iterator);
            ingoing_link_webpage->add_incoming_link(webpage_new);
            webpage_new->add_outgoing_link(ingoing_link_webpage);
            name_and_webpagepointer[*link_iterator] = ingoing_link_webpage;

        }
    }

}























