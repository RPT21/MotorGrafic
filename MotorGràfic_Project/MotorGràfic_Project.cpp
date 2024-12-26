#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <string>

// Importem les llibreries locals
#include "camera.h" 
#include "scene.h" 
#include "GLFW_Test.h"
#include "rasterizer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main()
{
    Vector3d camera_pos = 15 * Vector3d {1, 0, 0};
    double d = 8;
    int width = 1024;
    int height = 1024;
    double len_x = 10;
    double len_y = 10;

    Camera camera(camera_pos, d, width, height, len_x, len_y);
    camera.followPoint({ 0, 0, 0 });

    Scene scene;
    scene.addTriangle({ 0, 0, 0 }, { 0, 2, 0 }, { 0, 0, 2 }, { 255, 0, 0 }, 0.5, 0.5);

    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Crear una ventana
    GLFWwindow* window = glfwCreateWindow(width, height, "Motor Gràfic", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // Hacer el contexto de la ventana actual
    glfwMakeContextCurrent(window);

	// Deshabilita V - Sync - Si no es limiten els FPS a 60 Hz
    // glfwSwapInterval(0);

    // Asignar el callback de teclado
    // glfwSetKeyCallback(window, keyCallback);

	// Definim el vector que contindra la imatge
    vector<unsigned char> image(width * height * 3);

    // Configurar la textura con la imagen
    GLuint texture = setup_texture(image, width, height);

    // Variables para el cálculo de FPS
    double previous_time = glfwGetTime();
    double frame_count = 0;
    double fps = 0;
    double current_time;
    double delta_time;
	string title = "Motor Gràfic";
    string windowTitle;

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {

        // Procesar eventos - Teclat i altres events
        glfwPollEvents();
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {  // Flecha hacia delante
            camera.moveCamera({ 0, 0, 0.1 });
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {  // Flecha hacia atras
            camera.moveCamera({ 0, 0, -0.1 });
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {  // Flecha hacia izquierda
            camera.moveCamera({ 0, 0.1, 0 });
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {  // Flecha hacia derecha
            camera.moveCamera({ 0, -0.1, 0 });
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {  // Flecha hacia arriba
            camera.moveCamera({ 0.1, 0, 0 });
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {  // Flecha hacia abajo
            camera.moveCamera({ -0.1, 0, 0 });
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {  // Rotacion antihoraria
            camera.intrinsicRotation(M_PI / 30);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {  // Rotacion horaria
            camera.intrinsicRotation(-M_PI / 30);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {  // Girar a la derecha
            camera.rotateScreenY(-M_PI / 300);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {  // Girar a la izquierda
            camera.rotateScreenY(M_PI / 300);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {  // Girar hacia abajo
            camera.rotateScreenX(-M_PI / 300);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {  // Girar hacia arriba
            camera.rotateScreenX(M_PI / 300);
        }

        // Tiempo actual
        current_time = glfwGetTime();
        delta_time = current_time - previous_time;
        frame_count += 1;

        // Actualizar FPS cada segundo(es pot canviar)
        if (delta_time >= 1) {
            fps = frame_count / delta_time;
            previous_time = current_time;
            frame_count = 0;

            windowTitle = title + "- FPS: " + to_string(fps);
            // El metode c_str retorna un punter al primer caracter de la cadena de caracters
            glfwSetWindowTitle(window, windowTitle.c_str());
        }

		// Netegem el buffer de color
        glClear(GL_COLOR_BUFFER_BIT);

        // Generem la nova image
        rasterizeImage(image, camera, scene);

        // Apliquem la nova image a la textura - Important reutilitzar la mateixa textura. Crear una de nova dona lloc a memory leaks.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());

		// Actualizem la textura
        display_image();

        // Actualizar la ventana - Carrega la textura que esta en el buffer a la pantalla
        glfwSwapBuffers(window);
    }

    // Alliberem la textura de la memoria
    glDeleteTextures(1, &texture);

    // Destruir ventana y terminar GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
