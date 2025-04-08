#version 330

out vec4 FragColor;
//out vec4 FragColor1;

in vec4 outcol;
//in vec3 color;

void main()
{
	//gl_FragColor1  = vec4(color.x, color.y,color.z, color.w);
	gl_FragColor  = vec4(outcol.x, outcol.y,outcol.z, outcol.w);
	
} 
