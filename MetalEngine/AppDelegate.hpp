//
//  AppDelegate.h
//  renderer
//
//  Created by Marco Domingues on 01/07/2024.
//

#pragma once

#include "ViewDelegate.hpp"
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

class AppDelegate: public NS::ApplicationDelegate {
public:
    ~AppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching(NS::Notification* notification) override;
    virtual void applicationDidFinishLaunching(NS::Notification* notification) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;

private:
    NS::Window* _pWindow;
    MTK::View* _pMtkView;
    MTL::Device* _pDevice;
    ViewDelegate* _pViewDelegate = nullptr;

private:
    void createView(CGRect frame);
};
