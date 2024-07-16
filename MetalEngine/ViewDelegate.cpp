//
//  ViewDelegate.cpp
//  MetalEngine
//
//  Created by Marco Domingues on 01/07/2024.
//

#include "ViewDelegate.hpp"

ViewDelegate::ViewDelegate(MTL::Device* pDevice)
    : MTK::ViewDelegate(), _pRenderer(new Renderer(pDevice))
{
}

ViewDelegate::~ViewDelegate()
{
    delete _pRenderer;
}

void ViewDelegate::drawInMTKView(MTK::View *pView)
{
    _pRenderer->draw();
}
