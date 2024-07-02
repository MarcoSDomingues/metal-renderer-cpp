//
//  main.cpp
//  renderer
//
//  Created by Marco Domingues on 01/07/2024.
//

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <iostream>
#include "AppDelegate.hpp"

int main(int argc, const char * argv[]) {
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
    
    AppDelegate appDelegate;
    NS::Application* pSharedApplication = NS::Application::sharedApplication();
    pSharedApplication->setDelegate(&appDelegate);
    pSharedApplication->run();

    pAutoreleasePool->release();

    return 0;
}
