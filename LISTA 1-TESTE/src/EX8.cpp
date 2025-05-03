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

//Geração da espiral
std::vector<float> generateSpiral(int numTurns, int segmentsPerTurn, float maxRadius, float centerX, float centerY) {
    std::vector<float> vertices;
    float totalSegments = numTurns * segmentsPerTurn;

    for (int i = 0; i <= totalSegments; i++) {
        float t = (float)i / totalSegments;
        float angle = 2.0f * PI * numTurns * t;
        float radius = maxRadius * t;
        float x = radius * cos(angle) + centerX;
        float y = radius * sin(angle) + centerY;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    return vertices;
}

int main() {
    //Inicializa GLFW e configura contexto OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercicio 8 - Espiral", NULL, NULL);
    if (!window) {
        std::cout << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Carrega funções do OpenGL com GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    //Gera vértices da espiral
    std::vector<float> spiralVertices = generateSpiral(5, 100, 0.6f, 0.0f, 0.0f);

    //Configura VAO e VBO
    unsigned int spiralVAO, spiralVBO;
    glGenVertexArrays(1, &spiralVAO);
    glGenBuffers(1, &spiralVBO);

    glBindVertexArray(spiralVAO);
    glBindBuffer(GL_ARRAY_BUFFER, spiralVBO);
    glBufferData(GL_ARRAY_BUFFER, spiralVertices.size() * sizeof(float), spiralVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Shaders simples
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
            FragColor = vec4(0.9, 0.6, 0.1, 1.0); // Laranja
        }
    )";

    //Compila shaders
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

    //Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(spiralVAO);
        glDrawArrays(GL_LINE_STRIP, 0, spiralVertices.size() / 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Libera recursos
    glDeleteVertexArrays(1, &spiralVAO);
    glDeleteBuffers(1, &spiralVBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}