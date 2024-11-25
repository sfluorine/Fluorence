#version 460 core

out vec4 FragColor;

in vec2 v_uv;

uniform sampler2D u_sampler;

void main()
{
    FragColor = texture(u_sampler, v_uv);
}