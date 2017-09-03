
// 方向光源属性结构体
struct DirLightAttr
{
    vec3 direction; // 方向光源
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
{
	 // 环境光成分
    vec3    ambient = light.ambient * vec3(texture(material.diffuseMap, TextCoord));

    // 漫反射光成分 此时需要光线方向为指向光源
    vec3    lightDir = normalize(-light.direction); // 翻转方向光源的方向
    vec3    normal = normalize(FragNormal);
    float   diffFactor = max(dot(lightDir, normal), 0.0);
    vec3    diffuse = diffFactor * light.diffuse * vec3(texture(material.diffuseMap, TextCoord));

    // 镜面反射成分 此时需要光线方向为由光源指出
    float   specularStrength = 0.5f;
    vec3    reflectDir = normalize(reflect(-lightDir, normal));
    vec3    viewDir = normalize(viewPos - FragPos);
    float   specFactor = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3    specular = specFactor * light.specular * vec3(texture(material.specularMap, TextCoord));

    vec3    result = ambient + diffuse + specular;
}

struct PointLightAttr
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // 衰减常数
    float linear;   // 衰减一次系数
    float quadratic; // 衰减二次系数
};

struct SpotLightAttr
{
    vec3 position;  // 聚光灯的位置
    vec3 direction; // 聚光灯的spot direction
    float cutoff;   // 聚光灯张角的余弦值
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // 衰减常数
    float linear;   // 衰减一次系数
    float quadratic; // 衰减二次系数
};