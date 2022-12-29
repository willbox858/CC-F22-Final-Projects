#version 330     
layout (location = 0) in vec3 in_Pos;  
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec3 in_Normal;

out vec3 Color;

out vec3 WorldPos;
out vec3 WorldNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){       
    Color = in_Color;
    gl_Position = uProjection * uView * uModel * vec4(in_Pos,1);

    WorldPos = vec3(uModel * vec4(in_Pos,1.0));

    WorldNormal = mat3(transpose(inverse(uModel))) * in_Normal;

}