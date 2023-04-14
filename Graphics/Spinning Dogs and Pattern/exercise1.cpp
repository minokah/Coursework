/*
    CS3388 Assignment 2
    Brandon Luu

    exercise1.cpp

    This is exercise 1, where we are to draw a few spinning dog figures.
*/

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    // read file using fstream and store it into the vectors
    ifstream file("dog.txt");

    vector<float> x;
    vector<float> y;
    
    float first, second;
    while (file >> first >> second) {
        x.push_back(first);
        y.push_back(second);
    }

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Exercise 1", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // set the world space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0., 60., 0., 60., -1, 1);

    // spin in degrees
    double spin = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);

        // draw dogs every 45 degrees with a radius of 25
        // rotate then translate them
        glMatrixMode(GL_MODELVIEW);
        for (int i = 0; i != 8; i++) {
            glLoadIdentity();
            glTranslated(30., 30., 0.);
            
            // I originally used this formula, but I'd figured we would want to see usage of glRotate and glTranslate more...
            //glTranslated(30. + 25 * cos((i * 45) * 3.14 / 180), 30. + 25 * sin((i * 45) * 3.14 / 180), 0);

            glRotated(i * 45, 0, 0, 1.0);
            glTranslated(0., 25., 0.);
            glRotated(spin, 0.0, 0.0, 1.0);

            glBegin(GL_LINE_STRIP);
            glColor3ub(0x0, 0x0, 0x0);
            for (int i = 0; i != x.size(); i++) glVertex2f(x.at(i), y.at(i));
            glEnd();
        }

        spin++;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}