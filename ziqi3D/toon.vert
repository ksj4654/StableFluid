#version 430

in  vec3 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 uMat;
uniform vec4 uColor;

void main()
{
	
	gl_Position  = uMat*vec4(vPosition,1.0f);
	gl_Position *= vec4(1,1,-1,1);
   
	if(uColor.w<1)
		color = vColor;
	else 
		color = uColor;
}
