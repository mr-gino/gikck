#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Kod źródłowy vertex shadera
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\0";

// Kod źródłowy fragment shadera
const char* fragmentShaderSource = "#version 330 core\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

int main()
{
    // Inicjalizacja GLFW
    // ------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Tworzenie okna GLFW
    // ------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Texture", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Nie udało się utworzyć okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicjalizacja GLAD
    // -----------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie udało się zainicjalizować GLAD" << std::endl;
        return -1;
    }


    // Kompilacja shaderów
    // -------------------
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Sprawdzenie błędów kompilacji shadera
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::VERTEX::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Sprawdzenie błędów kompilacji shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::FRAGMENT::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // Połączenie shaderów w program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Sprawdzenie błędów podczas łączenia programu
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Konfiguracja danych wierzchołków
    // ------------------------------
    float vertices[] = {
        // Pozycje            // Koordynaty tekstury
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Lewy dolny
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Prawy dolny
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f  // Górny
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Połączenie VAO, VBO i skonfigurowanie atrybutów wierzchołków
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Ustawienie parametrów owijania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Ustawienie parametrów filtrowania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Wczytanie obrazu, utworzenie tekstury i generowanie mipmap
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Obrócenie tekstury w osi Y (OpenGL ma odwrócone koordynaty Y)
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Błąd podczas wczytywania tekstury" << std::endl;
    }
    stbi_image_free(data);


    // Odkomentuj tę linię, aby rysować trójkąty jako siatkę.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Pętla renderująca
    // ----------------
    while (!glfwWindowShouldClose(window))
    {
        // Wejście
        // -------
        processInput(window);

        // Renderowanie
        // -----------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Powiązanie tekstury
        glBindTexture(GL_TEXTURE_2D, texture);

        // Narysowanie trójkąta
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // Choć mamy tylko jeden VAO, to nie ma konieczności powiązywania go za każdym razem, ale robimy to dla porządku
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // Nie ma potrzeby odbierania powiązania po każdym użyciu

        // glfw: wymiana buforów i obsługa zdarzeń wejścia (naciśnięcie/przyciśnięcie klawiszy, ruch myszy itp.)
        // ---------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Opcjonalnie: zwolnienie zasobów po zakończeniu działania programu:
    // -----------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: zakończenie, wyczyszczenie wszystkich zasobów GLFW
    // ------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Przetworzenie wszystkich zdarzeń wejścia: sprawdzenie, czy istotne klawisze zostały wciśnięte/zwalniane w tej klatce i podjęcie odpowiednich działań
// --------------------------------------------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Funkcja wywoływana przy zmianie rozmiaru okna (przez system operacyjny lub użytkownika), wykonuje odpowiednie operacje
// ---------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Sprawdzenie, czy obszar renderowania odpowiada nowym wymiarom okna; należy pamiętać, że width i height mogą być znacznie większe niż te podane na wyświetlaczu Retina.
    glViewport(0, 0, width, height);
}
