#version 420
in vec2 TexCoords;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D normalMap;

void main()
{
    vec3 result;
	// result = vec3(1.f);

    result = texture(normalMap, TexCoords).rgb;
    // result = vec3(TexCoords.x, TexCoords.y, 0.f);

    FragColor = vec4(result, 1.0);
}