#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string readFileIntoString(const string& path) {
    ostringstream sstream;
    ifstream fs(path);
    sstream << fs.rdbuf();
    const string str(sstream.str());
    return str;
}

GLuint generateVao(const float points[]) {
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    return vao;
};

GLuint generateShaderProgram(const string vertexShaderPath, const string fragmentShaderPath) {
    // Vertex shader
    string vertShaderString = readFileIntoString(vertexShaderPath);
    const char* vertex_shader = vertShaderString.c_str();

    // Fragment shader 
    string fragmentShaderString = readFileIntoString(fragmentShaderPath);
    const char* fragment_shader = fragmentShaderString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // Program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    return shader_program;
}

int main() {
    // Start GL context and OS window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "failed to init GLEW");
        return 1;
    }

    // Get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // Get version string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth testing interprets a smaller value as "closer"
    glEnable(GL_PROGRAM_POINT_SIZE);

    /* End setup, start custom drawing code */
    // Format: xyzxyzxyz
    float points1[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    float points2[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLuint vao1 = generateVao(points1);
    GLuint vao2 = generateVao(points2);

    // Shaders
    GLuint shader_program_yellow = generateShaderProgram("./shaders/test_vs.glsl", "./shaders/test_fs_yellow.glsl");
    GLuint shader_program_pink = generateShaderProgram("./shaders/test_vs.glsl", "./shaders/test_fs_pink.glsl");

    glClearColor(0.0f, 0.9f, 0.9f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        // Wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// First VAO drawing
        glUseProgram(shader_program_yellow);
        glBindVertexArray(vao1);

        // Draw points 0-3 from the currently bound VAO with the current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //// Second VAO drawing
        glUseProgram(shader_program_pink);
        glBindVertexArray(vao2);

        // Draw points 0-3 from the currently bound VAO with the current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        // Update other events like input handling
        glfwPollEvents();

        // Send drawings to the display
        glfwSwapBuffers(window);
    }

    // Close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}