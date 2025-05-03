#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <iostream>

#define PI 3.14159265359f

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

std::vector<float> generatePolygonVertices(int sides, float radius, float centerX, float centerY) {
    std::vector<float> vertices;
    vertices.push_back(centerX); // Centro X
    vertices.push_back(centerY); // Centro Y
    vertices.push_back(0.0f);    // Z

    for (int i = 0; i <= sides; i++) {
        float angle = 2.0f * PI * i / sides;
        float x = radius * cos(angle) + centerX;
        float y = radius * sin(angle) + centerY;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
    return vertices;
}

std::vector<float> generateArc(float angleStart, float angleEnd, float radius, float centerX, float centerY) {
    std::vector<float> vertices = { centerX, centerY, 0.0f };
    int segments = 50;
    for (int i = 0; i <= segments; i++) {
        float angle = angleStart + (angleEnd - angleStart) * i / segments;
        float x = radius * cos(angle) + centerX;
        float y = radius * sin(angle) + centerY;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
    return vertices;
}

std::vector<float> generateStar(int points, float innerR, float outerR, float cx, float cy) {
    std::vector<float> vertices = { cx, cy, 0.0f };
    for (int i = 0; i <= points * 2; i++) {
        float r = (i % 2 == 0) ? outerR : innerR;
        float angle = i * PI / points;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
    return vertices;
}

int main() {
    // Inicialização GLFW + contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercicio 7 - Formas Geometricas", NULL, NULL);
    if (!window) {
        std::cout << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao carregar GLAD" << std::endl;
        return -1;
    }

    // Gerando vértices
    std::vector<float> circleVertices   = generatePolygonVertices(100, 0.4f,  0.0f,  0.0f);
    std::vector<float> octagonVertices  = generatePolygonVertices(8,   0.3f, -0.6f,  0.5f);
    std::vector<float> pentagonVertices = generatePolygonVertices(5,   0.3f,  0.6f, -0.5f);
    std::vector<float> pacmanVertices   = generateArc(PI / 4, 7 * PI / 4, 0.4f, -0.6f, -0.5f);
    std::vector<float> pizzaVertices    = generateArc(0.0f, PI / 3, 0.4f, 0.0f, 0.6f);
    std::vector<float> starVertices     = generateStar(5, 0.2f, 0.4f, 0.6f, 0.6f);

    std::vector<std::vector<float>> allShapes = {
        circleVertices, octagonVertices, pentagonVertices,
        pacmanVertices, pizzaVertices, starVertices
    };

    unsigned int VAOs[6], VBOs[6];
    glGenVertexArrays(6, VAOs);
    glGenBuffers(6, VBOs);

    for (int i = 0; i < 6; i++) {
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, allShapes[i].size() * sizeof(float), allShapes[i].data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Shaders
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.7, 0.2, 1.0); // Laranja claro
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        for (int i = 0; i < 6; i++) {
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, allShapes[i].size() / 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(6, VAOs);
    glDeleteBuffers(6, VBOs);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}