#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// kod źródłowy shadera wierzchołków
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// kod źródłowy shadera fragmentów
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: inicjalizacja i konfiguracja
    // ----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: tworzenie okna
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hourglass", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Nie udało się utworzyć okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: wczytanie wskaźników do funkcji OpenGL
    // -------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie udało się zainicjować GLAD" << std::endl;
        return -1;
    }


    // kompilacja i zlinkowanie programu shaderów
    // -----------------------------------------
    // shader wierzchołków
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // sprawdzenie błędów kompilacji shadera
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::WIERZCHOŁEK::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // shader fragmentów
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // sprawdzenie błędów kompilacji shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::FRAGMENT::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // zlinkowanie shaderów
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // sprawdzenie błędów linkowania
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::PROGRAM::LINKOWANIE_NIEUDANE\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // konfiguracja danych wierzchołków
    // --------------------------------
    float vertices1[] = {
        -0.4f, -0.6f, 0.0f, // lewy dolny punkt
         0.4f, -0.6f, 0.0f, // prawy dolny punkt
         0.0f,  0.0f, 0.0f  // górny punkt
    };

    float vertices2[] = {
        -0.4f, 0.6f, 0.0f, // lewy górny punkt
         0.4f, 0.6f, 0.0f, // prawy górny punkt
         0.0f, 0.0f, 0.0f  // górny punkt
    };

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    // powiązanie Vertex Array Object (VAO) jako pierwszego, następnie powiązanie i skonfigurowanie bufora wierzchołków (VBO) i atrybutów wierzchołków
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // odbindowanie VBO i VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // tworzenie drugiego VBO i VAO
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // odkomentuj tę linijkę, aby rysować w trybie wyświetlania linii.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // pętla renderowania
    // -----------------
    while (!glfwWindowShouldClose(window))
    {
        // obsługa wejścia
        // ---------------
        processInput(window);

        // renderowanie
        // -----------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // rysowanie drugiego trójkąta
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: zamiana buforów i obsługa zdarzeń wejściowych (naciśnięcie/wyciśnięcie klawiszy, ruch myszy itp.)
        // ---------------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // opcjonalne: zwolnienie wszystkich zasobów, gdy nie są już potrzebne:
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    // glfw: zakończenie, czyszczenie wszystkich wcześniej zaalokowanych zasobów GLFW.
    // --------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// obsługa wejścia: sprawdzenie, czy odpowiednie klawisze są naciśnięte/wyciśnięte w tej klatce i reakcja na to
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: funkcja wywoływana za każdym razem, gdy zmieni się rozmiar okna (zmiana rozmiaru przez OS lub użytkownika)
// --------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // upewnij się, że viewport pasuje do nowych wymiarów okna; zauważ, że szerokość i wysokość będą znacznie większe niż podane na wyświetlaczach retina.
    glViewport(0, 0, width, height);
}

