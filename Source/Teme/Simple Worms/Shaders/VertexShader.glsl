#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// HeightMap coords
uniform sampler2D heightPixels;
uniform float terrain_size;

// Passing to Fragment Shader
out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
	// Calculate heights to be added to the plane and used to calculate normals
	float u = 1./terrain_size * 1./256. * v_position.x;
	float v = 1./terrain_size * 1./256. * v_position.z;
	float inaltime = texture2D(heightPixels, vec2(u, v)).r;
	float height = inaltime * 256. / 30. - 2;
	vec3 new_v_position = v_position + vec3(0, height, 0);

	// Calculate the new normal so the light works
	vec2 texelSize = vec2(1./terrain_size, 1./terrain_size);
	float heightRight = texture2D(heightPixels, vec2(u + texelSize.x, v)).r;
	float heightUp = texture2D(heightPixels, vec2(u, v + texelSize.y)).r;
	vec3 new_normal = normalize(vec3(5 * height - 5 * heightRight, 1, 5 * height - 5 * heightUp));

	// Values for the FragmentShader
	world_position = (Model * vec4(new_v_position, 1)).xyz;
	world_normal   = normalize( mat3(Model) * normalize(new_normal) );
	texcoord = v_texture_coord;

	gl_Position = Projection * View * Model * vec4(new_v_position, 1.0);
	
}
