#include <iostream>
#include "handler.h"
using namespace std;

Handler::Handler() :
    next_(NULL)
{
}
Handler::Handler(Handler* next) :
    next_(next)
{
}

Handler::~Handler()
{
    if(next_) {
        delete next_;
    }
}

void Handler::set_next(Handler* next)
{
    next_ = next;
}

Handler::HANDLER_STATUS_T Handler::handle(SearchEng* eng, const std::string& cmd, std::istream& instr, std::ostream& ostr)
{
    if(canHandle(cmd)) {
        return process(eng, instr, ostr);
    }
    else if(next_ != NULL) {
        return next_->handle(eng, cmd, instr, ostr);
    }
    else {
        cout << "Unrecognized command" << endl;
        return HANDLER_ERROR;
    }
}

