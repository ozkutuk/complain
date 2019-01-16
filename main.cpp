#include "parser.hpp"
#include "codegen.hpp"

int main(void) {
    Codegen::init();
    yy::parser parser;
    parser.parse();
    return 0;
}
