#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 in_color;


layout (binding = 0) uniform Matrixes{
	mat4 World;
	mat4 View;
	mat4 Projection;
}matrixes;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

layout(location = 0) out vec4 color;

void main()
{
	mat4 Matrix = matrixes.Projection  * matrixes.View * matrixes.World ;
	gl_Position = Matrix * vec4(position, 1.0f);
	color = in_color;
}