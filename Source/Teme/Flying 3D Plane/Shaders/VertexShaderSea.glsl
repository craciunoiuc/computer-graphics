#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;
uniform int rand;

out vec3 color;

void main()
{
	// Compute world space vectors
	vec3 world_position = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal	= normalize(mat3(Model) * normalize(v_normal));
	
	vec3 normalised = normalize(light_position - world_position);

	// Define ambient light component
	float ambient_light = 0.97;

	// Compute diffuse light component
	float diffuse_light = material_kd * max(dot(world_normal, normalised), 0);

	// Compute specular light component
	float specular_light = 0;
	float additional_light = 2;

	if (diffuse_light > 0) {
		specular_light = material_ks * pow(max(dot(world_normal, normalised), 0), 
						 material_shininess) + additional_light;
	}

	// Compute light
	float d						= distance(light_position, v_position);
	float attenuation_factor	= 1 / (1 + 0.2 * d + 0.08 * d * d);
	float light					= ambient_light + attenuation_factor * (diffuse_light + 
								  specular_light);

	// Send color light output to fragment shader
	color = object_color * light;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
