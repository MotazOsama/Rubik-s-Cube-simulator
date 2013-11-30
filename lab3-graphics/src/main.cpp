/*
 * main.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: AHMAD
 */
#include <math.h>
#include "include/Angel.h"
#include "cube.h"
#include <sstream>
#include <string.h> // this should be already included in <sstream>
using namespace std;

GLuint view_loc, proj_loc, rota_loc;
GLuint program;
GLfloat window_width = 800;
GLfloat window_height = 500;
mat4 rotateX, rotateY, rotateZ;
const int number_of_cubes = 27;
GLint zoom = 70, dim = 0;
GLint current_shuffle_angle = 1;
GLboolean shuffling = false, colTurn = true, rotating_face = false, rotating_z =
		false;
GLint number_of_shuffle = 1, current_sign = 1;
mat4 cube_rotation;
cube myCubes[3][3][3];

void printCubesNo() {
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				//cout << myCubes[x][y][z].mynumber << " ";
			}
			//cout << endl;
		}
		//cout << endl;
	}
}

void rotateRowV(int rowNo, bool clockwise, int angle) {
	int sign = 1;
	if (clockwise)
		sign = -1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			//cout << "myname is " << myCubes[i][rowNo][j].mynumber << endl;
			myCubes[i][rowNo][j].transformation = RotateY(sign * angle)
					* myCubes[i][rowNo][j].transformation;

		}
	}
}
void rotateColumnV(int colNo, bool clockwise, int angle) {
	int sign = 1;
	if (clockwise)
		sign = -1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myCubes[colNo][i][j].transformation = RotateX(sign * angle)
					* myCubes[colNo][i][j].transformation;
		}

	}
}
void rotateZV(int zNo, bool clockwise, int angle) {
	int sign = -1;
	if (clockwise)
		sign = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myCubes[i][j][zNo].transformation = RotateZ(sign * angle)
					* myCubes[i][j][zNo].transformation;
		}

	}
}
void rotateCol(int colNo, bool clockwise) {
	cube temp[3][3][3];
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				if (colNo == x) {
					memcpy(&temp[colNo][z][y], &myCubes[colNo][y][z],
							sizeof(cube));
				} else {
					memcpy(&temp[x][y][z], &myCubes[x][y][z], sizeof(cube));
				}
			}
		}
	}
	printCubesNo();
	if (!clockwise) {
		for (int y = 0; y < 3; y++) {

			cube temploc;
			memcpy(&temploc, &temp[colNo][y][0], sizeof(cube));
			memcpy(&temp[colNo][y][0], &temp[colNo][y][2], sizeof(cube));
			memcpy(&temp[colNo][y][2], &temploc, sizeof(cube));
		}
	} else {
		for (int z = 0; z < 3; z++) {
			cube temploc;
			memcpy(&temploc, &temp[colNo][0][z], sizeof(cube));
			memcpy(&temp[colNo][0][z], &temp[colNo][2][z], sizeof(cube));
			memcpy(&temp[colNo][2][z], &temploc, sizeof(cube));
		}
	}
	memcpy(myCubes, temp, sizeof(cube) * 3 * 3 * 3);
	printCubesNo();

}
bool checkCompleted() {
	int count = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				if (myCubes[x][y][z].mynumber != count) {
					return false;
				}
				count++;
			}
		}
	}
	return true;
}
void rotateRow(int rowNo, bool clockwise) {
	printCubesNo();
	cube temp[3][3][3];
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				if (rowNo == y) {
					memcpy(&temp[x][rowNo][z], &myCubes[2 - z][rowNo][2 - x],
							sizeof(cube));
				} else {
					memcpy(&temp[x][y][z], &myCubes[x][y][z], sizeof(cube));
				}
			}
		}
	}
	if (!clockwise) {
		for (int x = 0; x < 3; x++) {
			cube temploc;
			memcpy(&temploc, &temp[x][rowNo][0], sizeof(cube));
			memcpy(&temp[x][rowNo][0], &temp[x][rowNo][2], sizeof(cube));
			memcpy(&temp[x][rowNo][2], &temploc, sizeof(cube));
		}
	} else {
		for (int z = 0; z < 3; z++) {
			cube temploc;
			memcpy(&temploc, &temp[0][rowNo][z], sizeof(cube));
			memcpy(&temp[0][rowNo][z], &temp[2][rowNo][z], sizeof(cube));
			memcpy(&temp[2][rowNo][z], &temploc, sizeof(cube));
		}
	}
	memcpy(myCubes, temp, sizeof(cube) * 3 * 3 * 3);
	printCubesNo();
}
void rotateZstripe(int depth, bool clockwise) {
	printCubesNo();
	cube temp[3][3][3];
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				if (depth == z) {
					memcpy(&temp[y][x][depth], &myCubes[x][y][depth],
							sizeof(cube));
				} else {
					memcpy(&temp[x][y][z], &myCubes[x][y][z], sizeof(cube));
				}
			}
		}
	}
	if (!clockwise) {
		for (int x = 0; x < 3; x++) {
			cube temploc;
			memcpy(&temploc, &temp[x][0][depth], sizeof(cube));
			memcpy(&temp[x][0][depth], &temp[x][2][depth], sizeof(cube));
			memcpy(&temp[x][2][depth], &temploc, sizeof(cube));
		}
	} else {
		for (int y = 0; y < 3; y++) {
			cube temploc;
			memcpy(&temploc, &temp[0][y][depth], sizeof(cube));
			memcpy(&temp[0][y][depth], &temp[2][y][depth], sizeof(cube));
			memcpy(&temp[2][y][depth], &temploc, sizeof(cube));
		}
	}
	memcpy(myCubes, temp, sizeof(cube) * 3 * 3 * 3);
	printCubesNo();
}

