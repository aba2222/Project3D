struct Light
{
    float3 lightPos;
    float padding0; // 填充
    float3 ambient;
    float padding1; // 填充
    float3 diffuseColor;
    float padding2; // 填充
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
    int lightType;
    float padding3[3]; // 填充
};

cbuffer LightCBuf : register(b0)
{
    Light lights[1];
    //int numLights;
};


cbuffer ObjectCBuf : register(b1)
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};

float3 ComputeLight(float3 lightDir, float att, float3 n, Light light, float3 worldPos)
{
    // 漫反射
    float3 diffuse = light.diffuseColor * light.diffuseIntensity * att * saturate(dot(lightDir, n));
    
    // 高光反射
    float3 reflectDir = reflect(-lightDir, n);
    float3 viewDir = normalize(-worldPos);
    float specAngle = saturate(dot(viewDir, reflectDir));
    float spec = pow(specAngle, specularPower);
    
    float viewFactor = saturate(dot(n, viewDir));
    float3 specular = specularIntensity * spec * light.diffuseColor * att * viewFactor;
    
    return diffuse + specular + light.ambient;
}


float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    float3 finalColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 1; i++)
    {
        float3 lightDir;
        float att = 1.0f;

        if (lights[i].lightType == 0) // 点光源
        {
            float3 vToL = lights[i].lightPos - worldPos;
            float distToL = length(vToL);
            lightDir = vToL / distToL;
            att = 1.0f / (lights[i].attConst + lights[i].attLin * distToL + lights[i].attQuad * (distToL * distToL));
        }
        else if (lights[i].lightType == 1) // 定向光
        {
            lightDir = normalize(lights[i].lightPos);
        }
        
        finalColor += ComputeLight(lightDir, 1.0f, n, lights[i], worldPos);
    }
    
    return float4(saturate(finalColor * materialColor), 1.0f);
}