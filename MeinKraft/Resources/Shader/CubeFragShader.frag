#version 450 core

out vec4 frag_color;

in vec2 uv;
flat in float idGS;
flat in vec3 scale;
flat in int Break;
flat in int XZ;
flat in int XY;
flat in int YZ;
flat in int skyOcclusion;

layout(binding = 0)
uniform sampler2D texMap;
uniform float offset;
uniform bool isWireframe;
uniform vec3 color;
uniform vec3 ambient;
uniform int	 breakTexture;
uniform float maxLightLevel;
uniform float minLightLevel;


float widthTexMap = textureSize(texMap, 0).x;
float heightTexMap = textureSize(texMap, 0).y;

void main()
{
	if(!isWireframe)
	{
		float offWidth = offset / widthTexMap;
		uint offsetX = uint(idGS) % 16;
		uint offsetY = uint(idGS / (heightTexMap / offset));
		float uvX = 0.f;
		float uvY = 0.f;
		if( XY == 1 )
		{
			uvX = (mod((uv.x * scale.x), 1.f) + float(offsetX)) * offWidth;
			uvY = (mod((uv.y * scale.y), 1.f) + float(offsetY)) * offWidth;
		}
		else if(XZ == 1)
		{
			uvX = (mod((uv.x * scale.x), 1.f) + float(offsetX)) * offWidth;
			uvY = (mod((uv.y * scale.z), 1.f) + float(offsetY)) * offWidth;
		}
		else if(YZ == 1)
		{
			uvX = (mod((uv.x * scale.z), 1.f) + float(offsetX)) * offWidth;
			uvY = (mod((uv.y * scale.y), 1.f) + float(offsetY)) * offWidth;
		}

		frag_color = vec4(texture(texMap, vec2(uvX , uvY )).xyz, texture(texMap, vec2(uvX , uvY )).w);
		if(Break == 1)
		{
			offsetX = uint(breakTexture) % 16;
			offsetY = uint(breakTexture / (heightTexMap / offset));
			uvX = (mod((uv.x * scale.z), 1.f) + float(offsetX)) * offWidth;
			uvY = (mod((uv.y * scale.y), 1.f) + float(offsetY)) * offWidth;

			frag_color *= vec4(texture(texMap, vec2(uvX , uvY )).xyz, 1.f);
		}

		if( skyOcclusion == 0 )
			frag_color*= vec4(ambient * maxLightLevel, 1.f);
		else
			frag_color*= vec4(ambient * minLightLevel, 1.f);

		if ( frag_color.w == 0.f)
			discard;
	}
	else
	{
		frag_color = vec4(color.xyz, 1.f);
	}
}