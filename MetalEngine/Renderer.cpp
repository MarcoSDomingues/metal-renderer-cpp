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
}

Renderer::~Renderer()
{
    _pDevice->release();
    _pCommandQueue->release();
}

void Renderer::draw(MTK::View *pView)
{
    NS::AutoreleasePool* pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* pCmdBuffer = _pCommandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pPassDescriptor = pView->currentRenderPassDescriptor();
    pPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 1.0, 0.0, 1.0));
    pPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadAction::LoadActionClear);
    MTL::RenderCommandEncoder* pEncoder = pCmdBuffer->renderCommandEncoder(pPassDescriptor);

    pEncoder->endEncoding();
    pCmdBuffer->presentDrawable(pView->currentDrawable());
    pCmdBuffer->commit();

    pAutoreleasePool->release();
}
