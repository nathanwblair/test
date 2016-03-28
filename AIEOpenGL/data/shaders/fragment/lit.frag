#version 330

in vec4 vPosition;
in vec4 vNormal;
in vec4 vTangent;
in vec4 vBinormal;

in vec2 vTexCoords;

out vec4 fragColour;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D ambientTexture;

uniform vec3 LightDir = vec3(1, 0, 0);
uniform vec3 CameraPos;
uniform float SpecPower = 16;
uniform vec3 LightColor = vec3(0.2, 0.2, 0.2);

void main()
{
    mat3 TBN = mat3(
        normalize(vTangent.xyz),
        normalize(vBinormal.xyz),
        normalize(vNormal.xyz)
    );
    
    vec3 n_ts = texture(normalTexture, vTexCoords).xyz * 2 - 1;
    vec3 n_ws = normalize(TBN * n_ts);
    
    
    float d = max(0, dot(n_ws, LightDir.xyz));
    
    vec3 E = normalize(CameraPos - vPosition.xyz);
    vec3 R = reflect(-LightDir.xyz, n_ws.xyz);
    
    float s = d * max(0, dot(E, R));
    s = pow(s, SpecPower);
    
    vec3 texColour = texture(diffuseTexture, vTexCoords).xyz;
    vec3 ambient = LightColor * texture(ambientTexture, vTexCoords).xyz * texColour;
    
    vec3 diffuse = d * texColour;
    vec3 spec = texture(specularTexture, vTexCoords).xyz * s;
    
	fragColour = vec4(ambient + diffuse + spec, 1);
}