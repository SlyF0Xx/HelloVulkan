#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 normcoord;
layout (location = 2) in vec3 norm;
//layout (location = 3) in vec3 tang;
//layout (location = 4) in vec3 bitang;
layout (location = 3) in vec2 texcoord;

layout (binding = 0) uniform CamMatrixes{
	mat4 View;
	mat4 Projection;
}ViewProjMat;

//may be dynamic
layout (binding = 1) uniform WorldMatrix{
	mat4 World;
}WorldMat;

//may be dynamic
layout (binding = 2) uniform TransMatrix{
	mat4 Transformation;
	vec3 LightPos;
	vec3 LightDir;
}TransMat;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

layout (location = 0) out vec4 ConvCoord;
layout (location = 1) out vec2 NormCoord;
layout (location = 2) out vec3 LightDir;
layout (location = 3) out vec3 LightPosDir;
layout (location = 4) out vec2 TexCoord;

void main() {
	mat4 Matrix = ViewProjMat.Projection  * ViewProjMat.View;
	mat4 Matrixs = Matrix * WorldMat.World ;
	gl_Position = Matrixs * vec4(position, 1.0f);
	mat4 Converted = TransMat.Transformation * WorldMat.World;
	ConvCoord = Converted * vec4(position, 1.0f);

	/*vec3 bitang = vec3(-norm.y, norm.x, 0);
	vec3 tang = vec3(-norm.x*norm.z, -norm.z*norm.y, norm.x*norm.x+norm.y*norm.y);

	if(bitang == vec3(0))
	{
		bitang = vec3(0.0, 1.0, 0.0);
		tang = vec3(1.0, 0.0, 0.0);
	}
	else
	{
		bitang = normalize(bitang);
		tang = normalize(tang);
	}*/

	vec3 binormal = cross(vec3(1,0,0),norm);
    vec3 tang = cross(norm,binormal);

	mat3 matrix;
	matrix[0] = tang;
	matrix[1] = binormal;
	matrix[2] = norm;

	LightPosDir = normalize(matrix * (normalize(TransMat.LightPos - vec3( WorldMat.World * vec4(position, 1.0f)) ) ));
	LightDir  = normalize( matrix * TransMat.LightDir);

	NormCoord = normcoord;
	TexCoord = texcoord;
}
