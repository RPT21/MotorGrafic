#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include "camera.h" // Incluim la classe camera - Com que l'he fet jo, s'ha de posar entre cometes, ja que es una llibreria local.
#include "scene.h" // Incluim la classe scene - Com que l'he fet jo, s'ha de posar entre cometes, ja que es una llibreria local.
#include "GLFW_Test.h"

int main()
{
    Matrix<double, 3, 1> camera_pos = 15 * Matrix<double, 3, 1> {1, 0, 0};
    double d = 8;
    int pixels_x = 1024;
    int pixels_y = 1024;
    double len_x = 10;
    double len_y = 10;

    Camera camera(camera_pos, d, pixels_x, pixels_y, len_x, len_y);
    camera.followPoint({ 0, 0, 0 });
    double a = 2;

    Scene scene;
    scene.addTriangle({ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 255, 0, 0 }, 0.5, 0.5);

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
    int width = 1024;
    int height = 1024;
    auto image = generate_rgb_image(width, height);

    // Configurar la textura con la imagen
    GLuint texture = setup_texture(image, width, height);

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
		texture = setup_texture(image, width, height);
        display_image();

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

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
