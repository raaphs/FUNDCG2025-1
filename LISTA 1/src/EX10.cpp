#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//Compilação de shaders
unsigned int createShaderProgram(const char* vShaderSrc, const char* fShaderSrc) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSrc, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSrc, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

int main() {
    //Inicializa GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercicio 10 - Casa Desenhada", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    //=== Dados da base da casa (quadrado) ===
    float baseVertices[] = {
        -0.5f, -0.5f, 0.0f,  //Inferior esquerdo
         0.5f, -0.5f, 0.0f,  //Inferior direito
         0.5f,  0.0f, 0.0f,  //Superior direito

        -0.5f, -0.5f, 0.0f,
         0.5f,  0.0f, 0.0f,
        -0.5f,  0.0f, 0.0f   //Superior esquerdo
    };

    //=== Dados do telhado (triângulo) ===
    float roofVertices[] = {
        -0.6f,  0.0f, 0.0f,
         0.6f,  0.0f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    //=== Dados da porta (retângulo) ===
    float doorVertices[] = {
        -0.1f, -0.5f, 0.0f,
         0.1f, -0.5f, 0.0f,
         0.1f, -0.2f, 0.0f,

        -0.1f, -0.5f, 0.0f,
         0.1f, -0.2f, 0.0f,
        -0.1f, -0.2f, 0.0f
    };

    //=== Dados da janela (pontos) ===
    float windowPoints[] = {
        -0.35f, -0.1f, 0.0f,
        -0.35f,  0.1f, 0.0f,
        -0.15f, -0.1f, 0.0f,
        -0.15f,  0.1f, 0.0f
    };

    //Shader simples
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
            FragColor = vec4(0.8, 0.6, 0.2, 1.0);
        }
    )";

    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    //==== VAOs e VBOs ====
    unsigned int VAOs[4], VBOs[4];
    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);

    //Base da casa
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Telhado
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roofVertices), roofVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Porta
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Janela (pontos)
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowPoints), windowPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Loop principal
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //Desenhar base
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Telhado
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Porta
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Janela como pontos
        glPointSize(10.0f);
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Limpeza
    glDeleteVertexArrays(4, VAOs);
    glDeleteBuffers(4, VBOs);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}