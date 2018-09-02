#version 120

varying vec3 pos;

void main()
{
    pos = gl_MultiTexCoord0.xyz;
	gl_Position = ftransform();
}