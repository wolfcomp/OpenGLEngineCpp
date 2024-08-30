#version 410
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragPos = vec3(model * vec4(position, 1.0));
    fragNormal = normal;
    fragTexCoord = texCoord;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}