void update(int value) {
	if (current_shuffle_angle * 10 <= 90) {
		bool anti = false;
		if (current_sign == -1)
			anti = true;
		if (!rotating_z) {
			if (colTurn) {
				rotateColumnV(dim, anti, 10);
			} else {
				rotateRowV(dim, anti, 10);
			}
		} else {
			rotateZV(dim, anti, 10);
		}
		current_shuffle_angle++;
		glutTimerFunc(100, update, 0);
	} else if (shuffling) {
		bool clockWize = false;
		if (current_sign == -1)
			clockWize = true;

		if (colTurn)
			rotateCol(dim, clockWize);
		else
			rotateRow(dim, clockWize);

		if (number_of_shuffle < 10) {
			srand(rand()+100);
			int first = rand() + 1;
			int second = rand() + 1;

			if (first - second < 0) {
				current_sign = -1;
			} else {
				current_sign = 1;
			}

			current_shuffle_angle = 1;
			dim = (rand() + 3) % 3;
			colTurn = !colTurn;
			number_of_shuffle++;
			glutTimerFunc(100, update, 0);
			printCubesNo();
		} else {
			shuffling = false;
		}
	} else if (rotating_face) {
		bool clockWize = false;
		if (current_sign == -1)
			clockWize = true;

		if (colTurn)
			rotateCol(dim, clockWize);
		else
			rotateRow(dim, clockWize);
		rotating_face = false;
		printCubesNo();
		if (checkCompleted())
			cout << "Congratulations  ^_^" << endl;
	} else if (rotating_z) {
		bool clockWize = false;
		if (current_sign == -1)
			clockWize = true;
		rotateZstripe(dim, clockWize);
		rotating_z = false;
		printCubesNo();
		if (checkCompleted())
			cout << "Congratulations  ^_^" << endl;

	}
	glutPostRedisplay(); //Tell GLUT that the scene has changed

}

