#include "Planet.h"



void Planet::genIndices(unsigned depth) {
	size_t nVerts = mesh.size();
	Indices.clear();
	std::vector<FaceTree*> lodFaces;
	for each (FaceTree* face in Faces) {
		face->getFaces(lodFaces, depth);
	}
	for each (FaceTree* face in lodFaces) {
		for(size_t i = 0; i < 3; i++) {
			Indices.push_back((*face)[i]);
		}
	}
	if(mesh.Vertices.size() != nVerts) {
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(glm::vec3), mesh.Vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.Colours.size() * sizeof(glm::vec3), mesh.Colours.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.Normals.size() * sizeof(glm::vec3), mesh.Normals.data(), GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), Indices.data(), GL_STATIC_DRAW);
}

//void Planet::genBuffers() {
//	glGenBuffers(1, &VertexBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), Vertices.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &ColourBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
//	glBufferData(GL_ARRAY_BUFFER, Colours.size() * sizeof(glm::vec3), Colours.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &IndexBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, IndexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), Indices.data(), GL_STATIC_DRAW);
//}

Planet::Planet(size_t seed = 0, unsigned depth=0) : mesh(seed) {
	const double ic_a = 0.52573111211913360602566908484789;
	const double ic_b = 0.85065080835203993218154049706302;
	std::vector<glm::vec3> Vertices = {glm::vec3(0, ic_a, ic_b), glm::vec3(0, ic_a, -ic_b), glm::vec3(0, -ic_a, ic_b), glm::vec3(0, -ic_a, -ic_b),
		glm::vec3(ic_a, ic_b, 0),glm::vec3(ic_a, -ic_b, 0), glm::vec3(-ic_a, ic_b, 0), glm::vec3(-ic_a, -ic_b, 0),
		glm::vec3(ic_b, 0, ic_a), glm::vec3(-ic_b, 0, ic_a), glm::vec3(ic_b, 0, -ic_a), glm::vec3(-ic_b, 0, -ic_a),};
	std::vector<unsigned> inds = {
		0,4,6,0,8,4,0,6,9,0,2,8,0,9,2,1,6,4,1,11,6,1,4,10,1,3,11,1,10,3,4,8,10,6,11,9,5,8,2,5,10,8,5,3,10,5,7,3,5,2,7,3,7,11,7,9,11,7,2,9
	};
	glm::mat4 rotation = glm::rotate(-acos(glm::dot(Vertices[0], glm::vec3(0, 1, 0))), glm::vec3(1, 0, 0));
	for(size_t i = 0; i < Vertices.size(); i++) {
		Vertices[i] = rotation * glm::vec4(Vertices[i], 1);
	}
	//init(verts, inds, depth);


	for each (glm::vec3 vert in Vertices) {
		mesh.addVertex(vert);
	}
	
	for(size_t i = 0; i < inds.size(); i += 3) {
		Faces.push_back(new FaceTree(&mesh, inds[i], inds[i + 1], inds[i + 2]));
	}


	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &ColourBuffer);
	glGenBuffers(1, &NormalBuffer);
	glGenBuffers(1, &IndexBuffer);

	genIndices(depth);
}


Planet::~Planet() {
}

void Planet::draw(GLuint programID, Camera* camera) {
	//glm::rotate(-acos(glm::dot(Vertices[0], glm::vec3(0,1,0))), glm::vec3(1,0,0)) *
	glm::mat4 model =  glm::translate(glm::vec3(0));
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();
	glm::mat4 MVP = projection * view * model;

	GLuint vertPosID, colourPosID, mvpID;
	vertPosID = glGetAttribLocation(programID, "vertex_position");
	colourPosID = glGetAttribLocation(programID, "vertex_colour");
	mvpID = glGetUniformLocation(programID, "MVP");

	GLuint normPosID, mID, lightDirID, lightColID, lightPowID, camPosID;
	normPosID = glGetAttribLocation(programID, "vertex_normal");
	mID = glGetUniformLocation(programID, "M");
	lightDirID = glGetUniformLocation(programID, "light_direction");
	lightColID = glGetUniformLocation(programID, "light_colour");
	lightPowID = glGetUniformLocation(programID, "light_power");
	camPosID = glGetUniformLocation(programID, "camera_position");

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

	glm::vec3 lightDirection = glm::vec3(3, 0, 3);
	glm::vec3 lightColour = glm::vec3(1);
	float lightPower = 2;
	glUniformMatrix4fv(mID, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(lightDirID, 1, &lightDirection[0]);
	glUniform3fv(lightColID, 1, &lightColour[0]);
	glUniform1f(lightPowID, lightPower);
	glUniform3fv(camPosID, 1, &(camera->getPosition())[0]);

	glEnableVertexAttribArray(vertPosID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(vertPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(colourPosID);
	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
	glVertexAttribPointer(colourPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(normPosID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(normPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(vertPosID);
	glDisableVertexAttribArray(colourPosID);
}


