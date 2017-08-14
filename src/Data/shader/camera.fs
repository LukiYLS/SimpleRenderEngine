#version 330

in vec2 uv;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 color1 = texture(texture1,uv);
	vec4 color2 = texture(texture2,uv);
	//vec3 color = mix(color1.rgb,color2.rgb,0.5);
	gl_FragColor = vec4(color1.rgb,1.0);
}