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

// Function to create a perspective projection matrix
float4x4 perspective_projection(float fovY, float aspect, float near, float far) {
    float yScale = 1.0 / tan(fovY / 2.0);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far + near) / zRange;
    float wzScale = -2.0 * far * near / zRange;

    return float4x4(
        float4(xScale, 0.0, 0.0, 0.0),
        float4(0.0, yScale, 0.0, 0.0),
        float4(0.0, 0.0, zScale, -1.0),
        float4(0.0, 0.0, wzScale, 0.0)
    );
}

// Function to create a look-at view matrix
float4x4 look_at(float3 eye, float3 center, float3 up) {
    float3 zAxis = normalize(eye - center);
    float3 xAxis = normalize(cross(up, zAxis));
    float3 yAxis = cross(zAxis, xAxis);

    return float4x4(
        float4(xAxis, 0.0),
        float4(yAxis, 0.0),
        float4(zAxis, 0.0),
        float4(-dot(xAxis, eye), -dot(yAxis, eye), -dot(zAxis, eye), 1.0)
    );
}

Vertex vertex vertexMain(uint vertexId [[vertex_id]],
                         device const float4* positions [[buffer(0)]],
                         device const float4* colors [[buffer(1)]])
{
    // Define the Model matrix (identity for simplicity)
    float4x4 model = float4x4(1.0);

    // Define the View matrix
    float3 eye = float3(0.0, 2.0, 3.0);
    float3 center = float3(0.0, 0.0, 0.0);
    float3 up = float3(0.0, 1.0, 0.0);
    float4x4 view = look_at(eye, center, up);

    // Define the Projection matrix
    float aspectRatio = 1.0; // Assuming a square viewport
    float fov = 45.0 * (3.14159265359 / 180.0); // Convert to radians
    float near = 0.1;
    float far = 100.0;
    float4x4 projection = perspective_projection(fov, aspectRatio, near, far);

    // Combine the model, view, and projection matrices
    float4x4 mvp = projection * view * model;

    Vertex vo;
    vo.position = mvp * positions[vertexId];
    vo.color = half4(colors[vertexId]);
    return vo;
}

half4 fragment fragmentMain(Vertex inVertex [[stage_in]])
{
    return half4(inVertex.color);
}
