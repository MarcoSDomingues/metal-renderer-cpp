//
//  Shaders.metal
//  MetalEngine
//
//  Created by Marco Domingues on 02/07/2024.
//

#include <metal_stdlib>
using namespace metal;


struct Vertex
{
    float4 position [[position]];
    half4 color;
};

struct Uniforms
{
    float4x4 modelViewProjectionMatrix;
};

Vertex vertex vertexMain(uint vertexId [[vertex_id]],
                         device const float4* positions [[buffer(0)]],
                         device const float4* colors [[buffer(1)]],
                         constant Uniforms *uniforms [[buffer(2)]])
{
    Vertex vo;
    vo.position = uniforms->modelViewProjectionMatrix * positions[vertexId];
    vo.color = half4(colors[vertexId]);
    return vo;
}

half4 fragment fragmentMain(Vertex inVertex [[stage_in]])
{
    return half4(inVertex.color);
}
