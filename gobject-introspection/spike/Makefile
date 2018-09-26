LLVM_CONFIG = llvm-config

all: run

clean:
	rm -rf libllvm-shared.so
	rm -rf llvm-call

run: llvm-call
	./llvm-call

libllvm-shared.so: llvm-shared.c
	cc -fPIC -shared -o $@ $<

llvm-call: llvm-call.cc libllvm-shared.so
	c++ $$($(LLVM_CONFIG) --cxxflags --ldflags --libs) --std=c++14 \
	  -o $@ $< libllvm-shared.so
