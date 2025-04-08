#version 330 

layout (location = 0) in vec4 bPos;
layout (location = 1) in vec2 vtexcoord0;
layout (location = 2) in vec4 incol;
layout (location = 3) in float offset;

uniform mat4 MVP;
out vec2 texcoord0;
out vec4 outcol;

void main()
{
	outcol=incol;
    gl_Position = MVP*vec4(bPos.x, bPos.y + offset, bPos.z, 1.0);
	texcoord0=vtexcoord0;
	
}