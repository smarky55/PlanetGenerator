#include "Face.h"



Face::Face(unsigned i0, unsigned i1, unsigned i2) {
	Indices[0] = i0;
	Indices[1] = i1;
	Indices[2] = i2;
}


Face::~Face() {
}
