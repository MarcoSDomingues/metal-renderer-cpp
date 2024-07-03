//
//  Renderer.hpp
//  MetalEngine
//
//  Created by Marco Domingues on 01/07/2024.
//

#pragma once

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>

struct MVertex {
    simd::float4 position;
    simd::float4 color;
};

class Renderer {
public:
    Renderer(MTL::Device* pDevice);
    ~Renderer();
    void draw(MTK::View* pView);

private:
    MTL::Device* _pDevice;
    MTL::CommandQueue* _pCommandQueue;
    MTL::Buffer* _pVertexPositionsBuffer;
    MTL::Buffer* _pVertexColorsBuffer;
    MTL::Buffer* _pVertexIndexBuffer;
    MTL::RenderPipelineState* _pRenderPipelineState;

private:
    void buildBuffers();
    void buildShaders();
};
