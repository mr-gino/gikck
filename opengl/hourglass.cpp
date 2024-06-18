#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Źródło kodu shadera wierzchołków
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Źródło kodu shadera fragmentów
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // Inicjalizacja i konfiguracja GLFW
    // ---------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Tworzenie okna GLFW
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

    // Inicjalizacja GLAD w celu ładowania funkcji OpenGL
    // -------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie udało się zainicjować GLAD" << std::endl;
        return -1;
    }

    // Kompilacja i linkowanie programu shaderów
    // ----------------------------------------
    // Kompilacja shadera wierzchołków
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Sprawdzanie błędów kompilacji shadera
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::VERTEX::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // Kompilacja shadera fragmentów
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Sprawdzanie błędów kompilacji shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::FRAGMENT::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // Linkowanie shaderów
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Sprawdzanie błędów podczas linkowania
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::PROGRAM::LINKOWANIE_NIEUDANE\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Konfiguracja danych wierzchołków i buforów wierzchołków
    // -----------------------------------------------------
    float vertices1[] = {
        -0.4f, -0.6f, 0.0f, // lewy
         0.4f, -0.6f, 0.0f, // prawy
         0.0f,  0.0f, 0.0f  // góra
    };

    float vertices2[] = {
        -0.4f, 0.6f, 0.0f, // lewy
         0.4f, 0.6f, 0.0f, // prawy
         0.0f, 0.0f, 0.0f  // góra
    };

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    // Powiązanie Vertex Array Object (VAO), następnie powiązanie i konfiguracja bufora wierzchołków (VBO), a potem konfiguracja atrybutów wierzchołków
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Odwiązanie VBO i VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Utworzenie drugiego VBO i VAO
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

    // Odkomentuj tę linijkę, aby rysować trójkąty jako siatkę.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Pętla renderowania
    // -----------------
    while (!glfwWindowShouldClose(window))
    {
        // Obsługa wejścia
        // ---------------
        processInput(window);

        // Renderowanie
        // -----------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Rysowanie drugiego trójkąta
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Zamiana buforów i obsługa zdarzeń wejściowych (wciśnięte/przetworzone klawisze, ruch myszy itp.)
        // ---------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Opcjonalne zwolnienie wszystkich zasobów po zakończeniu
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    // Zakończenie glfw, usuwając wszystkie zasoby GLFW.
    // -------------------------------------------------
    glfwTerminate();
    return 0;
}

// Funkcja do obsługi wejścia: sprawdza, czy odpowiednie klawisze zostały wciśnięte/wyciśnięte w tej klatce i reaguje odpowiednio
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Funkcja wywoływana przy zmianie rozmiaru okna (przez system operacyjny lub użytkownika)
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Upewnij się, że widok odpowiada nowym wymiarom okna; zauważ, że szerokość i
    // wysokość będą znacznie większe niż podane na ekranach 
*/
