/*
    CS3388 Assignment 2
    Brandon Luu

    exercise2.cpp

    This is exercise 2, where we draw a dot plot.
*/

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// given function to generate double from [-1, 1]
static inline double frand() {
    double x = ((double)rand()) / (double)RAND_MAX;
    if (rand() % 2) {
        x *= -1.0;
    }
    return x;
}

int main(int argc, char* argv[])
{
    // check argument count
    if (argc < 4) {
        cout << "Invalid number of arguments, expected 3: N width height" << endl;
        return -1;
    }

    // seed random
    srand(time(0));

    // ./program n width height
    int n = stoi(argv[1]);
    int width = stoi(argv[2]);
    int height = stoi(argv[3]);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Exercise 2", NULL, NULL);

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
    glOrtho(-1.1, 1.1, -1.1, 1.1, NULL, NULL);

    /* Render here */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glClearColor(1., 1., 1., 1.);

    glBegin(GL_POINTS);
    glPointSize(2.0f);
    glColor3f(0, 0, 0);

    // algorithm for generating the dots
    int corner0 = rand() % 4; // 0 topleft, 1 topright, 2 botleft, 3 botright
    double x0 = frand();
    double y0 = frand();
    while (-1 < x0 && x0 < 1) x0 = frand();
    while (-1 < y0 && y0 < 1) y0 = frand();

    int corneri = corner0;
    double xi = x0;
    double yi = y0;
    for (int i = 1; i != n; i++) {
        // pick a random corner not diagonal to the current one
        // topleft
        if (corneri == 0) {
            int chance = rand() % 3;
            if (chance == 0) corneri = 0;
            else if (chance == 1) corneri = 1;
            else if (chance == 2) corneri = 2;
        }
        // topright
        else if (corneri == 1) {
            int chance = rand() % 3;
            if (chance == 0) corneri = 1;
            else if (chance == 1) corneri = 0;
            else if (chance == 2) corneri = 3;
        }
        // botleft
        else if (corneri == 2) {
            int chance = rand() % 3;
            if (chance == 0) corneri = 2;
            else if (chance == 1) corneri = 0;
            else if (chance == 2) corneri = 3;
        }
        // botright
        else if (corneri == 3) {
            int chance = rand() % 3;
            if (chance == 0) corneri = 3;
            else if (chance == 1) corneri = 1;
            else if (chance == 2) corneri = 2;
        }

        // get midway distance
        double xc;
        double yc;
        if (corneri == 0) {
            xc = -1;
            yc = -1;
        }
        else if (corneri == 1) {
            xc = 1;
            yc = -1;
        }
        else if (corneri == 2) {
            xc = -1;
            yc = 1;
        }
        else if (corneri == 3) {
            xc = 1;
            yc = 1;
        }

        double xh = xi + (xc - xi) / 2.0;
        double yh = yi + (yc - yi) / 2.0;

        // draw the point
        glVertex2d(xh, yh);

        // this new point is now the previous point for next loop
        xi = xh;
        yi = yh;
    }

    glEnd();

    glfwSwapBuffers(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}