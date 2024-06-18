#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   TexCoord = aTexCoord;\n"
"}\0";

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
    // glfw: inicjalizacja i konfiguracja
    // ---------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: utworzenie okna
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "House", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Nie udało się utworzyć okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: załadowanie wskaźników do funkcji OpenGL
    // --------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie udało się zainicjalizować GLAD" << std::endl;
        return -1;
    }


    // kompilacja i łączenie programu shaderów
    // ---------------------------------------
    // vertex shader
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
        std::cout << "BŁĄD::SHADER::VERTEX::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // fragment shader
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
    // łączenie shaderów
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // sprawdzenie błędów łączenia shaderów
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "BŁĄD::SHADER::PROGRAM::LINKING_NIEUDANY\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // konfiguracja danych wierzchołków i atrybutów wierzchołków
    // -------------------------------------------------------
    float vertices[] = {
        // pozycje          // współrzędne tekstury
        -0.5f, -0.75f, 0.0f,  0.0f, 0.0f, // lewy dolny
        -0.5f, 0.25f, 0.0f,  1.0f, 0.0f, // prawy dolny
         0.5f,  -0.75f, 0.5f,  0.0f, 1.0f,  // górny

        -0.5f, 0.25f, 0.0f,  1.0f, 0.0f,
         0.5f,  -0.75f, 0.5f,  0.0f, 1.0f,
         0.5f, 0.25f, 0.0f, 1.0f, 1.0f,


         -0.55f, 0.25f, 0.0f, 0.0f, 0.0f,
         0.55f, 0.25f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.85f, 0.0f, 0.5f, 1.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // powiązanie VAO, VBO i skonfigurowanie atrybutów wierzchołków
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
    // ustawianie parametrów powtarzania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // ustawianie parametrów filtrowania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // wczytywanie obrazu, tworzenie tekstury i generowanie mipmap
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // odwrócenie wczytanego obrazu wzdłuż osi y.
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Błąd wczytywania tekstury" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // ustawianie parametrów powtarzania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // ustawianie parametrów filtrowania tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // wczytywanie obrazu, tworzenie tekstury i generowanie mipmap
    int width1, height1, nrChannels1;
    data = stbi_load("roof.jpg", &width1, &height1, &nrChannels1, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Błąd wczytywania tekstury" << std::endl;
    }
    stbi_image_free(data);


    // odkomentuj tę linię, aby rysować trójkąty w trybie siatki.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // pętla renderowania
    // ------------------
    while (!glfwWindowShouldClose(window))
    {
        // obsługa wejścia
        // ----------------
        processInput(window);

        // renderowanie
        // ------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // powiązanie tekstury
        glBindTexture(GL_TEXTURE_2D, texture);

        // rysowanie pierwszego trójkąta
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawArrays(GL_TRIANGLES, 6, 3);

        // obsługa zdarzeń i wymiana buforów
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // zwolnienie zasobów
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: zakończenie, zwolnienie zasobów
    glfwTerminate();
    return 0;
}

// funkcja obsługująca zmianę rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// funkcja obsługująca wejście z klawiatury
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
