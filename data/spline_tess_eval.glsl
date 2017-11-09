#version 410 core

layout (isolines) in;

patch in vec4 p_1;
patch in vec4 p2;

void main()
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	float b0 = (-1.f * u) + (2.f * u * u) + (-1.f * u * u * u);
	float b1 = (2.f) + (-5.f * u * u) + (3.f * u * u * u);
	float b2 = (u) + (4.f * u * u) + (-3.f * u * u * u);
	float b3 = (-1.f * u * u) + (u * u * u);
	vec4 new_pos = 0.5f * (b0*p_1 + b1*p0 + b2*p1 + b3*p2);
	gl_Position = vec4(new_pos.x + v * 0.08, new_pos.y + v * 0.08, new_pos.z, new_pos.w);
}
