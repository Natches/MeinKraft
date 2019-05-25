#version 450 core

out vec4 frag_color;

in vec2 uv;
in float time;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	frag_color = vec4(rand(vec2(rand(uv + time), time)), rand(vec2(time, time)), rand(vec2(rand(uv), time)), 1);
}