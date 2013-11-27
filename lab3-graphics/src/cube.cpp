/*
 * cube.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: AHMAD
 */

#include "cube.h"
using namespace std;
const GLfloat side = 2.0f;
GLuint indices[] = { 0, 1, 3, 2, 4, 5, 7, 6, 6, 5, 2, 1, 7, 6, 3, 2, 7, 4, 3, 0,
		4, 5, 0, 1 };
GLuint color_loc;
const vec3 red = vec3(1.0, 0.0, 0.0);
const vec3 green = vec3(0, 0.5, 0);
const vec3 blue = vec3(0.0, 0.0, 1.0);
const vec3 orange = vec3(1.0, 0.3, 0);
const vec3 white = vec3(1.0, 1.0, 1.0);
const vec3 yellow = vec3(1.0, 1.0, 0.0);

cube::cube() {
	// TODO Auto-generated constructor stub
}
void cube::createCube(GLuint program, GLfloat centerx, GLfloat centery,
		GLfloat centerz, GLint r, GLint c, GLint n) {
	create_cube(centerx, centery, centerz);

	centerX = centerx;
	centerY = centery;
	centerZ = centerz;
	mynumber = n;
//	cout << "center X : " << centerX << " center Y : " << centerY
//			<< " center Z : " << centerZ << endl;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, &vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glGenBuffers(1, &ibo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
			GL_STATIC_DRAW);

	color_loc = glGetUniformLocation(program, "color_out");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	row = r;
	column = c;

	transformation = identity();
	trans_loc = glGetUniformLocation(program, "transformation");

}

void cube::create_cube(GLfloat centerx, GLfloat centery, GLfloat centerz) {
	int i = 0;
	points[i++] = vec3(centerx - side / 2, centery - side / 2,
			centerz - side / 2);
	points[i++] = vec3(centerx + side / 2, centery - side / 2,
			centerz - side / 2);
	points[i++] = vec3(centerx + side / 2, centery - side / 2,
			centerz + side / 2);
	points[i++] = vec3(centerx - side / 2, centery - side / 2,
			centerz + side / 2);
	points[i++] = vec3(centerx - side / 2, centery + side / 2,
			centerz - side / 2);
	points[i++] = vec3(centerx + side / 2, centery + side / 2,
			centerz - side / 2);
	points[i++] = vec3(centerx + side / 2, centery + side / 2,
			centerz + side / 2);
	points[i++] = vec3(centerx - side / 2, centery + side / 2,
			centerz + side / 2);

}
vec3 getColor(int i) {
	switch (i) {
	case 0:
		return green;
	case 1:
		return blue;
	case 2:
		return yellow;
	case 3:
		return red;
	case 4:
		return white;
	case 5:
		return orange;
	}
	return yellow;
}

void cube::draw_cube() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	mat4 toOrigin = identity();
	toOrigin[0][3] = -centerX;
	toOrigin[1][3] = -centerY;
	toOrigin[2][3] = -centerZ;

	mat4 fromOrigin = identity();
	fromOrigin[0][3] = centerX;
	fromOrigin[1][3] = centerY;
	fromOrigin[2][3] = centerZ;
//	transformation = fromOrigin * transformation * toOrigin;
	glUniformMatrix4fv(trans_loc, 1, GL_TRUE, transformation);
	for (int i = 0; i < 6; i++) {
		glUniform3fv(color_loc, 1, getColor(i));
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT,
				BUFFER_OFFSET(i*4*sizeof(GLuint)) );
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLboolean cube::lie_inside(GLint x, GLint y, GLfloat width, GLfloat height,
		mat4 view, mat4 project) {
	vec4 newCenter = project * view * transformation
			* vec4(centerX, centerY, centerZ, 1);
	GLint newX = ceil(x - width / 2) / 26;
	GLint newY = ceil(-(y - height / 2)) / 25;
	if (newX >= ceil(newCenter[0]) - side / 2
			&& newX <= ceil(newCenter[0]) + side / 2)
		if (newY >= ceil(newCenter[1]) - side / 2
				&& newY <= ceil(newCenter[1]) + side / 2)
			return true;
	return false;
}

cube::~cube() {
// TODO Auto-generated destructor stub
}

