#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function

	vec4 color1 = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);
	
	vec4 color = mix(color1, color2, 0.5f);
	if (color.w < 0.5) {
		discard;
	}
	out_color = color;
}

/*
vec3 N = normalize(world_normal);
	vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 H = normalize(L + V);

	// Define ambient light component
	float ambient_light = 0.3;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(N), L), 0);

	// Compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);
	}

	// Compute light
	float light = 0;

	if (type_of_light == 1)
	{
		float cut_off_rad		= radians(cut_off_angle);
		float spot_light		= dot(-L, light_direction);
		float spot_light_limit	= cos(cut_off_rad);
		
		if (spot_light > spot_light_limit)
		{	 
			// Quadratic attenuation
			float linear_att		= (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor	= pow(linear_att, 2);
			light					= ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else
		{
			light = ambient_light;  // There is no spot light, but there is light from other objects
		}
	} else
	{
		light = ambient_light + diffuse_light + specular_light;
	}
*/