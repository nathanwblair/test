#version 410

in vec4 vNormal; 
in vec4 vPosition;
out vec4 FragColor; 

uniform vec3 LightDir; 
uniform vec3 LightColor; 
uniform vec3 CameraPos; 
uniform float SpecPow; 

void main() 
{ 
	float d = max(0, dot(normalize(vNormal.xyz), LightDir) ); 
	vec3 E = normalize( CameraPos - vPosition.xyz );

	vec3 R = reflect( -LightDir, vNormal.xyz ); 
	float s = max( 0, dot( E, R ) ); 

	s = pow( s, SpecPow ); 

	FragColor = vec4( LightColor * d + LightColor * s, 1);
}