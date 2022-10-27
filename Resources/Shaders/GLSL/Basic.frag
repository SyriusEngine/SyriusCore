#version 460

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main(){
    vec4 tColor1 = texture(tex1, texCoords);
    vec4 tColor2 = texture(tex2, texCoords);
    vec4 tempColor = mix(tColor1, tColor2, 0.2);
    if (tempColor.a < 0.1f){
        discard;
    }
    FragColor = tempColor;

}
