//
//  NSWindowExtensions.h
//  MetalEngine
//
//  Created by Marco Domingues on 19/07/2024.
//

/*
#pragma once

#include "AppKit/AppKitPrivate.hpp"

namespace NS::Private::Selector {

_APPKIT_PRIVATE_DEF_SEL( contentView,
                        "contentView" );

_APPKIT_PRIVATE_DEF_SEL( setLayer_,
                        "setLayer:" );

_APPKIT_PRIVATE_DEF_SEL( setWantsLayer_,
                        "setWantsLayer:" );
}

namespace NS::View {
public:
    void            setLayer( const Object* pLayer );
    void            setWantsLayer( bool wantsLayer );
}

_NS_INLINE void NS::View::setLayer( const NS::Object* pLayer )
{
    Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setLayer_ ), pLayer );
}

_NS_INLINE void NS::View::setWantsLayer( bool wantsLayer )
{
    Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setWantsLayer_ ), wantsLayer );
}

*/
