#version 330

in VertexData {
    vec3 pos;
    vec2 texCoord;
} FragIn;

/*uniform sampler3D volumeTex;
uniform int samples;
uniform float stepSize;*/

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(1.0f);//vec4(FragIn.pos, 1.0f);
}
