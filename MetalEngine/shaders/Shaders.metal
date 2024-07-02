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

Vertex vertex vertexMain(uint vertexId [[vertex_id]],
                         device const float4* positions [[buffer(0)]],
                         device const float4* colors [[buffer(1)]])
{
    Vertex v;
    v.position = positions[vertexId];
    v.color = half4(colors[vertexId]);
    return v;
}

half4 fragment fragmentMain(Vertex inVertex [[stage_in]])
{
    return half4(inVertex.color);
}
