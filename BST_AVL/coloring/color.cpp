#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include "avlbst.h"

using namespace std;


typedef AVLTree<char, int> ANSWER_MAP_T;
void printSolution(map<char,int>& answer, std::ostream& ostr);
void readMap(string file_name, map<char,set<char>> &closeby);
bool isValidLocation(int row, int col, int curr_row, int curr_col);
bool color_countries(char current, map<char,set<char>> closeby, map<char,int>& color_assignment);
bool isValid(char current, int color, map<char,set<char>> closeby,  map<char,int> color_assignment);


int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Please supply a map input file and output filename" << endl;
        return 1;
    }
    map<char, set<char>> closeby;
    readMap(argv[1], closeby);
    map<char,int> color_assignment;
    char current = closeby.begin()->first;
    color_countries(current, closeby, color_assignment);
    ofstream ofile(argv[2]);
    printSolution(color_assignment, ofile);
    ofile.close();

    return 0;
}


void printSolution(map<char,int>& answer, std::ostream& os)
{
    for(map<char,int>::iterator it = answer.begin();
            it!= answer.end();
            ++it) {
        os << it->first << " " << it->second << endl;
    }
} 


void readMap(string file_name, map<char,set<char>> &closeby)
{
    ifstream ifile(file_name);
    string current_line;
    getline(ifile, current_line);
    stringstream values(current_line);

    int country_number = 0;
    int col = 0;
    int row = 0;
    values >> country_number;
    values >> row;
    values >> col;
    char map_of_countries[row][col];
    for (int i = 0; i < row; i++){
        getline(ifile, current_line);
        stringstream values(current_line);
        for (int j = 0; j < col; j++){
            char val = 0;
            values >> val;
            map_of_countries[i][j] = val;
        }
        values.clear();
    }
    ifile.close();
     for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            if (closeby.find(map_of_countries[i][j]) == closeby.end()){
                set<char> v;
                closeby[map_of_countries[i][j]] = v;
            }
            if (isValidLocation(row, col, i + 1, j)){
                char c = map_of_countries[i][j];
                char o = map_of_countries[i+1][j];
                if (c != o){
                    if (closeby[c].find(o) == closeby[c].end())
                        closeby[c].insert(o);
                }
            }
            if (isValidLocation(row, col, i , j + 1)){
                char c = map_of_countries[i][j];
                char o = map_of_countries[i][j+1];
                if (c != o){
                    if (closeby[c].find(o) == closeby[c].end())
                        closeby[c].insert(o);
                }
            }
            if (isValidLocation(row, col, i - 1 , j)){
                char c = map_of_countries[i][j];
                char o = map_of_countries[i-1][j];
                if (c != o){
                    if (closeby[c].find(o) == closeby[c].end())
                        closeby[c].insert(o);
                }
            }
            if (isValidLocation(row, col, i , j- 1)){
                char c = map_of_countries[i][j];
                char o = map_of_countries[i][j-1];
                if (c != o){
                    if (closeby[c].find(o) == closeby[c].end())
                        closeby[c].insert(o);
                }
            }
        }
    }
}


bool isValidLocation(int row, int col, int curr_row, int curr_col){
    bool r = false;
    bool c = false;
    if ((curr_row >=0) && (curr_row < row)){
        r = true;
    }
    if ((curr_col >=0) && (curr_col < col)){
        c = true;
    }
    return (r && c);

}
bool color_countries(char current, map<char,set<char>> closeby, map<char,int>& color_assignment){
    if (color_assignment.size() == closeby.size()){
        return true;
    }
    else{
        for (int i = 1; i < 5; i++){
            if (isValid(current, i, closeby, color_assignment))

            {
                color_assignment[current] = i;
                map<char,set<char>>::iterator it = closeby.find(current);
                it++;
                char new_ = it->first;
                bool status = color_countries(new_, closeby, color_assignment);
                if (status){
                    return true;
                }
                else{
                    color_assignment.erase(current);
                }
            }
        }
    }
    return false;

}


bool isValid(char current, int color, map<char,set<char>> closeby,  map<char,int>color_assignment){
    set<char> close_current = closeby[current];
    set<char>::iterator it;
    bool valid = true;
    for (it = close_current.begin(); it != close_current.end(); it++){
        if (color_assignment.find(*it) != color_assignment.end()){
            bool same = (color_assignment.find(*it)->second != color);
            valid = valid && same;
        }
    }
    return valid;


}

