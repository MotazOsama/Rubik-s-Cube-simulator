/*
 * cube.h
 *
 *  Created on: Nov 23, 2013
 *      Author: AHMAD
 */
#include "include/Angel.h"
#ifndef CUBE_H_
#define CUBE_H_

class cube {
public:
	vec3 points[8];
	mat4 transformation;
	GLuint trans_loc;
	GLint row, column;
	GLfloat centerX, centerY, centerZ;
	GLint mynumber;
	cube();
	void createCube(GLuint program, GLfloat centerx, GLfloat centery,
			GLfloat centerz, GLint r, GLint c, GLint n);
	GLboolean lie_inside(GLint x, GLint y, GLfloat width, GLfloat height,
			mat4 view, mat4 project);
	void draw_cube();
	virtual ~cube();

private:
	GLuint vao;
	GLuint vboID[1];
	GLuint ibo[1];
	void create_cube(GLfloat centerx, GLfloat centery, GLfloat centerz);
};

#endif /* CUBE_H_ */
