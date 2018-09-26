#version 120

uniform vec3 metaPos[10];
uniform vec4 metaColor[10];
varying vec3 pos;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	float result = 0.0f;
	float valList[10];
	for(int i = 0; i < 10; i++)
	{
		float distX = pos.x - metaPos[i].x;
		float distY = pos.y - metaPos[i].y;
		float r = (metaPos[i].z * metaPos[i].z);
		float dist = (r) / ((distX * distX) + (distY * distY));
		valList[i] = dist;
		result += dist;
	}
	
	/*
	vec4 color;
	for(int i = 0; i < 10; i++)
	{
		color.r += metaColor[i].r * (valList[i] / result);
		color.g += metaColor[i].g * (valList[i] / result);
		color.b += metaColor[i].b * (valList[i] / result);
	}
	gl_FragColor.rgba = vec4(color.rgb, 0.8f);
	*/
	
	vec3 color;
	color.r = result;
	color.g = 1.0f;
	color.b = 1.0f;
	gl_FragColor.rgba = vec4(hsv2rgb(color).rgb, 0.7f);
	
	
    
}