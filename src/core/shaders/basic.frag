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

	vec2 tex_coord = FragIn.texCoord;
	
	vec4 ray_start = texture(tex_frontface, tex_coord);
	vec4 ray_end = texture (tex_backface, tex_coord);

	// if(abs(ray_start.r - ray_end.r) > 0.01f) {
	// 	vec3 direction = (ray_end.xyz - ray_start.xyz);
	// 	float len = length(direction);
	// 	direction = normalize(direction);
	
	// 	//hard_stepsize = len / float(hard_samples);

	// 	int count = 0;
	// 	vec3 sample_pos = ray_start.xyz;
	// 	for(int i=0; i<hard_samples; ++i) {
	// 		sample_pos += direction * hard_stepsize;
	// 		color += texture(volumeTex, sample_pos);

	// 		if(color.r > 0.0001f) {
	// 			++count;
	// 		}
	// 	}

	// 	if(count > 0)
	// 	 	color /= float(count);

	// 	color.a = color.r;
	// 	color.rgb = vec3(1.f);
	// }

	tex_coord = FragIn.texCoord; //(FragIn.pixPos + vec2(1.f)) * 0.5f;// / 3.333333f; //FragIn.texCoord;
	color = vec4(0.0f);
	int count = 0;

	vec4 direction = (ray_end - ray_start);
	direction = normalize(direction);

	vec3 sample_pos = ray_start.xyz;

        for(int i=0; i<=hard_samples; ++i) {
            //color += texture(volumeTex , vec3(tex_coord.x, tex_coord.y, float(i)*hard_stepsize));
            color += texture(volumeTex , sample_pos);
                
            //if(color.r > 0.0001)
    	    //    ++count;

    	    sample_pos += direction.xyz * 1.f/float(hard_samples);
        }

        //if(count > 0)
          //      color /= float(count);

        //color.a = color.r;
        //color.rgb = vec3(1.f);

	out_Color = color;//vec4(ray_start.xyz, 1.0f);//texture(volumeTex, vec3(0.6f));//vec4(max(max(color.r, color.g), color.b));
}
