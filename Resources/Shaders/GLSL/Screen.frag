#version 420

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main(){
    vec4 tColor = texture(tex, texCoords * 2.0);
    if (tColor.a < 0.1f){
        discard;
    }
    FragColor = tColor; //vec4(1.0f - tColor.xyz, 1.0f);
}
