#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    // tworzenie okna GLFW
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First OpenGL Frame", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Nie uda³o siê utworzyæ okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: wczytanie wszystkich funkcji OpenGL
    // ----------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie uda³o siê zainicjalizowaæ GLAD" << std::endl;
        return -1;
    }

    // pêtla renderowania
    // ------------------
    while (!glfwWindowShouldClose(window))
    {
        // obs³uga wejœcia
        // ----------------
        processInput(window);

        // renderowanie
        // ------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: zamiana buforów i obs³uga zdarzeñ wejœcia (wciœniêcie/przetworzenie klawiszy, ruch myszy itp.)
        // -----------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: zakoñczenie i zwolnienie zasobów GLFW
    // ------------------------------------------
    glfwTerminate();
    return 0;
}

// obs³uga zdarzeñ zmiany rozmiaru okna: ta funkcja jest wywo³ywana, gdy zmienia siê rozmiar okna przez system operacyjny lub u¿ytkownika
// ------------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // upewnij siê, ¿e widok (viewport) odpowiada nowym wymiarom okna; zauwa¿, ¿e szerokoœæ i wysokoœæ bêd¹ znacznie wiêksze ni¿ podane na wyœwietlaczach Retina.
    glViewport(0, 0, width, height);
}

// obs³uga wejœcia: sprawdzenie, czy odpowiednie klawisze s¹ wciœniête/zwolnione w danym klatce i reakcja na nie
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


/*
To jest podstawowy kod OpenGL, który tworzy okno, inicjalizuje kontekst OpenGL i obs³uguje wejœcie. 
W pêtli renderowania czyszczone s¹ bufory koloru i zamieniane s¹ buforów w oknie. 
Funkcja framebuffer_size_callback jest wywo³ywana, gdy zmienia siê rozmiar okna, a funkcja processInput 
sprawdza, czy zosta³ wciœniêty klawisz Esc, aby zamkn¹æ okno.
*/
