#include "FaceTree.h"




void FaceTree::tesselate() {
	unsigned end = MMesh->Vertices.size();

	glm::vec3 v1, v2, v3;
	v1 = MMesh->Vertices[Indices[0]];
	v2 = MMesh->Vertices[Indices[1]];
	v3 = MMesh->Vertices[Indices[2]];

	MMesh->addVertex(glm::normalize((v1 + v3) / 2.0f));
	MMesh->addVertex(glm::normalize((v1 + v2) / 2.0f));
	MMesh->addVertex(glm::normalize((v2 + v3) / 2.0f));

	FaceNormal = glm::normalize((v1 + v2 + v3) / 3.0f);

	FaceA = new FaceTree(MMesh, end, Indices[0], end + 1);
	FaceB = new FaceTree(MMesh, end + 1, Indices[1], end + 2);
	FaceC = new FaceTree(MMesh, end + 2, Indices[2], end);
	FaceD = new FaceTree(MMesh, end, end + 1, end + 2);
}



FaceTree::FaceTree(Mesh* mesh, unsigned i1, unsigned i2, unsigned i3) {
	unsigned indices[] = {i1, i2, i3};
	for(size_t i = 0; i < 3; i++) {
		Indices[i] = indices[i];
	}
	MMesh = mesh;
}

FaceTree::~FaceTree() {
	if(FaceA != nullptr) {
		delete FaceA;
		delete FaceB;
		delete FaceC;
		delete FaceD;
	}
}

void FaceTree::getFaces(std::vector<FaceTree*>& faces, unsigned depth) {
	if(depth == 0) {
		faces.push_back(this);
	} else {
		if(FaceA == nullptr) {
			tesselate();
		}

		FaceA->getFaces(faces, depth - 1);
		FaceB->getFaces(faces, depth - 1);
		FaceC->getFaces(faces, depth - 1);
		FaceD->getFaces(faces, depth - 1);
	}
}

Mesh::Mesh(size_t seed) {
	Seed = seed;
}

Mesh::~Mesh() {
}

void Mesh::addVertex(glm::vec3 vertex) {
	Vertices.push_back(vertex);

	double point = (Noise3d::getPoint(Seed, 0.75, vertex.x, vertex.y, vertex.z)*0.4
					+ Noise3d::getPoint(Seed + 1, 0.5, vertex.x, vertex.y, vertex.z)*0.5
					+ Noise3d::getPoint(Seed + 2, 0.25, vertex.x, vertex.y, vertex.z)*0.25
					+ Noise3d::getPoint(Seed + 3, 0.1, vertex.x, vertex.y, vertex.z)*0.1) / 1.25;
	// std::cout << point << std::endl;
	double latitude = 90 - glm::degrees(glm::acos(glm::dot(vertex, glm::vec3(0, 1, 0))));
	double temp = ((Noise3d::getPoint(Seed + 4, 0.3, vertex.x, vertex.y, vertex.z) + 2)/3)
				* glm::cos(glm::radians(latitude)) * 50 - 20;
	glm::vec4 colour;
	if(point < 0.55) {
		colour = glm::vec4(0, 0.22, 0.48,1) * float((point+0.1)/0.65);
	} else {
		colour = glm::vec4(0, (102. / 255)*point, 0, 1);
	}
	if(temp < 0) {
		colour = glm::mix(colour, glm::vec4(1), abs(temp / 20) + 0.5);
	}
	Colours.push_back(colour);

	Normals.push_back(vertex);
}
