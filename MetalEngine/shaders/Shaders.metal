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

struct VertexData
{
    device float4* positions [[id(0)]];
    device float4* colors [[id(1)]];
};

struct Uniforms
{
    float4x4 modelViewProjectionMatrix;
};

Vertex vertex vertexMain(uint vertexId [[vertex_id]],
                         device const VertexData* vertexData [[buffer(0)]],
                         constant Uniforms *uniforms [[buffer(1)]])
{
    Vertex vo;
    vo.position = uniforms->modelViewProjectionMatrix * vertexData->positions[vertexId];
    vo.color = half4(vertexData->colors[vertexId]);
    return vo;
}

half4 fragment fragmentMain(Vertex inVertex [[stage_in]])
{
    return half4(inVertex.color);
}
