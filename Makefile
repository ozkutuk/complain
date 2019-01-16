all: cpln

cpln: lexer.l parser.y main.cpp ast.hpp codegen.cpp codegen.hpp driver.cpp driver.hpp
	flex -o lexer.cpp lexer.l
	bison -o parser.cpp parser.y
	$(CXX) -g -std=c++17 `llvm-config --libs core native --ldflags` main.cpp lexer.cpp parser.cpp codegen.cpp driver.cpp -o $@

.PHONY: clean
clean:
	rm lexer.cpp parser.cpp parser.hpp stack.hh cpln
