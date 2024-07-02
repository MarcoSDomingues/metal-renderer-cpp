//
//  ViewDelegate.h
//  MetalEngine
//
//  Created by Marco Domingues on 01/07/2024.
//

#pragma once

#include "Renderer.hpp"
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

class ViewDelegate: public MTK::ViewDelegate  {
public:
    ViewDelegate(MTL::Device* pDevice);
    virtual ~ViewDelegate() override;
    virtual void drawInMTKView(MTK::View* pView) override;

private:
    Renderer* _pRenderer;
};
