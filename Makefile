FLEX_ROOT = /usr/local/opt/flex
BISON_ROOT = /usr/local/opt/bison

FLEX = $(FLEX_ROOT)/bin/flex
BISON = $(BISON_ROOT)/bin/bison
CXX = g++

CXXFLAGS = -I$(FLEX_ROOT)/include
LDFLAGS =

HEADERS = FieldSpecs.h FieldSpecsAstNode.h FieldSpecsAst.h FieldSpecsDriver.h
OBJECTS = test.o FieldSpecsParser.o FieldSpecsScanner.o FieldSpecsDriver.o

all: test

FieldSpecsScanner.cpp: FieldSpecsScanner.l
	@$(FLEX) -o $@ $^
	@echo "[Notice]: Flex process done."

FieldSpecsParser.cpp: FieldSpecsParser.y
	@$(BISON) -o $@ --defines=$(basename $@).h $^
	@echo "[Notice]: Bison process done."

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

test: $(OBJECTS) $(HEADERS)
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)
	@echo "[Notice]: Linking process done."

.PHONY: clean

clean:
	rm -f FieldSpecsScanner.cpp FieldSpecsParser.h
	rm -f FieldSpecsParser.cpp location.hh position.hh stack.hh
	rm -f $(OBJECTS) test
