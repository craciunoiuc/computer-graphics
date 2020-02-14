#version 330
 
// Get color value from vertex shader
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

//Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

// Material uniforms
uniform float material_kd;
uniform float material_ks;
uniform float material_shininess;

// Where to stop the spotlight
uniform float cut_off_angle;

// Texture to be put on objects
uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;

void main()
{
	// Normal vector
	vec3 N = normalize(world_normal);

	// Vision vector
	vec3 V = normalize(eye_position - world_position);

	// Light vector
	vec3 L = normalize(light_position - world_position);

	// Median vector
	vec3 H = normalize(L + V);

	// Same but for the second light
	vec3 L2 = normalize(light_position + vec3(18.1525, -2.95079, 1.0535) - world_position);
	vec3 H2 = normalize(L2 + V);

	// Define ambient light component
	float ambient_light = 0.8;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(N), L), 0) * 6;
	float diffuse_light2 = material_kd * max(dot(normalize(N), L2), 0) * 6;

	// Compute specular light component
	float specular_light = 0;
	float specular_light2 = 0;


	// Calculate the specular light with the formula from the lab
	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess) * 6;
	}

	if (diffuse_light2 > 0)
	{
		specular_light2 = material_ks * pow(max(dot(normalize(N), H2), 0), material_shininess) * 6;
	}

	// Compute light
	float light = 0;
	float light2 = 0;
	float cut_off_rad		= radians(cut_off_angle);
	float spot_light		= dot(-L, light_direction);
	float spot_light2		= dot(-L2, light_direction);
	float spot_light_limit	= cos(cut_off_rad);
	
	// Player1 spotlight
	if (spot_light > spot_light_limit)
	{	 
		// Quadratic attenuation
		float linear_att		= (spot_light - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor	= pow(linear_att, 2);
		light					= ambient_light + light_att_factor * (diffuse_light + specular_light);
	} else
	{
		light = ambient_light;
	}

	// Same thing but for Player2
	if (spot_light2 > spot_light_limit)
	{	 
		// Quadratic attenuation
		float linear_att		= (spot_light2 - spot_light_limit) / (1 - spot_light_limit);
		float light_att_factor	= pow(linear_att, 2);
		light2					= ambient_light + light_att_factor * (diffuse_light2 + specular_light2);
	} else
	{
		light2 = ambient_light;
	}

	// The color is taken from the texture and light is applied to it
	vec4 color = texture2D(texture_1, texcoord);
	vec3 colorAux = vec3(color.x, color.y, color.z);
	colorAux = colorAux * light * light2;
	out_color = vec4(colorAux, 1);
}