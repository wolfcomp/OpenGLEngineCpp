#version 410
out vec4 fragColor;

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
    vec4 color;
    int hasMap;
};

struct PointLight {
    // struct Light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // struct PointLight
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    // struct Light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // struct DirLight
    vec3 direction;
};

struct SpotLight {
    // struct Light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // struct PointLight
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    // struct SpotLight
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform PointLight pointLights[4];
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform vec3 viewPos;

bool useSpecularMap() {
    return (material.hasMap & 2) != 0;
}

bool useNormalMap() {
    return (material.hasMap & 4) != 0;
}

bool useDiffuseMap() {
    return material.hasMap != 0;
}

vec4 GetMaterialDiffuse() {
    vec4 diffuse = material.color;
    if(useDiffuseMap()) {
        diffuse = vec4(texture(material.diffuse, fragTexCoord));
    }
    return diffuse;
}

vec3 GetMaterialSpecular() {
    vec3 specular = vec3(0.0);
    if(useSpecularMap()) {
        specular = vec3(texture(material.specular, fragTexCoord));
    }
    return specular;
}

vec3 GetMaterialNormal() {
    vec3 normal = normalize(fragNormal);
    if(useNormalMap()) {
        normal = normalize(texture(material.normal, fragTexCoord).rgb * 2.0 - 1.0);
    }
    return normal;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec4 ambient = vec4(light.ambient, 1.0) * GetMaterialDiffuse();

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * (diff * GetMaterialDiffuse());

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * GetMaterialSpecular());

    return ambient + diffuse + vec4(specular, 1.0);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = vec4(light.ambient, 1.0) * GetMaterialDiffuse();

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * (diff * GetMaterialDiffuse());

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * GetMaterialSpecular());

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + vec4(specular, 1.0);
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient = vec4(light.ambient, 1.0) * GetMaterialDiffuse();

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * (diff * GetMaterialDiffuse());

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * GetMaterialSpecular());

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + vec4(specular, 1.0);
}

vec4 BlendLightColor(vec4 color, vec4 lightColor) {
    return (lightColor.r > 0.0 || lightColor.g > 0.0 || lightColor.b > 0.0) ? color * lightColor : color;
}

void main() {
    vec3 normal = GetMaterialNormal();
    vec3 viewDir = normalize(viewPos - fragPos);

    vec4 result = CalcDirLight(dirLight, normal, viewDir);
    for(int i = 0; i < 4; i++) {
        result = BlendLightColor(result, CalcPointLight(pointLights[i], normal, fragPos, viewDir));
    }
    result = BlendLightColor(result, CalcSpotLight(spotLight, normal, fragPos, viewDir));

    fragColor = result;
}