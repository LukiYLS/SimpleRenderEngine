
#version 330
uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;

in vec4 vertexColor;
in vec4 vertexPosition;
in vec3 vertexNormal;



out vec4 Color;
out vec3 Normal
out vec4 Position;

void main()
{
	Color = vertexColor;
	Normal = normalize(NormalMatrix * vertexNormal);	
	Position = MVMatrix * vertexPosition;
	gl_Position = MVPMatrix * vertexPosition;
}


/////////////////////////////////////////////////

#version 330
	
uniform	vec3 Ambient;
uniform	vec3 LightColor;
uniform	vec3 LightPosition;
uniform	float Shiness;
uniform	float Strength;
	
uniform	vec3 EyeDirection;
uniform	float ConstantAttenuation;
uniform	float LiearAttenuation;
uniform	float QuadricAttenuation;

in vec4 Color;
in vec4 Position;
in vec3 Normal;

void main()
{
	vec3 lightDirection(Position - LightPosition);
	float lightDistance = length(lightDirection);
	lightDirection /= lightDistance;
	float attenuation = 1.0/(ConstantAttenuation + LiearAttenuation * lightDistance + QuadricAttenuation * lightDistance * lightDistance);
	
	vec3 halfVec = normalize(lightDirection + EyeDirection);
	float diffuse = max(0.0,dot(Normal, lightDirection));
	float specular = max(0.0,dot(Normal, halfVec);
	
	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular,Shiness) * Strength;
	
	vec3 scatteredLight = Ambient + Color * diffuse * attenuation;
	vec3 reflectedLight = LightColor * specular * attenuation;
	
	vec3 rgb = min(Color.rgb * scatteredLight * reflectedLight, vec3(1.0));
	gl_FragColor = vec4(rgb,Color.a);
	
}
