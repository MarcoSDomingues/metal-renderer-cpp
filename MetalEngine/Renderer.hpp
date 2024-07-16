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
    Renderer();
    ~Renderer();
    void draw();

    inline MTK::View* getMetalView() const { return _pMetalView; }

private:
    MTL::Device* _pDevice;
    MTK::View* _pMetalView;
    MTL::CommandQueue* _pCommandQueue;
    MTL::Library* _pShaderLibrary;
    MTL::Buffer* _pVertexPositionsBuffer;
    MTL::Buffer* _pVertexColorsBuffer;
    MTL::Buffer* _pVertexIndexBuffer;
    MTL::Buffer* _pUniformsBuffer;
    MTL::Buffer* _pArgBuffer;
    MTL::Function* _pVertexFunction;
    MTL::RenderPipelineState* _pRenderPipelineState;

private:
    void makeMetalView();
    void buildBuffers();
    void buildShaders();
    void updateUniforms();
};
