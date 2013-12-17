#version 330

in VertexData {
    vec3 pos;
    vec2 texCoord;
} FragIn;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(FragIn.pos, 1.0f);
}