void init() {
	shuffling = true;
	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	float inX = -3, inY = -3, inZ = 3;
	int row = 1, column = 1;
	int counter = 0;
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			for (int z = 0; z < 3; ++z) {
				myCubes[x][y][z].createCube(program, inX, inY, inZ, row, column,
						counter);
				inZ -= 3;
				counter++;
			}
			inZ = 3;
			row++;
			inY += 3;
		}
		inY = -3;
		row = 1;
		column++;
		inX += 3;
	}
	view_loc = glGetUniformLocation(program, "model_view");
	proj_loc = glGetUniformLocation(program, "projection");
	rota_loc = glGetUniformLocation(program, "cube_rotate");
	glClearColor(0.0, 0.0, 0.0, 0.0); // white background

	rotateX = identity();
	rotateY = identity();
	rotateZ = identity();
	cube_rotation = identity();
	glutTimerFunc(3000, update, 0);
	printCubesNo();
	//cout << "****************************************" << endl;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 eye = vec4(5, 5, 12, 1.0f);
	vec4 at = vec4(0, 0, 0, 1.0f);
	vec4 up = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	mat4 view = LookAt(eye, at, up);
	mat4 project = Perspective(zoom, window_width / window_height, 0.1f, 50.0f);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(proj_loc, 1, GL_TRUE, project);
	if (!shuffling && !rotating_face && !rotating_z) {
		cube_rotation = rotateX * rotateY * rotateZ * cube_rotation;
	}
	glUniformMatrix4fv(rota_loc, 1, GL_TRUE, cube_rotation);
	for (int k = 0; k < 3; ++k)
		for (int j = 0; j < 3; ++j)
			for (int i = 0; i < 3; ++i) {

				myCubes[k][j][i].draw_cube();

			}
	rotateX = identity();
	rotateY = identity();
	rotateZ = identity();

	glutSwapBuffers();
}

/**
 * Callback for the keyboard handle.
 */

void keyboardHandle(unsigned char key, int x, int y) {
	bool alt_press = false;
	if (glutGetModifiers() == GLUT_ACTIVE_ALT) {
		alt_press = true;
	}
	if (!shuffling && !rotating_face && !rotating_z) {
		switch (key) {
		case 033:
			exit(EXIT_SUCCESS);
			break;
		case 'w':
			rotateX = RotateX(-10);
			break;
		case 's':
			rotateX = RotateX(10);
			break;
		case 'a':
			rotateY = RotateY(-10);
			break;
		case 'd':
			rotateY = RotateY(10);
			break;
		case 'z':
			rotateZ = RotateZ(-10);
			break;
		case 'x':
			rotateZ = RotateZ(10);
			break;
		case '[':
			zoom -= 10;
			break;
		case ']':
			zoom += 10;
			break;
		case '1':
			current_shuffle_angle = 1;
			dim = 0;
			colTurn = false;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);
			break;
		case '2':
			current_shuffle_angle = 1;
			dim = 1;
			colTurn = false;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);

			break;
		case '3':
			current_shuffle_angle = 1;
			dim = 2;
			colTurn = false;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);
			break;
		case '4':
			current_shuffle_angle = 1;
			dim = 0;
			colTurn = true;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);
			break;
		case '5':
			rotateColumnV(1, false, 90);
			rotateCol(1, false);
			current_shuffle_angle = 1;
			dim = 1;
			colTurn = true;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);
			break;
		case '6':
			current_shuffle_angle = 1;
			dim = 2;
			colTurn = true;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_face = true;
			glutTimerFunc(25, update, 0);
			break;
		case '7':
			current_shuffle_angle = 1;
			dim = 0;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_z = true;
			glutTimerFunc(25, update, 0);
			break;
		case '8':
			current_shuffle_angle = 1;
			dim = 1;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_z = true;
			glutTimerFunc(25, update, 0);
			break;
		case '9':
			current_shuffle_angle = 1;
			dim = 2;
			if (alt_press)
				current_sign = 1;
			else
				current_sign = -1;
			rotating_z = true;
			glutTimerFunc(25, update, 0);
			break;

		}

		glutPostRedisplay();
	}
}

/**
 * Callback for Mouse Handle.
 */
void mouseClickHandle(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {

		}
	}
}
/**
 * Callback for Mouse Motion without any clicks.
 */
void mouseMotionHandle(int x, int y) {

}

/**
 * Callback for resizing window.
 */
void reshape(int w, int h) {
	window_width = w;
	window_height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); // Set our viewport to the size of our window
//  glViewport(w / 2 - 250, h - 500, (GLsizei) 500, (GLsizei) 500); // Set our viewport to the size of our window
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Connect 4");
	glewInit();
	glEnable(GL_DEPTH_TEST);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardHandle);
	glutMouseFunc(mouseClickHandle);
	glutPassiveMotionFunc(mouseMotionHandle);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
