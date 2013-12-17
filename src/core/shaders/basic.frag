#version 330

in VertexData {
    //vec3 normal;
    //vec4 screen_space;
    //vec2 pixPos;
    vec2 texCoord;
} FragIn;

uniform sampler2D tex_frontface;
uniform sampler2D tex_backface;
uniform sampler3D volumeTex;

out vec4 out_Color;

void main(void)
{
	int actual_samples = 256;
	int hard_samples = 64;
	float hard_stepsize = 1.f/float(hard_samples);
	vec4 color = vec4(0.f);

	vec2 tex_coord = FragIn.texCoord; //vec2(gl_FragCoord.x / 640.f, gl_FragCoord.y / 480.f);
	
	vec3 ray_start = texture(tex_frontface, tex_coord).xyz;
	vec3 ray_end = texture (tex_backface, tex_coord).xyz;

	color = vec4(0.0f);
	int count = 0;

	vec3 direction = (ray_end - ray_start);
	float tend = length(direction);
	int numSteps = int(tend / hard_stepsize);
	direction = normalize(direction);

    for(int i=0; i<=numSteps; ++i) {
    	vec3 sample_pos = ray_start + (hard_stepsize*float(i)) * direction;
        //color += texture(volumeTex , vec3(tex_coord.x, tex_coord.y, float(i)*numSteps));
        color += texture(volumeTex , sample_pos);
            
        if(color.r > 0.0001)
	        ++count;

    }

    if(count > 0)
        color /= float(count);

    //color.a = color.r;
    //color.rgb = vec3(1.f);

	out_Color = color;//vec4(ray_start.xyz, 1.0f);//texture(volumeTex, vec3(0.6f));//vec4(max(max(color.r, color.g), color.b));
}
