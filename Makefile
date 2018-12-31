all: simple

simple: lexer.l parser.y main.cpp
	flex -o lexer.cpp lexer.l
	bison -o parser.cpp parser.y
	$(CXX) main.cpp lexer.cpp parser.cpp -o $@

.PHONY: clean
clean:
	rm lexer.cpp parser.cpp parser.hpp stack.hh simple
