#version 450 core

layout(location = 0) in vec4 vp;
layout(location = 1) in vec3 texid1;
layout(location = 2) in vec3 texid2;
layout(location = 3) in vec3 multiplier;
out Vertex
{
	vec3 pos;
	uint occl;
	vec3 texid1;
	vec3 texid2;
	vec3 scale;
} vertex;


void main()
{
	vertex.texid1	=	texid1;
	vertex.texid2	=	texid2;
	vertex.occl		=	uint(vp.w);
	vertex.pos		=	vp.xyz;
	vertex.scale    =	multiplier;
}