uniform mat4 projection, modelview, normalMat;
uniform int mode;

varying vec4 forFragColor;
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main(){
 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 vec3 lightPos = vec3(0,0,0);

 // all following geometric computations are performed in the
 // camera coordinate system (eye coordinates)
 vec3 normal = vec3(gl_NormalMatrix * gl_Normal);
 vec4 vertPos4 = gl_ModelViewMatrix * gl_Vertex;
 vec3 vertPos = vec3(vertPos4);
 vec3 lightDir = normalize(lightPos - vertPos);
  
 float spec_intensity = pow(max(dot(lightDir, normal), 0.0), 1.0);
 float lambertian = max(dot(lightDir,normal), 0.0);
 forFragColor = vec4(lambertian*diffuseColor + spec_intensity*specColor , 100.0);
}
