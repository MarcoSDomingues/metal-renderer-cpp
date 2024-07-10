//
//  Matrix.cpp
//  MetalEngine
//
//  Created by Marco Domingues on 10/07/2024.
//

#include "Matrix.hpp"

simd::float4x4 Matrix::createLookAt(simd::float3 eye, simd::float3 center, simd::float3 up)
{
    simd::float3 zAxis = simd::normalize(eye - center);
    simd::float3 xAxis = simd::normalize(simd::cross(up, zAxis));
    simd::float3 yAxis = simd::cross(zAxis, xAxis);

    return simd::float4x4(
        simd::make_float4(xAxis, 0.0),
        simd::make_float4(yAxis, 0.0),
        simd::make_float4(zAxis, 0.0),
        simd::make_float4(-simd::dot(xAxis, eye), -simd::dot(yAxis, eye), -simd::dot(zAxis, eye), 1.0)
    );
}

simd::float4x4 Matrix::createPerspective(float fovY, float aspect, float near, float far)
{
    float yScale = 1.0 / tan(fovY / 2.0);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far + near) / zRange;
    float wzScale = -2.0 * far * near / zRange;

    return simd::float4x4(
        simd::make_float4(xScale, 0.0, 0.0, 0.0),
        simd::make_float4(0.0, yScale, 0.0, 0.0),
        simd::make_float4(0.0, 0.0, zScale, -1.0),
        simd::make_float4(0.0, 0.0, wzScale, 0.0)
    );
}
