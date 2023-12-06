#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[5];

void main()
{
	int index=int(v_TexIndex);
	if(index==5)
	{
	o_Color=v_Color;	
	}
	else
	{
	o_Color =texture(u_Textures[index],v_TexCoord);
	}

}