#version 330 core

out vec4 FragColor;
in vec4 vertex_color;
uniform float transp = 1.0;

void main() {
    FragColor = vertex_color;
}

