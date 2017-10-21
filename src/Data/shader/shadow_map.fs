#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 Uv;
in vec4 lightSpacePos;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float isInShadow(vec4 lightspacePos)
{
	vec3 projectCoords = lightspacePos.xyz / lightspacePos.w;
	projectCoords *= 0.5 +0.5;
	
	float nearDepth = texture(shadowMap, projectCoords.xy).r;
	float currentDepth = projectCoords.z;
	
	return currentDepth > nearDepth ? 1.0 : 0.0; //1.0 means in the shadow
}

void main()
{
	vec3 color = texture(shadowMap, Uv).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = isInShadow(lightSpacePos);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    gl_FragColor = vec4(lighting, 1.0);
}