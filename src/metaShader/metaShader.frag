#version 120

uniform vec3 metaPos[10];
uniform int metaFlag[10];
uniform int metaWidth;
uniform int metaHeight;
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
	for(int i = 0; i < 10; i++)
	{
		if(metaFlag[i] > 0)
		{
			float distX = abs(pos.x - metaPos[i].x);
			float distY = abs(pos.y - metaPos[i].y);
			distX = min(distX, metaWidth - distX);
			distY = min(distY, metaHeight - distY);
			float r = (metaPos[i].z * metaPos[i].z);
			float dist = (r * 0.5f) / ((distX * distX) + (distY * distY));
			result += dist;
		}
	}

	
	
	vec3 color;
	color.r = min(1.0f, result);
	color.g = 1.0f;
	color.b = 1.0f;
	gl_FragColor.rgba = vec4(hsv2rgb(color).rgb, 1.0 - min(1.0f, result));
	
	
    
}