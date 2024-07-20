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
#include <iostream>

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

    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    renderer->setDrawableSize(width, height);

    metalWindow = reinterpret_cast<NS::Window*>(glfwGetCocoaWindow(glfwWindow));

    NS::View* nsView = metalWindow->getContentView();
    nsView->setLayer(renderer->metalLayer());
    nsView->setWantsLayer(true);
}

void Engine::run()
{
    while (!glfwWindowShouldClose(glfwWindow))
    {
        processInput();
        
        renderer->draw();

        glfwPollEvents();
    }
}

void Engine::processInput()
{
    const float cameraSpeed = 0.05f;

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        renderer->cameraPosition += cameraSpeed * renderer->cameraFront;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        renderer->cameraPosition -= cameraSpeed * renderer->cameraFront;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        simd::float3 cross = simd::cross(renderer->cameraFront, renderer->cameraUp);
        renderer->cameraPosition -= simd::normalize(cross) * cameraSpeed;
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        simd::float3 cross = simd::cross(renderer->cameraFront, renderer->cameraUp);
        renderer->cameraPosition += simd::normalize(cross) * cameraSpeed;
    }
}
