#include "Planet.h"



void Planet::genBuffers() {
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ColourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
	glBufferData(GL_ARRAY_BUFFER, Colours.size() * sizeof(glm::vec3), Colours.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), Indices.data(), GL_STATIC_DRAW);
}

Planet::Planet(size_t seed = 0, unsigned depth=0){
	const double ic_a = 0.52573111211913360602566908484789;
	const double ic_b = 0.85065080835203993218154049706302;
	std::vector<glm::vec3> verts = {glm::vec3(0, ic_a, ic_b), glm::vec3(0, ic_a, -ic_b), glm::vec3(0, -ic_a, ic_b), glm::vec3(0, -ic_a, -ic_b),
		glm::vec3(ic_a, ic_b, 0),glm::vec3(ic_a, -ic_b, 0), glm::vec3(-ic_a, ic_b, 0), glm::vec3(-ic_a, -ic_b, 0),
		glm::vec3(ic_b, 0, ic_a), glm::vec3(-ic_b, 0, ic_a), glm::vec3(ic_b, 0, -ic_a), glm::vec3(-ic_b, 0, -ic_a),};
	std::vector<unsigned> inds = {
		0,4,6,0,8,4,0,6,9,0,2,8,0,9,2,1,6,4,1,11,6,1,4,10,1,3,11,1,10,3,4,8,10,6,11,9,5,8,2,5,10,8,5,3,10,5,7,3,5,2,7,3,7,11,7,9,11,7,2,9
	};
	glm::mat4 rotation = glm::rotate(-acos(glm::dot(verts[0], glm::vec3(0, 1, 0))), glm::vec3(1, 0, 0));
	for(size_t i = 0; i < verts.size(); i++) {
		verts[i] = rotation * glm::vec4(verts[i], 1);
	}
	init(verts, inds, depth);

	Colours.resize(Vertices.size());
	for(size_t i = 0; i < Vertices.size(); i++) {
		glm::vec3 vert = Vertices[i];
		double point = (Noise3d::getPoint(seed, 0.75, vert.x, vert.y, vert.z)*0.4 
						+ Noise3d::getPoint(seed + 1, 0.5, vert.x, vert.y, vert.z)*0.5 
						+ Noise3d::getPoint(seed + 2, 0.25, vert.x, vert.y, vert.z)*0.25
						+ Noise3d::getPoint(seed + 3, 0.1, vert.x, vert.y, vert.z)*0.1) / 1.25;
		// std::cout << point << std::endl;
		double latitude = 90 - glm::degrees(glm::acos(glm::dot(vert, glm::vec3(0, 1, 0))));
		double temp = glm::cos(glm::radians(latitude)) * 50 - 25;
		if(point < 0.55) {
			Colours[i] = glm::vec3(0, 0.22, 0.48);
		} else {
			Colours[i] = glm::vec3(0, (102. / 255)*point, 0);
		}
		if(temp < 0) {
			Colours[i] = glm::mix(Colours[i], glm::vec3(1), abs(temp/20)+0.5);
		}
		//Colours[i] = glm::vec3(0, 0, abs(latitude / 90));
	}
	
	genBuffers();
}


Planet::~Planet() {
}

void Planet::draw(GLuint programID, Camera& camera) {
	//glm::rotate(-acos(glm::dot(Vertices[0], glm::vec3(0,1,0))), glm::vec3(1,0,0)) *
	glm::mat4 model =  glm::translate(glm::vec3(0));
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 projection = camera.getProjectionMatrix();
	glm::mat4 MVP = projection * view * model;

	GLuint vertPosID, colourPosID, mvpID;
	vertPosID = glGetAttribLocation(programID, "vertex_position");
	colourPosID = glGetAttribLocation(programID, "vertex_colour");
	mvpID = glGetUniformLocation(programID, "MVP");

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(vertPosID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(vertPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(colourPosID);
	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
	glVertexAttribPointer(colourPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(vertPosID);
	glDisableVertexAttribArray(colourPosID);
}
