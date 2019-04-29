#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices=36) out;

uniform mat4 MVP;
uniform float ITime;

out vec2 uv;
out float time;

vec4 GlobalPoint1 = MVP * vec4(-0.5, -0.5,  0.5, 1);
vec4 GlobalPoint2 = MVP * vec4( 0.5, -0.5,  0.5, 1);
vec4 GlobalPoint3 = MVP * vec4( 0.5,  0.5,  0.5, 1);
vec4 GlobalPoint4 = MVP * vec4(-0.5, -0.5, -0.5, 1);
vec4 GlobalPoint5 = MVP * vec4( 0.5, -0.5, -0.5, 1);
vec4 GlobalPoint6 = MVP * vec4( 0.5,  0.5, -0.5, 1);
vec4 GlobalPoint7 = MVP * vec4(-0.5,  0.5,  0.5, 1); 
vec4 GlobalPoint8 = MVP * vec4(-0.5,  0.5, -0.5, 1);


void main()
{
	time = ITime;
	vec4 pos = MVP * vec4(gl_in[0].gl_Position.xyz, 1);

	vec4 Point1 = pos + GlobalPoint1;
	vec4 Point2 = pos + GlobalPoint2;
	vec4 Point3 = pos + GlobalPoint3;
	vec4 Point4 = pos + GlobalPoint4;
	vec4 Point5 = pos + GlobalPoint5;
	vec4 Point6 = pos + GlobalPoint6;
	vec4 Point7 = pos + GlobalPoint7;
	vec4 Point8 = pos + GlobalPoint8;

	gl_Position = Point1;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point2;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point3;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point1;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point3;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point7;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point7;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point3;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point8;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = Point3;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point6;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point8;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point3;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point2;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point5;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point3;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point5;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point6;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point7;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point8;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point1;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point1;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point8;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point4;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point8;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point6;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point5;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	EndPrimitive();
	
	gl_Position = Point8;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	gl_Position = Point5;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point4;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point5;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point2;
	uv = vec2(1.f, 1.f);
	EmitVertex();
	gl_Position = Point1;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	EndPrimitive();

	gl_Position = Point5;
	uv = vec2(1.f, 0.f);
	EmitVertex();
	gl_Position = Point1;
	uv = vec2(0.f, 1.f);
	EmitVertex();
	gl_Position = Point4;
	uv = vec2(0.f, 0.f);
	EmitVertex();
	EndPrimitive();
}