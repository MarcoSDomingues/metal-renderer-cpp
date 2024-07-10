//
//  Matrix.hpp
//  MetalEngine
//
//  Created by Marco Domingues on 10/07/2024.
//

#pragma once
#include <simd/simd.h>

class Matrix {
public:
    static simd::float4x4 createLookAt(simd::float3 eye, simd::float3 center, simd::float3 up);
    static simd::float4x4 createPerspective(float fovY, float aspect, float near, float far);
};
