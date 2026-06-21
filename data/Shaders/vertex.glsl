#version 330 core

layout (location = 0) in vec3 aPos;   // x,y,z
layout (location = 1) in vec3 aColor; // r,g,b

out vec3 vColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;
}
