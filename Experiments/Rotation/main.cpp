#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp> //value_ptr

std::string readFileIntoString(const std::string& path) {
    std::ostringstream sstream;
    std::ifstream fs(path);
    sstream << fs.rdbuf();
    const std::string str(sstream.str());
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

GLuint generateShaderProgram(const std::string vertexShaderPath, const std::string fragmentShaderPath) {
    // Vertex shader
    std::string vertShaderString = readFileIntoString(vertexShaderPath);
    const char* vertex_shader = vertShaderString.c_str();

    // Fragment shader 
    std::string fragmentShaderString = readFileIntoString(fragmentShaderPath);
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

    float window_width = 640;
    float window_height = 480;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Hello Triangle", NULL, NULL);
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
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint vao1 = generateVao(points1);

    // Shaders
    GLuint shader_program = generateShaderProgram("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto start = std::chrono::system_clock::now();
    glm::mat4 transform_matrix = glm::mat4(1.0f); // construct identity matrix
    float radians = glm::sin(0.005) * glm::pi<float>();

    while (!glfwWindowShouldClose(window)) {
        // Wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// First VAO drawing
        glUseProgram(shader_program);
        glBindVertexArray(vao1);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        glUniform1f(glGetUniformLocation(shader_program, "u_time"), elapsed_seconds.count());
        glUniform2f(glGetUniformLocation(shader_program, "u_resolution"), window_width, window_height);

        transform_matrix = glm::rotate(transform_matrix, radians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "u_transform"), 1, GL_FALSE, glm::value_ptr(transform_matrix));

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