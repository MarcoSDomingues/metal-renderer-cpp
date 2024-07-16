//
//  Engine.hpp
//  MetalEngine
//
//  Created by Marco Domingues on 16/07/2024.
//

#pragma once

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

class Renderer;
struct GLFWwindow;

class Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
    void initializeWindow();

private:
    Renderer* renderer;
    GLFWwindow* glfwWindow;
    NS::Window* metalWindow;
    MTL::Device* device;
};
