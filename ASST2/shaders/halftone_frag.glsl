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

uniform vec3 lightPos; // Light position in camera space

// HINT: Use the built-in variable gl_FragCoord to get the screen-space coordinates

float round(float x) {
  if (x > 0.0)
    return floor(x + 0.5);
  else
    return floor(x - 0.5);

}

float segment(float position, float divisor) {
  return round(position / divisor) * divisor;
}

void main() {
  // Your solution should go here.
  // Only the background color calculations have been provided as an example.

  float intensity = Kd * 
      max(0.0, dot(normalInterp, normalize(lightPos - vertPos)));

  // The screen will be divided into NxN circles
  float dist = 10.0;

  // Find the nearest centroid
  vec4 centre = gl_FragCoord;
  centre.x = segment(centre.x, dist);
  centre.y = segment(centre.y, dist);

  float radiusGrowthMagicNumber = 0.5;

  float cutoff = radiusGrowthMagicNumber * dist * dist * 
      (1.0 - intensity) * (1.0 - intensity);

  if (dot(gl_FragCoord - centre, gl_FragCoord - centre) <= cutoff) {

    gl_FragColor = vec4(ambientColor, 1.0);

  } else {
    gl_FragColor = vec4(diffuseColor, 1.0);  
  }

  
}