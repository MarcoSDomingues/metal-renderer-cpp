//
//  Renderer.cpp
//  MetalEngine
//
//  Created by Marco Domingues on 01/07/2024.
//

#include "Renderer.hpp"
#include "Matrix.hpp"

Renderer::Renderer(MTL::Device* pDevice)
    : _pDevice(pDevice->retain())
{
    _pCommandQueue = _pDevice->newCommandQueue();
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    _pDevice->release();
    _pCommandQueue->release();
    _pVertexIndexBuffer->release();
    _pVertexPositionsBuffer->release();
    _pRenderPipelineState->release();
    _pVertexColorsBuffer->release();
    _pUniformsBuffer->release();
    _pArgBuffer->release();
    _pShaderLibrary->release();
    _pVertexFunction->release();
}

void Renderer::buildBuffers()
{
    const float s = 0.5f;
    const size_t NumVertices = 8;

    simd::float4 positions[NumVertices] =
    {
        { -s, -s, +s, 1.0f },
        { +s, -s, +s, 1.0f },
        { +s, +s, +s, 1.0f },
        { -s, +s, +s, 1.0f },

        { -s, -s, -s, 1.0f },
        { -s, +s, -s, 1.0f },
        { +s, +s, -s, 1.0f },
        { +s, -s, -s, 1.0f }
    };

    simd::float4 colors[NumVertices] =
    {
        {  1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 1.0f, 1.0f, 1.0f },
        {  0.0f, 1.0f, 0.0f, 1.0f },
        {  0.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, 1.0f, 0.0f, 1.0f }
    };

    uint16_t indices[] = {
        0, 1, 2, /* front */
        2, 3, 0,

        1, 7, 6, /* right */
        6, 2, 1,

        7, 4, 5, /* back */
        5, 6, 7,

        4, 0, 3, /* left */
        3, 5, 4,

        3, 2, 6, /* top */
        6, 5, 3,

        4, 7, 1, /* bottom */
        1, 0, 4
    };

    const size_t positionsDataSize = NumVertices * sizeof(simd::float4);
    const size_t colorsDataSize = NumVertices * sizeof(simd::float4);
    const size_t indexDataSize = sizeof(indices);

    _pVertexPositionsBuffer = _pDevice->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    _pVertexColorsBuffer = _pDevice->newBuffer(colorsDataSize, MTL::ResourceStorageModeManaged);
    _pVertexIndexBuffer = _pDevice->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    memcpy(_pVertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(_pVertexColorsBuffer->contents(), colors, colorsDataSize);
    memcpy(_pVertexIndexBuffer->contents(), indices, indexDataSize);

    _pVertexPositionsBuffer->didModifyRange(NS::Range::Make(0, _pVertexPositionsBuffer->length()));
    _pVertexColorsBuffer->didModifyRange(NS::Range::Make(0, _pVertexColorsBuffer->length()));
    _pVertexIndexBuffer->didModifyRange(NS::Range::Make(0, _pVertexIndexBuffer->length()));

    assert(_pShaderLibrary);
    assert(_pVertexFunction);

    MTL::ArgumentEncoder *pArgEncoder = _pVertexFunction->newArgumentEncoder(0);
    _pArgBuffer = _pDevice->newBuffer(pArgEncoder->encodedLength(), MTL::ResourceStorageModeManaged);

    pArgEncoder->setArgumentBuffer(_pArgBuffer, 0);
    pArgEncoder->setBuffer(_pVertexPositionsBuffer, 0, 0);
    pArgEncoder->setBuffer(_pVertexColorsBuffer, 0, 1);

    _pArgBuffer->didModifyRange(NS::Range::Make(0, _pArgBuffer->length()));

    pArgEncoder->release();
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    _pShaderLibrary = _pDevice->newDefaultLibrary();
    if (!_pShaderLibrary)
    {
        __builtin_printf("Error creating shader library.");
        assert( false );
    }

    _pVertexFunction = _pShaderLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function* pFragmentFunc = _pShaderLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    MTL::RenderPipelineDescriptor *pPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    pPipelineDescriptor->setVertexFunction(_pVertexFunction);
    pPipelineDescriptor->setFragmentFunction(pFragmentFunc);
    pPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    NS::Error* pError = nullptr;

    _pRenderPipelineState = _pDevice->newRenderPipelineState(pPipelineDescriptor, &pError);
    if (!_pRenderPipelineState)
    {
        __builtin_printf("%s", pError->localizedDescription()->utf8String());
        assert(false);
    }

    pFragmentFunc->release();
    pPipelineDescriptor->release();
}

void Renderer::updateUniforms()
{
    // Identity matrix
    simd::float4x4 modelMatrix = simd::float4x4(1.0);

    simd::float3 eye = simd::make_float3(0.0, 2.0, 2.0);
    simd::float3 center = simd::make_float3(0.0, 0.0, 0.0);
    simd::float3 up = simd::make_float3(0.0, 1.0, 0.0);
    simd::float4x4 viewMatrix = Matrix::createLookAt(eye, center, up);

    float aspectRatio = 800.0 / 600.0;
    float fov = 45.0 * (M_PI / 180.0);
    float near = 0.1;
    float far = 100.0;
    simd::float4x4 perspectiveProjectionMatrix = Matrix::createPerspective(fov, aspectRatio, near, far);

    simd::float4x4 mvpMatrix = perspectiveProjectionMatrix * viewMatrix * modelMatrix;

    const size_t uniformsDataSize = sizeof(simd::float4x4);
    _pUniformsBuffer = _pDevice->newBuffer(uniformsDataSize, MTL::ResourceStorageModeManaged);

    memcpy(_pUniformsBuffer->contents(), &mvpMatrix, uniformsDataSize);

    _pUniformsBuffer->didModifyRange(NS::Range::Make(0, _pUniformsBuffer->length()));
}

void Renderer::draw(MTK::View *pView)
{
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    updateUniforms();

    MTL::CommandBuffer* pCommandBuffer = _pCommandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pPassDescriptor = pView->currentRenderPassDescriptor();
    pPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.8, 0.8, 0.8, 1.0));
    pPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreAction::StoreActionStore);

    MTL::RenderCommandEncoder* pEncoder = pCommandBuffer->renderCommandEncoder(pPassDescriptor);

    pEncoder->setRenderPipelineState(_pRenderPipelineState);
    pEncoder->setVertexBuffer(_pArgBuffer, 0, 0);
    pEncoder->useResource(_pVertexPositionsBuffer, MTL::ResourceUsageRead);
    pEncoder->useResource(_pVertexColorsBuffer, MTL::ResourceUsageRead);
    pEncoder->setVertexBuffer(_pUniformsBuffer, 0, 1);

    pEncoder->setCullMode(MTL::CullModeBack);
    pEncoder->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    pEncoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                    6 * 6,
                                    MTL::IndexType::IndexTypeUInt16,
                                    _pVertexIndexBuffer,
                                    0);
    pEncoder->endEncoding();
    pCommandBuffer->presentDrawable(pView->currentDrawable());
    pCommandBuffer->commit();

    pAutoreleasePool->release();
}
