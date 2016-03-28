#version 410

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColor;

uniform vec3 LightDir;
uniform sampler2D diffuse;
uniform sampler2D normal; 

void main() 
{ 
	mat3 TBN = mat3( 
		normalize( vTangent ), 
		normalize( vBiTangent ), 
		normalize( vNormal ));

	vec3 N = texture(normal, vTexCoord).xyz * 2 - 1; 

	float d = max( 0, dot( normalize( TBN * N ), normalize( LightDir ))); 

	FragColor = texture(diffuse, vTexCoord); 
	FragColor.rgb = FragColor.rgb * d;
}