#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 position;

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

void main() {
	mat4 Matrix = ViewProjMat.Projection  * ViewProjMat.View;
	mat4 Matrixs = Matrix * WorldMat.World ;
	gl_Position = Matrixs * vec4(position, 1.0f);
	mat4 Converted = TransMat.Transformation * WorldMat.World;
	ConvCoord = Converted * vec4(position, 1.0f);
}
