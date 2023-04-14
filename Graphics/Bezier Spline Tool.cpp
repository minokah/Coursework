/*
    CS3388 Assignment 3
    Brandon Luu

    splinetool.cpp

    This program creates a window where the user can create and manipulate a cubic bezier spline.
*/

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// provided structures for the points and nodes
struct Point {
    float x = 0, y = 0;
};

struct Node : Point {
    bool hasHandle1 = false, hasHandle2 = false;
    Point position, handle1, handle2;
};

// cubic bezier function
float bezier3(float a, float c1, float c2, float b, float t) {
    float value = pow(1 - t, 3) * a + 3 * pow(1 - t, 2) * t * c1 + 3 * (1 - t) * pow(t, 2) * c2 + pow(t, 3) * b;
    return value;
}

int main(int argc, char** argv)
{
    // check argument count for width and height
    if (argc < 3) {
        cout << "Invalid number of arguments, expected: width height" << endl;
    }

    // create window
    int width, height;
    width = stoi(argv[1]);
    height = stoi(argv[2]);

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Spline Tool", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // set glfw stuff...
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glViewport(0, 0, width, height);
    glfwWindowHint(GLFW_SAMPLES, 4);

    vector<Node> nodes;

    glMatrixMode(GL_MODELVIEW);

    // variables
    bool cleared = false; // clear button held
    bool dragging = false; // lmb held

    float deadzone = 20.f; // dragging and "collision" deadzone
    // if you are dragging too fast and it "lets go", increase this
    // if you want to be able to drag control points closer to the node, decrease this

    int dragIndex = -1; // drag index so we don't accidentally drag other indices at the same time

    int dragType = -1; // drag type so we don't accidentally drag the other entities of that indice at the same time
    // -1: default/none, 0: node, 1: handle1, 2: handle2

    float dragSlope = 0; // remember slope for when we drag node
    Point dragOrigin, h1Origin, h2Origin; // remmeber the origin for offsetting

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // keyboard functionality
        int eState = glfwGetKey(window, GLFW_KEY_E);
        if (eState == GLFW_PRESS) {
            if (!cleared) {
                nodes.clear();
                cleared = true;
            }
        }
        else if (eState == GLFW_RELEASE) cleared = false;

        // mouse functionality
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouseState == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            ypos = height - ypos;

            // check all points to drag
            for (int i = 0; i != nodes.size(); i++) {
                Point* position = &nodes[i].position;
                Point* handle1 = &nodes[i].handle1;
                Point* handle2 = &nodes[i].handle2;

                // move node, log the positions for our control points and node to offset
                if (xpos < position->x + deadzone && xpos > position->x - deadzone && ypos < position->y + deadzone && ypos > position->y - deadzone) {
                    dragging = true;
                    if (dragIndex == -1) {
                        dragIndex = i;
                        dragType = 0;
                        dragSlope = (handle1->y - position->y) / (handle1->x - position->x);
                        dragOrigin.x = position->x;
                        dragOrigin.y = position->y;
                        h1Origin.x = handle1->x;
                        h1Origin.y = handle1->y;
                        h2Origin.x = handle2->x;
                        h2Origin.y = handle2->y;
                    }

                    // this is to prevent "sticky" nodes where we accidentally grab another node
                    if (dragIndex == i && dragType == 0) {
                        position->x = xpos;
                        position->y = ypos;

                        handle1->x = h1Origin.x - (dragOrigin.x - position->x);
                        handle1->y = h1Origin.y - (dragOrigin.y - position->y);
                        handle2->x = h2Origin.x - (dragOrigin.x - position->x);
                        handle2->y = h2Origin.y - (dragOrigin.y - position->y);
                    }
                }

                // move handle1
                else if (nodes[i].hasHandle1 && xpos < handle1->x + deadzone && xpos > handle1->x - deadzone && ypos < handle1->y + deadzone && ypos > handle1->y - deadzone) {
                    dragging = true;
                    if (dragIndex == -1) {
                        dragIndex = i;
                        dragType = 1;
                    }
                    if (dragIndex == i && dragType == 1) {
                        handle1->x = xpos;
                        handle1->y = ypos;

                        // provided formula
                        float slope = (handle1->y - position->y) / (handle1->x - position->x);
                        float dist = sqrt(pow(handle1->x - position->x, 2) + pow(handle1->y - position->y, 2));

                        float hOffset = sqrt(pow(dist, 2) / (1 + pow(slope, 2)));
                        if (handle1->x > position->x) handle2->x = position->x - hOffset;
                        else handle2->x = position->x + hOffset;

                        handle2->y = position->y - (handle1->y - position->y);
                    }
                }

                // move handle2
                else if (nodes[i].hasHandle2 && xpos < handle2->x + deadzone && xpos > handle2->x - deadzone && ypos < handle2->y + deadzone && ypos > handle2->y - deadzone) {
                    dragging = true;
                    if (dragIndex == -1) {
                        dragIndex = i;
                        dragType = 2;
                    }
                    if (dragIndex == i && dragType == 2) {
                        handle2->x = xpos;
                        handle2->y = ypos;

                        // provided formula
                        float slope = (handle2->y - position->y) / (handle2->x - position->x);
                        float dist = sqrt(pow(handle1->x - position->x, 2) + pow(handle1->y - position->y, 2));

                        float hOffset = sqrt(pow(dist, 2) / (1 + pow(slope, 2)));
                        if (handle2->x > position->x) handle1->x = position->x - hOffset;
                        else handle1->x = position->x + hOffset;

                        handle1->y = position->y - (handle2->y - position->y);
                    }
                }
            }

            // create node if not dragging
            if (!dragging) {
                Node point;
                point.position.x = xpos;
                point.position.y = ypos;

                // default offsets are +- 50px
                point.handle1.x = xpos;
                point.handle1.y = ypos + 50;

                point.handle2.x = xpos;
                point.handle2.y = ypos - 50;

                point.hasHandle1 = true;

                // find closest endpoint
                Node* closestEnd = NULL;
                float closestDist = INT_MAX;
                int index = 0;
                for (int i = 0; i != nodes.size(); i++) {
                    if (!nodes[i].hasHandle2) {
                        Point* position = &nodes[i].position;
                        float dist = sqrt(pow(xpos - position->x, 2) + pow(ypos - position->y, 2));

                        if (dist < closestDist) {
                            closestEnd = &nodes[i];
                            closestDist = dist;
                            index = i;
                        }
                    }
                }

                // only create second handle if we will have more than 3 nodes
                if (nodes.size() >= 2) closestEnd->hasHandle2 = true;

                // first item in index, so it goes to the front now
                if (index != 0) nodes.push_back(point);
                else nodes.insert(nodes.begin(), point);
            }
        }
        // mouse released, reset everything
        else if (mouseState == GLFW_RELEASE) {
            dragging = false;
            dragIndex = -1;
            dragType = -1;
            dragOrigin.x = 0;
            dragOrigin.y = 0;
            h1Origin.x = 0;
            h1Origin.y = 0;
            h2Origin.x = 0;
            h2Origin.y = 0;
        }

        // draw it all!
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i != nodes.size(); i++) {
            glLoadIdentity();

            // reference so we don't have to do nodes[i] every time
            Point* position = &nodes[i].position;
            Point* handle1 = &nodes[i].handle1;
            Point* handle2 = &nodes[i].handle2;

            // dotted lines from control point to node
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(4.0f, GL_LINE_STIPPLE);
            glColor3f(0, 162.f / 255.f, 232.f / 255.f);
            glLineWidth(2.f);

            if (nodes[i].hasHandle1) {
                glBegin(GL_LINES);
                glVertex2f(handle1->x, handle1->y);
                glVertex2f(position->x, position->y);
                glEnd();
            }

            if (nodes[i].hasHandle2) {
                glBegin(GL_LINES);
                glVertex2f(handle2->x, handle2->y);
                glVertex2f(position->x, position->y);
                glEnd();
            }

            glDisable(GL_LINE_STIPPLE);

            // node itself
            glPointSize(10.f);
            glColor3f(0, 0, 1.0f);
            glBegin(GL_POINTS);
            glVertex2f(position->x, position->y);
            glEnd();

            // circular control points
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // handle1
            if (nodes[i].hasHandle1) {
                glColor3f(0, 0, 0);
                glBegin(GL_POINTS);
                glVertex2f(handle1->x, handle1->y);
                glEnd();
            }

            // handle2
            if (nodes[i].hasHandle2) {
                glColor3f(0, 0, 0);
                glBegin(GL_POINTS);
                glVertex2f(handle2->x, handle2->y);
                glEnd();
            }

            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_BLEND);
        }

        // draw bezier curve using 200 points
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBegin(GL_LINE_STRIP);
        for (int i = 1; i < nodes.size(); i++) {
            // if handle2 exists for previous node, use it, otherwise stick with handle1
            float h2x = nodes[i - 1].handle1.x;
            float h2y = nodes[i - 1].handle1.y;

            if (nodes[i - 1].hasHandle2) {
                h2x = nodes[i - 1].handle2.x;
                h2y = nodes[i - 1].handle2.y;
            }

            for (float a = 0; a != 200; a++) {
                glVertex2f(bezier3(nodes[i - 1].position.x, h2x, nodes[i].handle1.x, nodes[i].position.x, a / 200), bezier3(nodes[i - 1].position.y, h2y, nodes[i].handle1.y, nodes[i].position.y, a / 200));
            }
        }
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glEnd();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}