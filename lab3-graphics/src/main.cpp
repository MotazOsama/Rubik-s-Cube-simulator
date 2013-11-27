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
GLboolean shuffling = false, colTurn = true;
GLint number_of_shuffle = 1, current_sign = 1;
mat4 cube_rotation;
cube myCubes[3][3][3];

void printCubesNo() {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                cout << myCubes[x][y][z].mynumber << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void rotateRowV(int rowNo, bool clockwise, int angle) {
    int sign = 1;
    if (clockwise)
        sign = -1;
    cout << "rotating rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "myname is " << myCubes[i][rowNo][j].mynumber << endl;
            myCubes[i][rowNo][j].transformation = RotateY(sign * angle)
                    * myCubes[i][rowNo][j].transformation;

        }
    }
    cout << "rotating rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << endl;
}

void rotateColumnV(int colNo, bool clockwise, int angle) {
    int sign = 1;
    if (clockwise)
        sign = -1;
    cout << "rotating ccccccccccccccccccccccccccccccccccccccc" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "myname is " << myCubes[colNo][i][j].mynumber << endl;
            myCubes[colNo][i][j].transformation = RotateX(sign * angle)
                    * myCubes[colNo][i][j].transformation;
        }

    }
    cout << "rotating ccccccccccccccccccccccccccccccccccccccc" << endl;

}

void rotateCol(int colNo, bool clockwise) {
    cube temp[3][3][3];
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                if (colNo == x) {
                    cout << "**column>>" << myCubes[colNo][y][z].mynumber
                            << endl;

                    memcpy(&temp[colNo][z][y], &myCubes[colNo][y][z],
                            sizeof(cube));
                } else {
                    memcpy(&temp[x][y][z], &myCubes[x][y][z], sizeof(cube));
                }
            }
        }
    }
    cout << "llllllllllllllllllllllllllllll" << endl;
    printCubesNo();
    cout << "oooooooooooooooooooooooooooooo" << endl;

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
    cout << "ppppppppppppppppppppppppppppppppppp" << endl;

}

void rotateRow(int rowNo, bool clockwise) {
    cout << "lllllllllllllllllllllllllllllllllllllll" << endl;
    printCubesNo();
    cout << "*************************************" << endl;
    cube temp[3][3][3];
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                if (rowNo == y) {
                    cout << "**row>>" << myCubes[2 - z][rowNo][2 - x].mynumber
                            << endl;
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
    cout << "pppppppppppppppppppppppppppppppppppppppp" << endl;
}

void update(int value) {
    if (current_shuffle_angle * 10 <= 90) {
        bool anti = false;
        if (current_sign == -1)
            anti = true;
        if (colTurn) {
            rotateColumnV(dim, anti, 10);
        } else {
            rotateRowV(dim, anti, 10);
        }
        shuffling = true;
        current_shuffle_angle++;
        glutTimerFunc(100, update, 0);
    } else {
        bool clockWize = false;
//      int dim = number_of_shuffle % 3 == 0 ? 3 : number_of_shuffle % 3;
        if (current_sign == -1)
            clockWize = true;

        if (colTurn)
            rotateCol(dim, clockWize);
        else
            rotateRow(dim, clockWize);

        if (number_of_shuffle < 6) {
            int first = rand() + 1;
            int second = rand() + 1;

            if (first - second < 0) {
                current_sign = -1;
            } else {
                current_sign = 1;
            }

            current_shuffle_angle = 1;
            dim = (dim + 1) % 3;
            colTurn = !colTurn;
            number_of_shuffle++;
            glutTimerFunc(100, update, 0);
            printCubesNo();
            cout << "****************************************" << endl;
        } else {
            shuffling = false;
        }
    }
    glutPostRedisplay(); //Tell GLUT that the scene has changed

}

void init() {
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
    cout << "****************************************" << endl;
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
    if (!shuffling) {
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
    if (!shuffling) {
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
            rotateRowV(0, true, 90);
            rotateRow(0, true);
            break;
        case '2':
            rotateRowV(1, false, 90);
            rotateRow(1, false);
            break;
        case '3':
            rotateRowV(2, false, 90);
            rotateRow(2, false);
            break;
        case '4':
            rotateColumnV(0, true, 90);
            rotateCol(0, true);
            break;
        case '5':
            rotateColumnV(1, false, 90);
            rotateCol(1, false);
            break;
        case '6':
            rotateColumnV(2, false, 90);
            rotateCol(2, false);
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
