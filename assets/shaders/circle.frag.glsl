#version 450 core


layout (location=0) out vec4 o_Color;

struct VertexOutput
{
	vec3 Position;
	vec4 Color;
	float Thickness;
	float Fade;

};
layout (location=5) in VertexOutput Output;

void main()
{

    float distance = 1.0 - length(Output.Position);
    float circleAlpha = smoothstep(0.0, Output.Fade, distance);
    circleAlpha*= smoothstep(Output.Thickness + Output.Fade, Output.Thickness, distance);

    // Set output color
    o_Color = Output.Color*circleAlpha;
    



}