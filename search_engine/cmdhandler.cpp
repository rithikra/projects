#include "cmdhandler.h"
#include "util.h"
#include "msort.h"
using namespace std;

struct alphabetical {
    bool operator()(const std::pair<WebPage*, double> &lhs, 
        const std::pair<WebPage*, double> &rhs) 
    {   
        string left_word =  lhs.first->filename();
        string right_word = rhs.first->filename();
        return (left_word < right_word); 
    }
};

struct score_order {
    bool operator()(const std::pair<WebPage*, double> &lhs, 
        const std::pair<WebPage*, double> &rhs) 
    {   
        double left_number =  lhs.second;
        double right_number = rhs.second;
        return (left_number > right_number); 
    }
};

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}




ANDHandler::ANDHandler()
{

}

ANDHandler::ANDHandler(Handler* next)
    : Handler(next)
{

}

bool ANDHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";

}

Handler::HANDLER_STATUS_T ANDHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string current_term;
    vector<string> all_terms;
    //take in all values into web page
    while (instr >> current_term){
        all_terms.push_back(conv_to_lower(current_term));
    }
    //create the combiner
    ANDWebPageSetCombiner combiner;
    WebPageSet final_pages;
    //run the search function using and combiner
    final_pages = eng->search(all_terms, &combiner);
    //display the search
    display_hits(final_pages, ostr);
    return HANDLER_OK;
}


ORHandler::ORHandler()
{

}

ORHandler::ORHandler(Handler* next)
    : Handler(next)
{

}

bool ORHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OR";

}

Handler::HANDLER_STATUS_T ORHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string current_term;
    vector<string> all_terms;
    //take in all values into webpage
    while (instr >> current_term){
        all_terms.push_back(conv_to_lower(current_term));
    }
    //create OR combiner and new return webpageset
    ORWebPageSetCombiner combiner;
    WebPageSet final_pages;
    //run search function
    final_pages = eng->search(all_terms, &combiner);
    //display the results
    display_hits(final_pages, ostr);
    return HANDLER_OK;
}


DIFFHandler::DIFFHandler()
{

}

DIFFHandler::DIFFHandler(Handler* next)
    : Handler(next)
{

}

bool DIFFHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";

}

Handler::HANDLER_STATUS_T DIFFHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string current_term;
    vector<string> all_terms;
    //get all terms in input
    while (instr >> current_term){
        all_terms.push_back(conv_to_lower(current_term));
    }
    //create diff combiner
    DiffWebPageSetCombiner combiner;
    WebPageSet final_pages;
    //run search
    final_pages = eng->search(all_terms, &combiner);
    //display hits
    display_hits(final_pages, ostr);
    return HANDLER_OK;
}



PRANDHandler::PRANDHandler()
{
}

PRANDHandler::PRANDHandler(Handler* next)
    : Handler(next)
{

}


bool PRANDHandler::canHandle(const std::string& cmd) const{
    return cmd == "PRAND";
}
Handler::HANDLER_STATUS_T PRANDHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string current_command;
    instr >> current_command;
    if (current_command != "n" && current_command != "r"){
        //error
        throw invalid_argument("Invalid Input");
    }
    string current_term;
    vector<string> all_terms;



    //get all terms in input
    while (instr >> current_term){
        all_terms.push_back(conv_to_lower(current_term));
    }
    

    //create diff combiner
    ANDWebPageSetCombiner combiner;
    WebPageSet final_pages;


    //run search
    final_pages = eng->search(all_terms, &combiner);


    if (final_pages.empty()){
        ostr << "0" << endl;
        return HANDLER_OK;
    }

    //add in pagerank part
    WebPageScorePairs ranked_pages = eng->pageRank(final_pages);

    //SORT
    if (current_command == "n"){
        mergeSort(ranked_pages, alphabetical());
    }
    else if (current_command == "r"){
        mergeSort(ranked_pages, score_order());
    }



    //display hits
    display_ranked_hits(ranked_pages, ostr);
    return HANDLER_OK;
}


PRORHandler::PRORHandler()
{
}

PRORHandler::PRORHandler(Handler* next)
    : Handler(next)
{

}


bool PRORHandler::canHandle(const std::string& cmd) const{
    return cmd == "PROR";
}
Handler::HANDLER_STATUS_T PRORHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string current_command;
    instr >> current_command;
    if (current_command != "n" && current_command != "r"){
        //error
        throw invalid_argument("Invalid Input");
    }
    string current_term;
    vector<string> all_terms;




    //get all terms in input
    while (instr >> current_term){
        all_terms.push_back(conv_to_lower(current_term));
    }
    

    //create diff combiner
    ORWebPageSetCombiner combiner;
    WebPageSet final_pages;
    //run search
    final_pages = eng->search(all_terms, &combiner);
     if (final_pages.empty()){
        ostr << "0" << endl;
        return HANDLER_OK;
    }

    //add in pagerank part
    WebPageScorePairs ranked_pages = eng->pageRank(final_pages);

    //SORT
    if (current_command == "n"){
        mergeSort(ranked_pages, alphabetical());
    }
    else if (current_command == "r"){
        mergeSort(ranked_pages, score_order());
    }



    //display hits
    display_ranked_hits(ranked_pages, ostr);
    return HANDLER_OK;
}




OutgoingHandler::OutgoingHandler()
{

}

OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";

}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string name;
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        //find the page 
        WebPage* current_page = eng->retrieve_page(name);
        //if null, error
        if (current_page == NULL){
            return HANDLER_ERROR;
        }
        else{
            //otherwise get a webpageset of all outgoing links
            WebPageSet OUTGOINGLINKS = current_page->outgoing_links();
            //display the links
            display_hits(OUTGOINGLINKS,ostr);
        }
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}




IncomingHandler::IncomingHandler()
{

}

IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";

}

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
     string name;
     //if cannot get proper input, error
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        //retrive the page that we want links for
        WebPage* current_page = eng->retrieve_page(name);
        //if results = null, error
        if (current_page == NULL){
            return HANDLER_ERROR;
        }
        else{
            //otherwise find incoming links
            WebPageSet INCOMINGLINKS = current_page->incoming_links();
            //print incoming links
            display_hits(INCOMINGLINKS,ostr);
        }
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}





