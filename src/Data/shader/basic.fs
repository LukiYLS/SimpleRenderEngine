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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float shiness;
	float strength;
    float constant; // 衰减常数
    float linear;   // 衰减一次系数
    float quadratic; // 衰减二次系数
};
uniform PointLight pointLight;


void main()
{
	vec3 lightDirection = pointLight.position - Position;
	float lightDistance = length(lightDirection);
	lightDirection /= lightDistance;
	float attenuation = 1.0/(pointLight.constant + pointLight.linear * lightDistance + pointLight.quadratic * lightDistance * lightDistance);
	
	vec3 halfVec = normalize(lightDirection + EyeDirection);
	float diffuse = max(0.0,dot(Normal, lightDirection));
	float specular = max(0.0,dot(Normal, halfVec));	
	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular,pointLight.shiness) * pointLight.strength;
		
	
	
	vec3 scatteredLight = pointLight.ambient + pointLight.color * diffuse * attenuation;
	vec3 reflectedLight = pointLight.color * specular * attenuation;
	
	vec4 Color = texture(texture1, uv);
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	gl_FragColor = vec4(rgb, Color.a);
}