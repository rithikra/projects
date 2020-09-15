#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "gitint.h"

using namespace std;


const std::string INVALID_COMMAND = "Invalid command";
const std::string INVALID_OPTION = "Invalid option";
const std::string INVALID_COMMIT_NUMBER = "Invalid commit number";
const std::string INVALID_ARGUMENT = "Invalid argument";
const std::string FILE_EXISTS = "File already exists";
const std::string FILE_NOT_EXISTS = "File does not exist";
const std::string LOG_COMMIT_STARTER = "Commit: ";
const std::string NO_INPUT = "Missing or no input message provided";
const std::string INVALID_COMMIT_MESSAGE = "Invalid commit message";
const std::string NO_FILES_ADDED = "No files have been staged to be commited";
const std::string TAG_EXISTS = "Tag already exists";
const std::string NO_COMMIT_OR_TAG = "No commit number or tag name";
const std::string TAG_NOT_EXISTS = "Tag does not exist";

GitInt::GitInt(){
}


void GitInt::create(const std::string& filename, int value){
    if (filename.empty()) {
        throw std::runtime_error(INVALID_ARGUMENT);
    }
    if (user_files.find(filename) != user_files.end()) {
        throw std::invalid_argument(FILE_EXISTS);
    }
    user_files[filename] = value;
    return;
}


void GitInt::edit(const std::string& filename, int value){
    if (filename.empty()) {
        throw std::runtime_error(INVALID_ARGUMENT); 
    }
    if (user_files.find(filename) == user_files.end()){
        throw std::invalid_argument(FILE_NOT_EXISTS);
    }
    map<std::string, int>::iterator it = user_files.find(filename);
    if (it != user_files.end()){
        it->second = value;
    }
    return;
}



void GitInt::display(const std::string& filename) const{
    map<std::string, int>::const_iterator it = user_files.find(filename);
    if (it != user_files.end()){
        cout << filename << " : " << it->second << endl;
    }
    else{
        throw std::invalid_argument(FILE_NOT_EXISTS);
    }
}
void GitInt::display_all() const{
    display_helper(user_files);
}


void GitInt::add(std::string filename){
    if (user_files.find(filename) != user_files.end()){
        to_commit.insert(filename);
    }
    else
    {
        throw std::invalid_argument(FILE_NOT_EXISTS);
    }    
}

 
std::map<std::string, int> GitInt::buildState(CommitIdx from, CommitIdx to ) const{
    CommitIdx counter = from;
    std::map<std::string,int> total;
    while  (counter > to && counter > 0){
        map<std::string,int>::const_iterator it;
        for(it = commit_history[counter].diffs_.begin(); it != commit_history[counter].diffs_.end(); ++it){
            total[it->first] += it->second;
        }
        counter = commit_history[counter].parent_;
    }
    return total;
}


 
void GitInt::commit(std::string message){
    if (message == ""){
        throw std::runtime_error(NO_INPUT);
    }
    if (to_commit.empty()){
        throw std::runtime_error(NO_FILES_ADDED);
    }
    if (count == 0){
        count+= 1;
        map<string, int> differences;
        CommitObj dummy_commit("", differences, 0);
        commit_history.push_back(dummy_commit);
        set<std::string>::iterator it;
        for (it = to_commit.begin(); it != to_commit.end(); ++it){
            differences[*it] = user_files[*it];
        }
        CommitObj commit_(message, differences, 0);
        commit_history.push_back(commit_);
        to_commit.clear();
        head_ = 1;
    }
    else{
        map<string, int> differences;
        map<string, int> past_total = buildState(head_,0);
        set<std::string>::iterator it;
        for (it = to_commit.begin(); it != to_commit.end(); ++it){
            if (past_total.find(*it) == past_total.end()){
                differences[*it] = user_files[*it];    
            }
            else if (past_total[*it] != user_files[*it]){
                differences[*it] = user_files[*it] - past_total[*it];
            }
        }
        CommitObj commit_(message, differences, head_);
        commit_history.push_back(commit_);

        to_commit.clear();
        head_ = (int) commit_history.size() - 1;
    }
}




void GitInt::create_tag(const std::string& tagname, CommitIdx commit){
    if (tag_map.find(tagname) == tag_map.end()){
        tag_map[tagname] = commit;
        tag_vector.push_back(tagname);
    }
    else{
        throw std::invalid_argument(TAG_EXISTS);
    }
    return;
}

    

 void GitInt::tags() const{
    if (tag_vector.empty()){
        return;
    }
    for (int counter = (int) tag_vector.size() - 1; counter > -1; --counter){
        cout << tag_vector[counter] << endl;
    }
    return; 
 }

  
