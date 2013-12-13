#version 330

in VertexData {
    vec3 normal;
} FragIn;

out vec4 out_Color;

void main(void)
{
	vec4 color = vec4(1.0, 1.0, 0.0, 1.0);
	out_Color = color;
}
