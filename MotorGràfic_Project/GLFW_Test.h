#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <GL/gl.h>
# include <Eigen/Dense>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

// Color inicial de la pantalla (rojo, verde, azul)
float backgroundColor[] = { 0.0f, 0.0f, 0.0f };
bool isEnabled = false;

// Callback de teclado
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Cambiar el color de fondo según la tecla presionada
        if (key == GLFW_KEY_R) { // Tecla 'R' para rojo
            backgroundColor[0] = 1.0f; backgroundColor[1] = 0.0f; backgroundColor[2] = 0.0f;
        }
        else if (key == GLFW_KEY_G) { // Tecla 'G' para verde
            backgroundColor[0] = 0.0f; backgroundColor[1] = 1.0f; backgroundColor[2] = 0.0f;
        }
        else if (key == GLFW_KEY_B) { // Tecla 'B' para azul
            backgroundColor[0] = 0.0f; backgroundColor[1] = 0.0f; backgroundColor[2] = 1.0f;
        }
        else if (key == GLFW_KEY_W) { // Tecla 'W' para blanco
            backgroundColor[0] = 1.0f; backgroundColor[1] = 1.0f; backgroundColor[2] = 1.0f;
        }
        else if (key == GLFW_KEY_K) { // Tecla 'K' para negro
            backgroundColor[0] = 0.0f; backgroundColor[1] = 0.0f; backgroundColor[2] = 0.0f;
        }
        else if (key == GLFW_KEY_SPACE) { // Tecla per canviar el mode de funcionament
            if (isEnabled) {
                isEnabled = false;
            }
            else if (!isEnabled) {
                isEnabled = true;
            }

        }
    }
}

// Función para generar una imagen RGB (una matriz de valores de píxeles)
std::vector<unsigned char> generate_rgb_image(int width, int height) {
    std::vector<unsigned char> image(width * height * 3);

    // Llenamos la matriz con un gradiente de color
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image[(y * width + x) * 3 + 0] = static_cast<unsigned char>(x % 256);  // R
            image[(y * width + x) * 3 + 1] = static_cast<unsigned char>(y % 256);  // G
            image[(y * width + x) * 3 + 2] = static_cast<unsigned char>((x + y) % 256);  // B
        }
    }
    return image;
}

// Crear una textura y cargar la imagen en ella
GLuint setup_texture(const std::vector<unsigned char>& image, int width, int height) {
    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configurar la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Cargar la imagen en la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());

    return texture;
}

// Dibuja la textura en la pantalla

void display_image() {
    // Dibuixem la textura en un quadrilater (la pantalla)
    // Per tant si volem pintar una textura al quadrilater, hem d'especificar la regio rectangular de la textura que volem pintar
    // Per tant necessitem especificar 4 punts en les coordenades de la textura.
    // Despres hem d'assignar aquests punts a les coordenades del quadrilater que volem pintar (els extrems de la pantalla)
    // Per aixo fem servir les funcions glTexCoord2f i glVertex2f.   
    // La pantalla utilitza unes coordenades normalitzades que van de -1 a 1 en els eixos X i Y.
    // La textura utilitza unes coordenades normalitzades que van de 0 a 1 en els eixos X i Y.

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0); // punt inferior esquerra de la textura
    glVertex2f(-1, -1); // punt inferior esquerra de la pantalla

    glTexCoord2f(1, 0); // punt inferior dret de la textura
    glVertex2f(1, -1); // punt inferior dret de la pantalla

    glTexCoord2f(1, 1); // punt superior dret de la textura
    glVertex2f(1, 1); // punt superior dret de la pantalla

    glTexCoord2f(0, 1); // punt superior esquerra de la textura
    glVertex2f(-1, 1); // punt superior esquerra de la pantalla

    glEnd();
}

int PrintImageColorTest() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Change Background Color", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // Hacer el contexto de la ventana actual
    glfwMakeContextCurrent(window);

    // Asignar el callback de teclado
    glfwSetKeyCallback(window, keyCallback);

    // Generar la imagen RGB
    int width = 512;
    int height = 512;
    auto image = generate_rgb_image(width, height);

    // Configurar la textura con la imagen
    GLuint texture = setup_texture(image, width, height);

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {

        if (isEnabled) {
            // Cambiar el color de fondo
            glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else if (!isEnabled) {
            // Activar la textura y dibujarla
            glBindTexture(GL_TEXTURE_2D, texture);
            display_image();
        }

        // Actualizar la ventana - Carrega la imatge que esta en el buffer a la pantalla
        glfwSwapBuffers(window);

        // Procesar eventos - Teclat i altres events
        glfwPollEvents();
    }

    // Destruir ventana y terminar GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

int printTriangleGradient()
{
    if (!glfwInit()) {
        return -1; // Error al inicializar GLFW
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Ventana OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1; // Error al crear la ventana
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar un triángulo
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
