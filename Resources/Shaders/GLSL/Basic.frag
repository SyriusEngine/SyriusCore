#version 460

layout(location = 2) in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main(){
    vec4 tColor = texture(tex, texCoords);
    if (tColor.a < 0.1f){
        discard;
    }
    FragColor = tColor;
}
