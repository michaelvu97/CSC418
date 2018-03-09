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

float step_threshold(float intensity, float threshold) {

    if (intensity < threshold)
        return 0.0;
    else
        return 1.0;

}

void main() {

  vec3 lightPosNormalized = normalize(lightPos);
  vec3 testLightPosNorm = normalize(lightPos - vertPos);

  // First light level
  float diffuseIntensity1 = max(0.0, dot(normalInterp, testLightPosNorm));
  diffuseIntensity1 = step_threshold(diffuseIntensity1, 0.6);

  // Second light level
  float diffuseIntensity2 = max(0.0, dot(normalInterp, testLightPosNorm));
  diffuseIntensity2 = step_threshold(diffuseIntensity2, 0.3);

  // Reflect the light ray
  vec3 mirrorRay = normalize(reflect(vertPos - lightPos, normalInterp));

  // Specular light level
  float specularIntensity = clamp(0.0, 
        pow(max(0.0, dot(normalize(vertPos),-mirrorRay)), shininessVal), 1.0);
  specularIntensity = step_threshold(specularIntensity, shininessVal / 128.0);

  gl_FragColor = vec4(
        (ambientColor * Ka) + 
        (Kd * diffuseColor * diffuseIntensity1 * 2.0 / 3.0) + 
        (Kd * diffuseColor * diffuseIntensity2 / 3.0) + 
        (Ks * specularIntensity * specularColor),
      1.0);
}
