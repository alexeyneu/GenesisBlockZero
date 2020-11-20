LINKER=c++
GUILIBS=--std=c++20 `pkg-config --cflags --libs opencv4`

t : force.cpp
	$(LINKER) force.cpp $(GUILIBS) -o t

