#include "Planet.h"



void Planet::genIndices(unsigned depth) {
	size_t nVerts = mesh.size();
	Indices.clear();
	AtmoIndices.clear();
	std::vector<FaceTree*> lodFaces;
	for each (FaceTree* face in Faces) {
		face->getFaces(lodFaces, depth);
	}
	for each (FaceTree* face in lodFaces) {
		for(size_t i = 0; i < 3; i++) {
			Indices.push_back((*face)[i]);
		}
		AtmoIndices.push_back((*face)[0]);
		AtmoIndices.push_back((*face)[2]);
		AtmoIndices.push_back((*face)[1]);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(glm::vec3), mesh.Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ColourBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.Colours.size() * sizeof(glm::vec4), mesh.Colours.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.Normals.size() * sizeof(glm::vec3), mesh.Normals.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), Indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, AtmoIndBuffer);
	glBufferData(GL_ARRAY_BUFFER, AtmoIndices.size() * sizeof(unsigned), AtmoIndices.data(), GL_STATIC_DRAW);
}

void Planet::genTexture(sCubeMapProps properties) {
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, properties.texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *properties.texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	size_t TEXTURE_SIZE = properties.texture_size;

	std::vector<GLubyte> testData(TEXTURE_SIZE * TEXTURE_SIZE * 4, 128);
	std::vector<GLubyte> xData(TEXTURE_SIZE * TEXTURE_SIZE * 4, 255);

	for(size_t i = 0; i < 6; i++) {
		if(i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, properties.internal_format, 
						 TEXTURE_SIZE, TEXTURE_SIZE, 0, properties.format, GL_UNSIGNED_BYTE, &testData[0]);
		} else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, properties.internal_format,
						 TEXTURE_SIZE, TEXTURE_SIZE, 0, properties.format, GL_UNSIGNED_BYTE, &xData[0]);
		}
	}

	ShaderProgram program = ShaderProgram();
	program.addStage(GL_VERTEX_SHADER, properties.vertex_file_path);
	program.addStage(GL_FRAGMENT_SHADER, properties.fragment_file_path);
	program.linkProgram();
	GLuint programID = program.programID; // Compatablity

	glm::mat4 View, Projection, VP;
	Projection = glm::perspective(glm::pi<float>()/2+0.000001f, 1.0f, 0.1f, 10.0f);
	std::vector<glm::vec3> dirs = {
		glm::vec3(-1,0,0), glm::vec3(1,0,0),
		glm::vec3(0,1,0), glm::vec3(0,-1,0),
		glm::vec3(0,0,-1), glm::vec3(0,0,1)
	};
	std::vector<glm::vec3> ups = {
		glm::vec3(0,-1,0), glm::vec3(0,-1,0),
		glm::vec3(0,0,-1), glm::vec3(0,0,1),
		glm::vec3(0,-1,0),glm::vec3(0,-1,0)
	};

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glUseProgram(programID);

	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	GLuint vertPosID, vpID, seedID;
	vertPosID = glGetAttribLocation(programID, "vertex_position");
	vpID = glGetUniformLocation(programID, "VP");
	seedID = glGetUniformLocation(programID, "Seed");
	
	glUniform1i(seedID, Seed);

	glEnableVertexAttribArray(vertPosID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(vertPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDisable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 0);

	for(size_t i = 0; i < 6; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *properties.texture, 0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error("Framebuffer attachment failed for cubemap rendering");
		}

		View = glm::lookAt(glm::vec3(0), dirs[i], ups[i]);
		VP = Projection * View * glm::translate(glm::vec3(0));
		glUniformMatrix4fv(vpID, 1, GL_FALSE, &VP[0][0]);

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	glDisableVertexAttribArray(vertPosID);

	glEnable(GL_CULL_FACE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &frameBuffer);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glDeleteProgram(programID);
}

Planet::Planet(size_t seed = 0, unsigned depth) : mesh(seed) {
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

	/*std::vector<glm::vec3> Vertices = {
		glm::vec3(1,1,1),glm::vec3(1,1,-1),glm::vec3(1,-1,1),glm::vec3(1,-1,-1),
		glm::vec3(-1,1,1),glm::vec3(-1,1,-1),glm::vec3(-1,-1,1),glm::vec3(-1,-1,-1)
	};
	std::vector<unsigned> inds = {
		0,1,4,4,1,5,
		3,2,6,3,6,7,
		0,2,1,2,3,1,
		4,5,6,7,6,5,
		0,4,2,2,4,6,
		1,3,5,3,7,5
	};
	for(size_t i = 0; i < Vertices.size(); i++) {
		Vertices[i] = glm::normalize(Vertices[i]);
	}*/
	Seed = seed;

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
	glGenBuffers(1, &AtmoIndBuffer);

	genIndices(depth);
	sCubeMapProps textureProps;
	textureProps.texture = &planetTexture;
	textureProps.vertex_file_path = "cube_vertex.glsl";
	textureProps.fragment_file_path = "cube_fragment.glsl";
	genTexture(textureProps);

	textureProps.texture = &normalMap;
	textureProps.fragment_file_path = "normal_fragment.glsl";
	textureProps.internal_format = GL_RGB32F;
	textureProps.format = GL_RGB;
	genTexture(textureProps);

	textureProps.texture = &heightMap;
	textureProps.fragment_file_path = "height_fragment.glsl";
	textureProps.internal_format = GL_R8;
	textureProps.format = GL_RED;
	genTexture(textureProps);
}


