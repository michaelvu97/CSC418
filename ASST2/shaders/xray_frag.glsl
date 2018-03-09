precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in view space

void main() {
    
  // [0,1]
  float diffuseIntensity = dot(normalInterp, -normalize(vertPos));

  diffuseIntensity = 1.0 - (diffuseIntensity * diffuseIntensity);

  gl_FragColor = vec4(Kd * diffuseColor * diffuseIntensity, 1.0);
}
