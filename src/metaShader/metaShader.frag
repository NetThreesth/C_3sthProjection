#version 120

uniform vec3 metaPos[10];
varying vec3 pos;

void main()
{
	float result = 0.0f;
	for(int i = 0; i < 10; i++)
	{
		float distX = pos.x - metaPos[i].x;
		float distY = pos.y - metaPos[i].y;
		float r = (metaPos[i].z * metaPos[i].z) * 3;
		result += r / ((distX * distX) + (distY * distY));
	}
	
	gl_FragColor.rgba = vec4(result, result, result, 1.0);
    
}