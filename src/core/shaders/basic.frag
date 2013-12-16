#version 330

in VertexData {
    vec3 normal;
    vec4 screen_space;
    vec2 pixPos;
    vec2 texCoord;
} FragIn;

uniform sampler3D volumeTex;
/*uniform int samples;
uniform float stepSize;*/

out vec4 out_Color;

void main(void)
{
	int hard_samples = 25;
	float hard_stepsize = 1.f/float(hard_samples);
	vec4 color = vec4(0.f);
	
	//vec2 normsscoord = FragIn.screen_space.xy*0.5 + 0.5;

	vec2 tex_coord = (FragIn.pixPos + vec2(1.f)) * 0.5f;// / 3.333333f; //FragIn.texCoord;
	int count =0;

	//for(int i=0; i<=samples; ++i) {
	for(int i=0; i<=hard_samples; ++i) {
		
		//color += texture(volumeTex , vec3(tex_coord.x, tex_coord.y, float(i)*stepSize));
		color += texture(volumeTex , vec3(tex_coord.x, tex_coord.y, float(i)*hard_stepsize));
		
		if(color.r > 0.0001)
			++count;

	}

	if(count > 0)
		color /= float(count);

	//color = texture(volumeTex , vec3(texCoord.x, texCoord.y, 0.3f)); //vec3(0.5,0.5,0.5));//vec3(normsscoord, 0.5f));

	/*if(color.x != 0.0 || color.y != 0.0 || color.z != 0.0)
		color.x = 0.5f;*/

	out_Color = color;//vec4(max(max(color.r, color.g), color.b));
}
