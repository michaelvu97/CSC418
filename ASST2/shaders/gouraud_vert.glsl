attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex
varying vec4 color;

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position in camera space


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.

  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;

  vertPos = vec3(vertPos4);

  // Normal in camera space
  normalInterp = normalize(vec3(normalMat * vec4(normal, 1.0)));

  // Calculate diffuse for this vertex
  float diffuseIntensity = dot(normalize(lightPos - vertPos), normalInterp);

  vec3 mirrorRay = normalize(reflect(vertPos - lightPos, normalInterp));

  float specularIntensity = diffuseIntensity * 
        pow(dot(normalize(vertPos),-mirrorRay), shininessVal);

  color = vec4(ambientColor + (diffuseIntensity * Kd * diffuseColor) + 
        (specularIntensity * Ks * specularColor), 1.0); 
}