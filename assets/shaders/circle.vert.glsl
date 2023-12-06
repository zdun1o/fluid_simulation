#version 450 core

layout (location=0) in vec3 a_Position;
layout (location=1) in vec4 a_Color;
layout (location=2) in float a_Thickness;
layout (location=3) in float a_Fade;


layout (location=4) uniform mat4 u_ViewProjection;


struct VertexOutput
{
	vec3 Position;
	vec4 Color;
	float Thickness;
	float Fade;

};
layout (location=5) out VertexOutput Output;
void main()
{
	Output.Position=a_Position;
	Output.Color=a_Color;
	Output.Thickness=a_Thickness;
	Output.Fade=a_Fade;

	gl_Position=u_ViewProjection*vec4(a_Position,1.0);

}