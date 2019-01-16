#include "parser.hpp"
#include "codegen.hpp"
#include "driver.hpp"

int main(void) {
    Codegen::init();
    Driver driver;
    yy::parser parser(driver);
    parser.parse();
    return 0;
}
