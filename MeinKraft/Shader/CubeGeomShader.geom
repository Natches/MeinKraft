#version 450 core
 
layout(points) in;
layout(triangle_strip, max_vertices=36) out;

in Vertex
{
	vec3 pos;
	uint occl;
	vec3 texid1;
	vec3 texid2;
	vec3 scale;
} vertex[];

uniform mat4 VP;
uniform float Fov;
uniform vec3 CameraPos;
uniform vec3 CameraDir;
uniform float ViewDistance;


out vec2 uv;
flat out float idGS;
flat out vec3 scale;
flat out int Break;
flat out int XZ;
flat out int XY;
flat out int YZ;
flat out int skyOcclusion;

void main()
{
	XZ = 0;
	XY = 0;
	YZ = 0;
	vec3 CamToVert = vertex[0].pos - CameraPos;
	float angle = acos(dot(CameraDir, normalize(CamToVert)));
	scale = vertex[0].scale;
	if(angle <= Fov && length(CamToVert) < ViewDistance)
	{
		mat4 M;
		M[0] = vec4(vertex[0].scale.x, 0, 0, vertex[0].pos.x);
		M[1] = vec4(0, vertex[0].scale.y, 0, vertex[0].pos.y);
		M[2] = vec4(0, 0, vertex[0].scale.z, vertex[0].pos.z);
		M[3] = vec4(0, 0, 0, 1);

		M = transpose(M);

		mat4 MVP = VP * M;

		vec4 GlobalPoint1 = MVP * vec4(-0.5, -0.5,  0.5, 1);
		vec4 GlobalPoint2 = MVP * vec4( 0.5, -0.5,  0.5, 1);
		vec4 GlobalPoint3 = MVP * vec4( 0.5,  0.5,  0.5, 1);
		vec4 GlobalPoint4 = MVP * vec4(-0.5, -0.5, -0.5, 1);
		vec4 GlobalPoint5 = MVP * vec4( 0.5, -0.5, -0.5, 1);
		vec4 GlobalPoint6 = MVP * vec4( 0.5,  0.5, -0.5, 1);
		vec4 GlobalPoint7 = MVP * vec4(-0.5,  0.5,  0.5, 1); 
		vec4 GlobalPoint8 = MVP * vec4(-0.5,  0.5, -0.5, 1);
		
		uint i = uint(vertex[0].occl);
		if((i & (1 << 7)) == 1 << 7)
			Break = 1;
		else
			Break = 0;
		//Right
		if((i & (1 << 0)) != 1 << 0 && CameraPos.x >= vertex[0].pos.x + 0.5f)
		{
			skyOcclusion = 1;
			YZ = 1;
			idGS = vertex[0].texid2.x; 
			gl_Position = GlobalPoint3;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint2;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint5;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();

			gl_Position = GlobalPoint3;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint5;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint6;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			EndPrimitive();
		}
		//
		
		//Left
		else if((i & (1 << 1)) != 1 << 1 && CameraPos.x <= vertex[0].pos.x - 0.5f)
		{
			skyOcclusion = 1;
			YZ = 1;
			idGS = vertex[0].texid1.z; 
			gl_Position = GlobalPoint7;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint8;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint1;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();

			gl_Position = GlobalPoint1;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint8;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint4;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			EndPrimitive();
		}
		//
		
		//Up
		if((i & (1 << 2)) != 1 << 2 && CameraPos.y >= vertex[0].pos.y + 0.5f)
		{
			YZ = 0;
			XZ = 1;
			if((i & (1 << 6)) != 1 << 6)
				skyOcclusion = 0;
			else
				skyOcclusion = 1;
			idGS = vertex[0].texid1.x; 
			gl_Position = GlobalPoint7;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint3;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint8;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();
			
			gl_Position = GlobalPoint3;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint6;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint8;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();
		}
		//
		
		//Down
		else if((i & (1 << 3)) != 1 << 3 && CameraPos.y  <= vertex[0].pos.y - 0.5f)
		{
			skyOcclusion = 1;
			YZ = 0;
			XZ = 1;
			idGS = vertex[0].texid1.y; 
			gl_Position = GlobalPoint5;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint2;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint1;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();

			gl_Position = GlobalPoint5;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint1;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint4;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			EndPrimitive();
		}
		//
		
		//Forward
		if((i & (1 << 4)) != 1 << 4 && CameraPos.z  >= vertex[0].pos.z + 0.5f )
		{
			skyOcclusion = 1;
			YZ = 0;
			XZ = 0;
			XY = 1;
			idGS = vertex[0].texid2.y; 
			gl_Position = GlobalPoint1;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint2;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint3;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			EndPrimitive();

			gl_Position = GlobalPoint1;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint3;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint7;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			EndPrimitive();
		}
		//
		
		//Back
		else if((i & (1 << 5)) != 1 << 5 && CameraPos.z <= vertex[0].pos.z - 0.5f)
		{
			skyOcclusion = 1;
			YZ = 0;
			XZ = 0;
			XY = 1;
			idGS = vertex[0].texid2.z; 
			gl_Position = GlobalPoint8;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint6;
			uv = vec2(1.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint5;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			EndPrimitive();
			
			gl_Position = GlobalPoint8;
			uv = vec2(0.f, 0.f);
			EmitVertex();
			gl_Position = GlobalPoint5;
			uv = vec2(1.f, 1.f);
			EmitVertex();
			gl_Position = GlobalPoint4;
			uv = vec2(0.f, 1.f);
			EmitVertex();
			EndPrimitive();
		}
		//
	}
}