#version 330

in vec2 uv;
in vec3 Position;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 EyeDirection;
struct Material
{
	float shiness;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
    vec3 specular;
};
//uniform Material material;
struct PointLight
{
	vec3 color;
    vec3 position;
    float ambientIntensity;
    float diffuseIntensity;   

	float shiness;
	float strength;
    float constant; // 衰减常数
    float linear;   // 衰减一次系数
    float quadratic; // 衰减二次系数
};
uniform PointLight pointLight;
struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
};
vec4 CalcPointLight(PointLight pointLight,vec3 position, vec3 normal)
{
    vec3 lightDirection = position - pointLight.position;
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);
	float attenuation = 1.0/(pointLight.constant + pointLight.linear * lightDistance + pointLight.quadratic * lightDistance * lightDistance);
	
	vec4 ambientColor = vec4(pointLight.color, 1.0f) * pointLight.ambientIntensity;
	
	vec3 halfVec = normalize(lightDirection + EyeDirection);
	float diffuse = max(0.0,dot(normal, -lightDirection));
	float specular = max(0.0,dot(normal, halfVec));	
	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular,pointLight.shiness) * pointLight.strength;		
	vec4 diffuseColor = vec4(pointLight.color * pointLight.diffuseIntensity * diffuse * attenuation, 1.0f);
	vec4 specularColor = vec4(pointLight.color * specular * attenuation, 1.0f);
	return vec4(ambientColor + diffuseColor + specularColor);//(diffuseColor);
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 position, vec3 normal)
{
    vec3 lightToPos = normalize(position - spotLight.pointLight.position);
    float SpotFactor = dot(lightToPos, spotLight.direction);

    if (SpotFactor > spotLight.cutoff) {
		
        vec4 Color = CalcPointLight(spotLight.pointLight, position, normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - spotLight.cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
} 
void main()
{	
	vec4 lightColor = CalcPointLight(pointLight, Position, Normal);
	vec4 Color = texture(texture1, uv);	
	gl_FragColor = Color * lightColor;
}