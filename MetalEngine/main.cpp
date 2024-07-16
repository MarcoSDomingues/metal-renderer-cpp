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

#include "Engine.hpp"

int main(int argc, const char * argv[]) {
    std::unique_ptr<Engine> engine = std::make_unique<Engine>();
    engine->run();

    return 0;
}
