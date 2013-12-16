#version 330

in VertexData {
    vec3 normal;
    vec4 screen_space;
    vec2 pixPos;
    vec2 texCoord;
} FragIn;

uniform sampler3D volumeTex;

out vec4 out_Color;

void main(void)
{
	int hard_samples = 64;
	float hard_stepsize = 1.f/float(hard_samples);
	vec4 color = vec4(0.f);
	

	vec2 tex_coord = (FragIn.pixPos + vec2(1.f)) * 0.5f;// / 3.333333f; //FragIn.texCoord;
	int count =0;

	for(int i=0; i<=hard_samples; ++i) {
		

		color += texture(volumeTex , vec3(tex_coord.x, tex_coord.y, float(i)*hard_stepsize));
		
		if(color.r > 0.0001)
			++count;

	}

	if(count > 0)
		color /= float(count);

	color.a = color.r;
	color.rgb = vec3(1.f);
	out_Color = color;//vec4(max(max(color.r, color.g), color.b));
}
