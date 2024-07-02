//
//  Renderer.cpp
//  MetalEngine
//
//  Created by Marco Domingues on 01/07/2024.
//

#include "Renderer.hpp"

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
    _pVertexColorsBuffer->release();
    _pVertexPositionsBuffer->release();
    _pRenderPipelineState->release();
}

void Renderer::buildBuffers()
{
    const size_t NumVertices = 3;
    using namespace simd;

    float4 positions[NumVertices] =
    {
        {  0.0f,  0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f }
    };

    float4 colors[NumVertices] =
    {
        {  1.0f, 0.0f, 0.0f, 1.0f },
        {  0.0f, 1.0f, 0.0f, 1.0f },
        {  0.0f, 0.0f, 1.0f, 1.0f }
    };

    const size_t positionsDataSize = NumVertices * sizeof(float4);
    const size_t colorsDataSize = NumVertices * sizeof(float4);

    _pVertexPositionsBuffer = _pDevice->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    _pVertexColorsBuffer = _pDevice->newBuffer(colorsDataSize, MTL::ResourceStorageModeManaged);

    memcpy(_pVertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(_pVertexColorsBuffer->contents(), colors, colorsDataSize);

    _pVertexPositionsBuffer->didModifyRange(NS::Range::Make(0, _pVertexPositionsBuffer->length()));
    _pVertexColorsBuffer->didModifyRange(NS::Range::Make(0, _pVertexColorsBuffer->length()));
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    MTL::Library* pLibrary = _pDevice->newDefaultLibrary();
    if ( !pLibrary )
    {
        __builtin_printf("Error creating shader library.");
        assert( false );
    }

    MTL::Function* pVertexFunc = pLibrary->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function* pFragmentFunc = pLibrary->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    MTL::RenderPipelineDescriptor *pPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    pPipelineDescriptor->setVertexFunction(pVertexFunc);
    pPipelineDescriptor->setFragmentFunction(pFragmentFunc);
    pPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    NS::Error* pError = nullptr;

    _pRenderPipelineState = _pDevice->newRenderPipelineState(pPipelineDescriptor, &pError);
    if (!_pRenderPipelineState)
    {
        __builtin_printf("%s", pError->localizedDescription()->utf8String());
        assert(false);
    }

    pVertexFunc->release();
    pFragmentFunc->release();
    pPipelineDescriptor->release();
    pLibrary->release();
}

void Renderer::draw(MTK::View *pView)
{
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* pCommandBuffer = _pCommandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pPassDescriptor = pView->currentRenderPassDescriptor();
    pPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.8, 0.8, 0.8, 1.0));
    pPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreAction::StoreActionStore);

    MTL::RenderCommandEncoder* pEncoder = pCommandBuffer->renderCommandEncoder(pPassDescriptor);

    pEncoder->setRenderPipelineState(_pRenderPipelineState);
    pEncoder->setVertexBuffer(_pVertexPositionsBuffer, 0, 0);
    pEncoder->setVertexBuffer(_pVertexColorsBuffer, 0, 1);
    pEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    pEncoder->endEncoding();
    pCommandBuffer->presentDrawable(pView->currentDrawable());
    pCommandBuffer->commit();

    pAutoreleasePool->release();
}
