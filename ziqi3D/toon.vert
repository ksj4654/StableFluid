#version 430

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 uMat;
uniform vec4 uColor;

void main()
{
	
	gl_Position  = uMat*vPosition;
	gl_Position *= vec4(1,1,-1,1);
   
	if(uColor.w<1)
		color = vColor;
	else 
		color = uColor;
}
