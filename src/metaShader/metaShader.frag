#version 120

#define cMetaballNum 30
uniform vec3 metaPos[cMetaballNum];
uniform int metaFlag[cMetaballNum];
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
	for(int i = 0; i < cMetaballNum; i++)
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
	color.r = min(0.5f, result);
	color.g = 0.7f;
	color.b = 0.5f;
	gl_FragColor.rgba = vec4(color.rrr, 1.0 - min(1.0f, result));
	
	
    
}