#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "searchui.h"
#include "md_parser.h"
#include "txt_parser.h"
#include "cmdhandler.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Must provide an index file" << endl;
        return 1;
    }
    SearchEng seng(new TXTParser);
    MDParser md;
    seng.register_parser("md", &md);
    TXTParser text;
    seng.register_parser("txt", &text);
    SearchUI ui(&seng);
    ANDHandler * and_ = new ANDHandler();
    ORHandler* or_ = new ORHandler(and_);
    ui.add_handler(or_);
    DIFFHandler * diff_ = new DIFFHandler(or_);
    ui.add_handler(diff_);
    PrintHandler * print_ = new PrintHandler(diff_);
    ui.add_handler(print_);
    PRANDHandler * prand_ = new PRANDHandler(print_);
    ui.add_handler(prand_);
    PRORHandler * pror_ = new PRORHandler(prand_);
    ui.add_handler(pror_);
    IncomingHandler * incoming_ = new IncomingHandler(pror_);
    ui.add_handler(incoming_);
    OutgoingHandler * outgoing_ = new OutgoingHandler(incoming_);
    ui.add_handler(outgoing_);
    QuitHandler * quit_ = new QuitHandler(outgoing_);
    and_->set_next(quit_);
    ui.add_handler(quit_);
    ui.add_handler(and_);

    try {
        seng.read_pages_from_index(argv[1]);
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    bool result;
    if (argc == 2)
    {
        result = ui.run(cin, cout);

    }
    else if (argc == 3) {
        ifstream in_file(argv[2]);
        result = ui.run(in_file, cout);
    }
    else  {
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        result = ui.run(in_file, out_file);
    }

    return result;
}