bool GitInt::checkout(CommitIdx commitIndex){
    bool exist_ = valid_commit(commitIndex);
    if (exist_){
        head_ = commitIndex;
        to_commit.clear();
        return true;
    }
    else{
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
}


bool GitInt::checkout(std::string tag){
    if (tag_map.find(tag) != tag_map.end()){
        head_ = tag_map[tag];
        to_commit.clear();
        return true;
    }
    else{
        throw std::invalid_argument(TAG_NOT_EXISTS);
    }
}


void GitInt::log() const{
    CommitIdx i = head_;
    while (i != 0){
        string message = commit_history[i].msg_;
        log_helper(i, message);
        i = commit_history[i].parent_;
    }
}



void GitInt::diff(CommitIdx to) const{
    map<string, int> total_diffs;
    map<string, int> build_state_diffs = buildState(head_);
    map<string, int>::const_iterator it;
    if (to > (int) (commit_history.size()-1)){
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    if (commit_history.size() == 1){
        map<std::string,int>::const_iterator it;
        for (it = user_files.begin(); it != user_files.end(); ++it){
            cout << it->first << " : " << it->second << endl;
        }
    }
    else if (commit_history.size() > 1){
        for(it = user_files.begin(); it != user_files.end(); ++it){
            if (build_state_diffs.find(it->first) != build_state_diffs.end()){
                total_diffs[it->first] = user_files.at(it->first) - build_state_diffs[it->first];
            }
            else{ 
                total_diffs[it->first] = user_files.find(it->first)->second;
            }
            if (total_diffs[it->first] == 0){
                total_diffs.erase(it->first);
            }
        }
        map<string, int>::const_iterator other_it;
        for(other_it = total_diffs.begin(); other_it != total_diffs.end(); other_it++){
            cout << other_it->first << " : " << other_it->second << endl;
        }
    }
    else{
        for (it = user_files.begin(); it != user_files.end(); ++it){
            cout << it->first << " : " << it->second << endl;
        }
    }
}

    
void GitInt::diff(CommitIdx from, CommitIdx to) const{
    if ((to < 0) || !valid_commit(from)){
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    else if (from < to){
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    else if (to == 0){
        map<string, int> from_map = buildState(from);
        map<std::string,int>::const_iterator it;
        for (it = from_map.begin(); it != from_map.end(); ++it){
            cout << it->first << " : " << it->second << endl;
        }
    }
    else{
        map<string, int> from_map = buildState(from);
        map<string, int> to_map = buildState(to);
        map<string, int> current_state;
        map<std::string,int>::const_iterator it;
        for (it = from_map.begin(); it != from_map.end(); ++it){
            if (to_map.find(it->first) != to_map.end()){
                current_state[it->first] = from_map[it->first] - to_map[it->first];
            }
            else if (to_map.find(it->first) == to_map.end()){
                current_state[it->first] = from_map[it->first];
            }
            if (current_state[it->first] == 0){
                current_state.erase(it->first);
            }
        }
        for (it = current_state.begin(); it != current_state.end(); ++it){
            cout << it->first << " : " << it->second << endl;
        }
    }
}


bool GitInt::valid_commit(CommitIdx commit) const{
    if ((commit >= 0 ) && (commit < (int) (commit_history.size()))) {
        return true;
    }
    else {
        return false;
    }
}
void GitInt::print_menu() const
{
    cout << "Menu:                          " << endl;
    cout << "===============================" << endl;
    cout << "create   filename int-value    " << endl;
    cout << "edit     filename int-value    " << endl;
    cout << "display  (filename)            " << endl;
    cout << "display  commit-num            " << endl;
    cout << "add      file1 (file2 ...)     " << endl;
    cout << "commit   \"log-message\"       " << endl;
    cout << "tag      (-a tag-name)         " << endl;
    cout << "log                            " << endl;
    cout << "checkout commit-num/tag-name   " << endl;
    cout << "diff                           " << endl;
    cout << "diff     commit                " << endl;
    cout << "diff     commit-n commit-m     " << endl;
}


bool GitInt::process_command(std::string cmd_line)
{
    bool quit = false;
    std::stringstream ss(cmd_line);
    std::string cmd;
    ss >> cmd;
    if (ss.fail()) throw std::runtime_error(INVALID_COMMAND);

    if (cmd == "quit") {
        quit = true;
        return quit;
    }
    else if (cmd == "create"){
        std::string name;
        int value;
        ss >> name;
        if (ss.fail()){
            throw std::runtime_error(INVALID_ARGUMENT);
        }
        ss >> value;
         if (ss.fail()){
            throw std::runtime_error(INVALID_ARGUMENT);
        }
        create(name, value);
    }

    else if (cmd == "edit"){
        std::string name;
        int value;
        ss >> name;
        if (ss.fail()){
            throw std::runtime_error(INVALID_ARGUMENT);
        }
        ss >> value;
         if (ss.fail()){
            throw std::runtime_error(INVALID_ARGUMENT);
        }
        edit(name, value);

    }
    else if (cmd == "display"){
        std::string filename;
        CommitIdx commit_number;
        if (ss.peek() == std::char_traits<char>::eof()){
            display_all();
        }
        ss >> commit_number;
        if (!ss.fail()){
            if (valid_commit(commit_number)){
                display_commit(commit_number);
            }
        }
        else if (ss.fail()){
            ss.clear();
            ss>> filename;
            if (!ss.fail()){
                ss >> filename; 
                display(filename);
            }
        } 
    }
    else if (cmd == "add"){
        if (ss.peek() == std::char_traits<char>::eof()){
            return quit;
        }
        std::string filename;
        ss >> filename;
        while (!ss.fail()){
            add(filename);
            ss >> filename;
        }
    }
    else if (cmd == "commit"){
        if (ss.peek() == std::char_traits<char>::eof()){
            throw std::runtime_error(INVALID_COMMIT_MESSAGE);
            return quit;
        }
        std::string commit_message;
        getline(ss, commit_message);
        if (ss.fail()){
            throw std::runtime_error(INVALID_COMMIT_MESSAGE);
        }
        else{
            if (commit_message[1] != '"'){
                throw std::runtime_error(INVALID_COMMIT_MESSAGE);
            }
            bool second_quote = false;
            int second_quote_location;
            for (int i = 2; i < (int) commit_message.size(); i++){
                if (commit_message[i] == '"'){
                    second_quote = true;
                    second_quote_location = i;
                }
            }
            if (second_quote){
                std::string proper_message;
                for (int i = 2; i < second_quote_location; i++){
                    proper_message += commit_message[i];
                }
                proper_message[second_quote_location] = '\0';
                commit(proper_message);
            }
            else{
                throw std::runtime_error(INVALID_COMMIT_MESSAGE);
            }
        }
    }
    else if (cmd == "tag"){
        if (ss.peek() == std::char_traits<char>::eof()){
           
            tags();
           
            return quit;
        }
        else{
            std::string a_character;
            ss >> a_character;
            if (ss.fail()){
                throw std::runtime_error(INVALID_ARGUMENT);
            }
            if (a_character != "-a"){
                throw std::runtime_error(INVALID_ARGUMENT);
            }
            else{
                if (ss.peek() == std::char_traits<char>::eof()){
                    throw std::runtime_error(INVALID_ARGUMENT);
                }
                else{
                    std::string tag_message;
                    ss >> tag_message;
                    create_tag(tag_message, head_);
                }
            }
        }
    }
    else if (cmd == "checkout"){
        if (ss.peek() == std::char_traits<char>::eof()){
            throw std::runtime_error(NO_COMMIT_OR_TAG);
            return quit;
        }
        else{
            CommitIdx commit_number;
            string tag_name;
            ss >> commit_number;
            if (!ss.fail()){
                checkout(commit_number);
            }
            else if (ss.fail()){
                ss.clear();
            }   
            ss >> tag_name; 
            if (!ss.fail()){
                checkout(tag_name);
            }
            else if (ss.fail()){
                ss.clear();
            } 
        }
    }
    else if (cmd == "log"){
        log();
    }
    else if (cmd == "diff"){
        if (ss.peek() == std::char_traits<char>::eof()){
           diff(head_);
        }
        else{
            CommitIdx starting = 0;
            int ending = 0;
            ss >> starting;
            bool starting_fail = ss.fail();
            ss >> ending;
            bool ending_fail = ss.fail();
       
            if (!ending_fail && ending > 0){
                diff(starting, ending);
            }
            else if (!starting_fail){
                diff(starting);
            }
            else {
                throw std::runtime_error(INVALID_ARGUMENT);
            }
        }
    }
    else{
        throw std::runtime_error(INVALID_COMMAND);
    }



    ss.clear();


    
    return quit;
}

void GitInt::display_commit(CommitIdx commit) const
{
    if (false == valid_commit(commit) ) {
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    display_helper(commit_history[commit].diffs_);
}
void GitInt::display_helper(const std::map<std::string, int>& dat) const
{
    for (std::map<std::string, int>::const_iterator cit = dat.begin();
            cit != dat.end();
            ++cit) {
        std::cout << cit->first << " : " << cit->second << std::endl;
    }
}
void GitInt::log_helper(CommitIdx commit_num, const std::string& log_message) const
{
    std::cout << LOG_COMMIT_STARTER << commit_num << std::endl;
    std::cout << log_message << std::endl << std::endl;

}



