#include <iostream>
#include <GL/glut.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>
#include <string>

GLfloat vertices[8][3] =
{
        {-0.5, -0.5, -0.5},
        {0.5, -0.5, -0.5},
        {0.5, 0.5, -0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},
        {0.5, 0.5, 0.5},
        {-0.5, 0.5, 0.5}
};

GLint faces[6][4] =
{
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 1, 5, 4},
        {2, 3, 7, 6},
        {0, 3, 7, 4},
        {1, 2, 6, 5}
};

GLfloat colors[6][3] = 
{
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {1.0, 1.0, 0.0},
        {1.0, 0.0, 1.0},
        {0.0, 1.0, 1.0}
};

// Нормали 
GLfloat normals[6][3] = 
{
        {0.0, 0.0, -1.0},
        {0.0, 0.0, 1.0},
        {-1.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, -1.0, 0.0}
};

struct TexCoord 
{
    float x, y;
};

// Координаты текстур для каждой вершины куба
TexCoord textureCoordinates[6][4] = 
{
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} },
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} },
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} },
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} },
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} },
        { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} }
};

GLuint textureID;

float angle = 0.0;
float currentRotationAngle = 0.0;
float speed = 0.5;
bool isRotating = true;
float scaleFactor = 0.5;

GLfloat lightPosition[4] = {0.0, 0.0, -2.0, 0.0};

bool isTransparent = false;
bool useTextures = false;

void loadTexture()
{
    int width, height, nrChannels;
    unsigned char* image = stbi_load("texture.jpg", &width, &height, &nrChannels, STBI_rgb_alpha); // Текстура

    if (image)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        stbi_image_free(image);
    }
    else
    {
        std::cout << "Error loading texture" << std::endl;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -5.0);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glRotatef(-currentRotationAngle, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glPushMatrix();
    glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(0.1, 20, 20);
    glPopMatrix();

    glPopMatrix();

    glRotatef(currentRotationAngle, 1.0, 1.0, 0.0);
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    currentRotationAngle += speed;

    if (isTransparent)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if (useTextures)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    for (size_t i = 0; i < 6; i++)
    {
        if (!useTextures)
        {
            glColor4f(colors[i][0], colors[i][1], colors[i][2], isTransparent ? 0.5f : 1.0f);
        }
        glBegin(GL_QUADS);
        glNormal3fv(normals[i]);
        for (int j = 0; j < 4; j++)
        {
            if (useTextures)
            {
                glTexCoord2f(textureCoordinates[i][j].x, textureCoordinates[i][j].y);
            }
            glVertex3fv(vertices[faces[i][j]]);
        }
        glEnd();
    }

    if (useTextures)
    {
        glDisable(GL_TEXTURE_2D);
    }

    if (isTransparent)
    {
        glDisable(GL_BLEND);
    }

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        isRotating = !isRotating;
        if (!isRotating)
        {
            speed = 0.0;
        }
        else
        {
            speed = 0.5;
        }
        break;
    case 't':
        isTransparent = !isTransparent;
        break;
    case 'u':
        useTextures = !useTextures;
        break;
    }
}

int main(int argc, char* argv[])
{
    std::cout << argv[0] << '\n';
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);

    glutCreateWindow("Cube");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    loadTexture(); // Загрузка текстуры
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}