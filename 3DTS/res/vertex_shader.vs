#version 420
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoords;

// out vec3 FragPos;
out vec2 TexCoords;
// out vec4 FragPosLightSpace;

// out float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform mat4 light_space_matrix;

void main()
{
    // height = position.y;
    // FragPos = vec3(model * vec4(position, 1.0));
    TexCoords = aTexCoords;
    vec3 new_position = vec3(position.x, position.y / 2.f, position.z);
    // FragPosLightSpace = light_space_matrix * vec4(FragPos, 1.0);

    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(position, 1.f);
    // gl_Position = vec4(position, 1.f);
}