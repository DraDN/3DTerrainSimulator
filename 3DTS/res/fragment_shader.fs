#version 430
in vec2 TexCoords;
in float height;

out vec4 FragColor;
in vec3 FragPos;

layout (binding = 0) uniform sampler2D normalMap;

// camera position
uniform vec3 view_pos;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light light;

struct Material {
    float max_height;
    float max_steepness;
    float blend;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

Material default_material = Material(0.0, 0.0, 0.0, vec3(1.0), 0.0, 1.0, 0.0);

layout (std430, binding = 1) buffer MaterialBuffer {
    Material materials[];
};

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 apply_lighting_to_material(Material mat) {
    vec3 normal = texture(normalMap, TexCoords).rgb;
    vec3 N = normalize(normal * 2.0 - 1.0);
    vec3 V = normalize(view_pos - FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, mat.albedo, mat.metallic);

    vec3 Lo = vec3(0.0);
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L);

    // float distance = length(light.position - FragPos);
    // float attenuation = 1.0 / (distance * distance);
    float attenuation = 1.0;
    vec3 radiance = light.color * attenuation;

    float NDF = DistributionGGX(N, H, mat.roughness);
    float G = GeometrySmith(N, V, L, mat.roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - mat.metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * mat.albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * mat.albedo * mat.ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    return color;
}

void main()
{
    vec3 result;
	// result = vec3(1.f);

    Material output_material = default_material;

    for (int i = 0; i < materials.length(); ++i) {
        if (height <= materials[i].max_height) {
            output_material = materials[i];
        }
    }

    // result = texture(normalMap, TexCoords).rgb;
    result = apply_lighting_to_material(output_material);
    // result = output_material.albedo;
    // result = vec3(TexCoords.x, TexCoords.y, 0.f);

    FragColor = vec4(result, 1.0);
}