//
//  Engine.cpp
//  MetalEngine
//
//  Created by Marco Domingues on 16/07/2024.
//

#define GLFW_EXPOSE_NATIVE_COCOA
#include "Engine.hpp"
#include "Renderer.hpp"
#include "glfw3.h"
#include "glfw3native.h"

Engine::Engine()
{
    renderer = new Renderer();

    initializeWindow();
}

Engine::~Engine()
{
    metalWindow->release();
    glfwTerminate();
    delete renderer;
}

void Engine::initializeWindow()
{
    if (!glfwInit())
    {
        __builtin_printf("Error initializing glfw");
        assert(false);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindow = glfwCreateWindow(800, 600, "Metal Engine", NULL, NULL);

    if (!glfwWindow)
    {
        glfwTerminate();
        __builtin_printf("Error creating window");
        assert(false);
    }

    metalWindow = reinterpret_cast<NS::Window*>(glfwGetCocoaWindow(glfwWindow));
    metalWindow->setContentView(renderer->metalView());
}

void Engine::run()
{
    while (!glfwWindowShouldClose(glfwWindow))
    {
        renderer->draw();

        glfwPollEvents();
    }
}
