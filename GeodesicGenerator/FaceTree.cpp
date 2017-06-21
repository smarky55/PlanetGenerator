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
	/*size_t seed = Seed;
	double point = (Noise3d::getPoint(seed, 0.75, vertex)*0.4
					+ Noise3d::getPoint(++seed, 0.5, vertex)*0.5
					+ Noise3d::getPoint(++seed, 0.25, vertex)*0.25
					+ Noise3d::getPoint(++seed, 0.1, vertex)*0.1
					+ Noise3d::getPoint(++seed, 0.05, vertex)*0.075) / 1.325;
	
	double latitude = 90 - glm::degrees(glm::acos(glm::dot(vertex, glm::vec3(0, 1, 0))));
	double temp = ((Noise3d::getPoint(++seed, 0.3, vertex) + 2)/3)
				* glm::cos(glm::radians(latitude)) * 50 - 20;*/
	glm::vec4 colour = glm::vec4(0.1);
	/*float sea_offset = 0.55;
	float mountain_offset = 0.7;
	glm::vec3 blue_sea = glm::vec3(0, 0.22, 0.48);
	glm::vec3 green_grass = glm::vec3(0, 102. / 255, 0);
	glm::vec3 brown_mountain = glm::vec3(0.26, 0.14, 0.03);
	if(point < sea_offset) {
		colour = glm::vec4(blue_sea * float((point + 0.1) / (sea_offset + 0.1)), 
						   1.5f * (Noise3d::getPoint(++seed, 0.01, vertex)/4 +0.75));
	} else if(point < mountain_offset) {
		colour = glm::mix(glm::vec4(green_grass * float(point / mountain_offset), 0.1) ,
						  glm::vec4(brown_mountain, 0.1),
						  pow((point-sea_offset)/(mountain_offset-sea_offset), 4));
	} else {
		colour = glm::mix(glm::vec4(brown_mountain, 0.1), glm::vec4(1,1,1,0.1), 
						  pow(Noise3d::getPoint(++seed, 0.01, vertex) * (point-mountain_offset)/(1-mountain_offset), 0.6));
	}
	if(temp < 0) {
		colour = glm::mix(colour, glm::vec4(1,1,1,0.1), abs(temp / 20) + 0.5);
	}*/
	Colours.push_back(colour);

	Normals.push_back(vertex);
}
