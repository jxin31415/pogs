# acceptable build_types: Release/Debug/Profile
build_type := Release
# build_type := Debug

.SILENT:

all: 
	rm -rf bin && \
	rm -rf fun_verifier/in && \
	mkdir bin && \
	mkdir fun_verifier/in && \
	g++ -std=c++17 main.cpp -o bin/main && \
	./bin/main
