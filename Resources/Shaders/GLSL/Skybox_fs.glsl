#version 460

in vec3 texCoords;

out vec4 FragColor;

layout(binding = 0) uniform samplerCube skybox;

void main(){
    vec4 tColor1 = texture(skybox, texCoords);
    FragColor = tColor1;

}
