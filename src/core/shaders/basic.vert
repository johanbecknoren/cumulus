#version 330

layout(location = 0) in  vec3  in_Position;
layout(location = 1) in  vec3  in_Normal;
layout(location = 2) in  vec2  in_texCoord;

uniform mat4 camTrans;

out VertexData {
    vec3 normal;
    vec4 screen_space;
} VertexOut;

void main(void)
{
	vec4 spacePos =  camTrans * vec4(in_Position, 1.0);
	
    VertexOut.normal = in_Normal;
    VertexOut.screen_space = spacePos / spacePos.w;

    gl_Position = spacePos;
}
