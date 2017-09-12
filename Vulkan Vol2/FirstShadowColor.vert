#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 in_color;

layout (binding = 0) uniform CamMatrixes{
	mat4 View;
	mat4 Projection;
}ViewProjMat;

layout (binding = 1) uniform ModelMatrixes{
	mat4 World;
}WorldMat;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	mat4 Matrix = ViewProjMat.Projection  * ViewProjMat.View * WorldMat.World;
	gl_Position = Matrix * vec4(position, 1.0f);
}