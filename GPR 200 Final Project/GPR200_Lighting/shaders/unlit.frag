#version 330                          
out vec4 FragColor;

in vec3 Color;
in vec3 WorldPos;
in vec3 WorldNormal;

uniform vec3 uColor;

void main(){         
    FragColor = vec4(Color * uColor,1.0f);
}
