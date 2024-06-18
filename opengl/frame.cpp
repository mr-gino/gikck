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
        std::cout << "Nie uda�o si� utworzy� okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: wczytanie wszystkich funkcji OpenGL
    // ----------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie uda�o si� zainicjalizowa� GLAD" << std::endl;
        return -1;
    }

    // p�tla renderowania
    // ------------------
    while (!glfwWindowShouldClose(window))
    {
        // obs�uga wej�cia
        // ----------------
        processInput(window);

        // renderowanie
        // ------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: zamiana bufor�w i obs�uga zdarze� wej�cia (wci�ni�cie/przetworzenie klawiszy, ruch myszy itp.)
        // -----------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: zako�czenie i zwolnienie zasob�w GLFW
    // ------------------------------------------
    glfwTerminate();
    return 0;
}

// obs�uga zdarze� zmiany rozmiaru okna: ta funkcja jest wywo�ywana, gdy zmienia si� rozmiar okna przez system operacyjny lub u�ytkownika
// ------------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // upewnij si�, �e widok (viewport) odpowiada nowym wymiarom okna; zauwa�, �e szeroko�� i wysoko�� b�d� znacznie wi�ksze ni� podane na wy�wietlaczach Retina.
    glViewport(0, 0, width, height);
}

// obs�uga wej�cia: sprawdzenie, czy odpowiednie klawisze s� wci�ni�te/zwolnione w danym klatce i reakcja na nie
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


/*
To jest podstawowy kod OpenGL, kt�ry tworzy okno, inicjalizuje kontekst OpenGL i obs�uguje wej�cie. 
W p�tli renderowania czyszczone s� bufory koloru i zamieniane s� bufor�w w oknie. 
Funkcja framebuffer_size_callback jest wywo�ywana, gdy zmienia si� rozmiar okna, a funkcja processInput 
sprawdza, czy zosta� wci�ni�ty klawisz Esc, aby zamkn�� okno.
*/
