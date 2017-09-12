#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 3) uniform sampler2D DepthImage;
layout (location = 0) in vec4 ConvCoord;

layout (location = 0) out vec4 color; 

void main(){
	float shadow = 0.0f;
	float Z = (ConvCoord.z/ConvCoord.w) - 0.001f; //Magic number. Because texture have infelicity

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

		}
	}

	shadow/=9;
			
	color = vec4(vec3(1.0f, 1.0f, 1.0f) * shadow, 1.0f);
}