Planet::~Planet() {
	for each (FaceTree* faceTree in Faces) {
		delete faceTree;
	}
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &ColourBuffer);
	glDeleteBuffers(1, &NormalBuffer);
	glDeleteBuffers(1, &IndexBuffer);

	glDeleteTextures(1, &planetTexture);
}

void Planet::draw(GLuint programID, Camera* camera) {
	glUseProgram(programID);

	//glm::rotate(-acos(glm::dot(Vertices[0], glm::vec3(0,1,0))), glm::vec3(1,0,0)) *
	glm::mat4 model = glm::translate(glm::vec3(0, 0, 0));// *glm::rotate(glm::radians(-23.5f), glm::vec3(1, 0, 0)) *glm::rotate(((float)glfwGetTime() * 2 * glm::pi<float>()) / 60, glm::vec3(0, 1, 0));
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();
	glm::mat4 MVP = projection * view * model;
	Model = model;

	GLuint vertPosID, mvpID, textureID, normalMapID;
	vertPosID = glGetAttribLocation(programID, "vertex_position");
	mvpID = glGetUniformLocation(programID, "MVP");
	textureID = glGetUniformLocation(programID, "cube_texture");
	normalMapID = glGetUniformLocation(programID, "normal_map");

	GLuint normPosID, mID, lightDirID, lightColID, lightPowID, camPosID, isAtmosID, seedID;
	//normPosID = glGetAttribLocation(programID, "vertex_normal");
	mID = glGetUniformLocation(programID, "M");
	lightDirID = glGetUniformLocation(programID, "light_direction");
	lightColID = glGetUniformLocation(programID, "light_colour");
	lightPowID = glGetUniformLocation(programID, "light_power");
	camPosID = glGetUniformLocation(programID, "camera_position");
	isAtmosID = glGetUniformLocation(programID, "isAtmos");
	seedID = glGetUniformLocation(programID, "Seed");

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);

	glm::vec3 lightDirection = glm::vec3(3, 0, 3);
	glm::vec3 lightColour = glm::vec3(1);
	float lightPower = 2;
	glUniformMatrix4fv(mID, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(lightDirID, 1, &lightDirection[0]);
	glUniform3fv(lightColID, 1, &lightColour[0]);
	glUniform1f(lightPowID, lightPower);
	glUniform3fv(camPosID, 1, &(camera->getPosition())[0]);
	glUniform1i(isAtmosID, 0);
	glUniform1i(seedID, Seed);

	glUniform1i(textureID, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, planetTexture);

	glUniform1i(normalMapID, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, normalMap);

	glEnableVertexAttribArray(vertPosID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(vertPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*glEnableVertexAttribArray(normPosID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(normPosID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AtmoIndBuffer);

	model = model * glm::scale(glm::vec3(1.1));
	MVP = projection * view * model;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(mID, 1, GL_FALSE, &model[0][0]);
	glUniform1i(isAtmosID, 1);

	glDrawElements(GL_TRIANGLES, AtmoIndices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(vertPosID);
	//glDisableVertexAttribArray(normPosID);
}


#ifdef _DEBUG
void Planet::drawNormals(Camera * camera) {
	static ShaderProgram program = ShaderProgram();
	if(!program.isLinked()) {
		program.addStage(GL_VERTEX_SHADER, "passthrough_vertex.glsl");
		program.addStage(GL_GEOMETRY_SHADER, "normals_geometry.glsl");
		program.addStage(GL_FRAGMENT_SHADER, "passthrough_fragment.glsl");
		program.linkProgram();
	}
	glUseProgram(program.programID);
	glm::mat4 model = Model;
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();
	glm::mat4 MVP = projection * view * model;

	GLuint vertposID, normalmapID, normallengthID, mvpID;

	vertposID = glGetAttribLocation(program.programID, "vertex_position");
	normalmapID = glGetUniformLocation(program.programID, "normal_map");
	normallengthID = glGetUniformLocation(program.programID, "normal_length");
	mvpID = glGetUniformLocation(program.programID, "MVP");

	float normalLength = 0.1f;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(normallengthID, normalLength);

	glUniform1i(normalmapID, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, normalMap);

	glEnableVertexAttribArray(vertposID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(vertposID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(vertposID);
}
#endif // DEBUG



