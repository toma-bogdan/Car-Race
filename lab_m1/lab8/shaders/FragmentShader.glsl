#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 light_position2;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 L2 = normalize(light_position2 - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 H2 = normalize(L2 + V);
	vec3 R = normalize(reflect(L, world_normal));
	vec3 R2 = normalize(reflect(L2, world_normal));
    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(normalize(N), L), 0.f);
	float diffuse_light2 = material_kd * max(dot(normalize(N), L2), 0.f);
    float specular_light = 0;
    float specular_light2 = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
		specular_light2 = material_ks * pow(max(dot(N, H2), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
	
	float light = 0.f;
	float light2 = 0.f;
    if (type_of_light == 1)
	{
		float cut_off_rad		= radians(cut_off_angle);
		float spot_light		= dot(-L, light_direction);
		float spot_light_limit	= cos(cut_off_rad);
		
		if (spot_light > spot_light_limit)
		{	 
			// Quadratic attenuation
			float linear_att		= (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			float light_att_factor	= linear_att * linear_att;
			light					= ambient_light + light_att_factor * (diffuse_light + specular_light);
			light2					= ambient_light + light_att_factor * (diffuse_light2 + specular_light2);
		} else
		{
			light = ambient_light;  // There is no spot light, but there is light from other objects
			light2 = ambient_light;
		}
	} else
	{
		float d						= distance(light_position, world_position);
		float attenuation_factor	= 1.f / max(d * d, 1.f);
		light						= ambient_light + attenuation_factor * (diffuse_light + specular_light);
		light2					    = ambient_light + attenuation_factor * (diffuse_light2 + specular_light2);
	}

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

	vec3 color = object_color * (light + light2);
    // TODO(student): Write pixel out color
    out_color = vec4(color,1.f);

}
