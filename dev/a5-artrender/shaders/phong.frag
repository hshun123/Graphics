#version 330

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;


void main() {
    
  vec3 l = normalize(light_in_eye_space - position_in_eye_space);
 
  vec3 n = normalize(normal_in_eye_space);

  vec3 e = normalize(-position_in_eye_space);

  // halfway vector
  vec3 h = normalize(e + l);

  // ambient, diffuse, specular
  vec4 ambient = ka * Ia;
  vec4 diffuse = kd * Id * max(0, dot(n, l)); // dot(n, 1) could be negative
  vec4 specular = ks * Is * pow(max(dot(h, n), 0), s);

  // equation
  color = ambient + diffuse + specular;


   // color = vec4(0,0,0,1);
}
