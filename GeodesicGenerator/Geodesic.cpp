#include "Geodesic.h"



void Geodesic::init(std::vector<glm::vec3>& verts, std::vector<unsigned>& inds, unsigned depth) {
	for(size_t i = 0; i < inds.size(); i += 3) {
		Faces.push_back(Face(inds[i], inds[i + 1], inds[i + 2]));
	}
	Vertices = std::vector<glm::vec3>(verts);
	//std::cout << Faces.size() << ' ' << Vertices.size() << std::endl;
	unsigned nFaces = Faces.size();
	for(size_t i = 0; i < nFaces; i++) {
		tesselate(0, depth);
	}
	genIndices();
	genNormals();
	//std::cout << Faces.size() << ' ' << Vertices.size() << std::endl;
}

void Geodesic::tesselate(unsigned face, unsigned depth = 0) {
	unsigned end = Vertices.size();
	Face f = Faces[face]; 
	if(depth != 0) {
		glm::vec3 v1, v2, v3;
		v1 = Vertices[f[0]];
		v2 = Vertices[f[1]];
		v3 = Vertices[f[2]];

		Faces.erase(Faces.begin() + face); // Remove now unnescesary face

		// Compute new vertices
		Vertices.push_back(glm::normalize((v1 + v3) / 2.0f));
		Vertices.push_back(glm::normalize((v1 + v2) / 2.0f));
		Vertices.push_back(glm::normalize((v2 + v3) / 2.0f));

		unsigned nFaces = Faces.size(); // Get number of faces for recursion

		// Add new faces
		Faces.push_back(Face(end, f[0], end + 1));
		Faces.push_back(Face(end + 1, f[1], end + 2));
		Faces.push_back(Face(end + 2, f[2], end));
		Faces.push_back(Face(end, end + 1, end + 2));

		// Recursive tesselation for each new face
		for(size_t i = 0; i < 4; i++) {
			tesselate(nFaces, depth - 1);
		}
	}
}

void Geodesic::genIndices() {
	for each (Face face in Faces) {
		for(size_t i = 0; i < 3; i++) {
			Indices.push_back(face[i]);
		}
	}
}

void Geodesic::genNormals() {
	for each (glm::vec3 vertex in Vertices) {
		Normals.push_back(glm::normalize(vertex));
	}
}

Geodesic::Geodesic(std::vector<glm::vec3>& verts, std::vector<unsigned>& inds, unsigned depth = 0) {
	init(verts, inds, depth);
}

Geodesic::Geodesic() {
}


Geodesic::~Geodesic() {
}
