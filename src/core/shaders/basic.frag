#version 330

in VertexData {
    vec3 normal;
    vec4 screen_space;
} FragIn;

uniform sampler3D volumeTex;

out vec4 out_Color;

void main(void)
{
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	
	vec2 normsscoord = FragIn.screen_space.xy*0.5 + 0.5;

	color = texture(volumeTex , vec3(0.5,0.5,0.5));//vec3(normsscoord, 0.5f));

	if(color.x != 0.0 || color.y != 0.0 || color.z != 0.0)
		color.x = 1.0;

	out_Color = color;
}