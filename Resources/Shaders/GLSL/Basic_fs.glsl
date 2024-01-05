#version 420

in vec2 texCoords;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tex1;

void main(){
    vec4 tColor1 = texture(tex1, texCoords);
    if (tColor1.a < 0.1f){
        discard;
    }
    FragColor = tColor1;

}
