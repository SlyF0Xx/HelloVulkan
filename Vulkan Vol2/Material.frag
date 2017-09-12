#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable


layout (binding = 3) uniform sampler2D DepthImage;
layout (binding = 4) uniform sampler2D NormTexture;
layout (binding = 5) uniform sampler2D Texture;

layout (location = 0) in vec4 ConvCoord;
layout (location = 1) in vec2 NormCoord;
layout (location = 2) in vec3 LightDir;
layout (location = 3) in vec3 LightPosDir;
layout (location = 4) in vec2 TexCoord;

layout (location = 0) out vec4 color; 

void main(){
	float shadow = 0.0f;
	float Z = (ConvCoord.z/ConvCoord.w) - 0.001f; //Magic number. Because texture have infelicity
	float light = 0.0f;
	for(int i = -1; i< 2; i++)
	{
		for(int j = -1; j<2; j++)
		{
			//Checking if coordinates located in texture
			// 884 and 561 is screen width and height
			if((ConvCoord.x/ConvCoord.w + i/884)*0.5+0.5 <=1 && (ConvCoord.x/ConvCoord.w + i/884)*0.5+0.5 >= 0 &&
				(ConvCoord.y/ConvCoord.w +j/561)*0.5+0.5<=1 && (ConvCoord.y/ConvCoord.w + j/561)*0.5+0.5 >= 0)
			{
				float Depth = texture(DepthImage, vec2((ConvCoord.x/ConvCoord.w + i/884)*0.5+0.5, (ConvCoord.y/ConvCoord.w + j/561)*0.5+0.5)).r;
				if (Depth >= Z && Depth>0)
					shadow += 1.0f;
				else
					shadow += 0.5f;
			}
			else
				shadow += 0.5f;
			

			/*vec3 bumpNormal = (texture(NormTexture, vec2(NormCoord.x+i/884, NormCoord.y + j/561) )*2.0-1.0).xyz;

			light += ((dot(LightDir, LightPosDir)<(-sqrt(2.0f)/3.0f)) ? dot(bumpNormal, LightPosDir) : 0.1f) +0.1f;
			*/
		}
	}

	shadow/=9;
	//light/=9;

	vec3 bumpNormal = (texture(NormTexture, NormCoord)*2.0-1.0).xyz;

	light = ((dot(LightDir, LightPosDir)<(-sqrt(2.0f)/3.0f) && dot(bumpNormal, LightPosDir) > 0) ? dot(bumpNormal, LightPosDir) : (dot(bumpNormal, LightPosDir)+1)*0.7f ) +0.1f;
			
	
	vec3 texture = vec3(texture(Texture, TexCoord));

	color = vec4(texture * light * shadow, 1.0f);
}