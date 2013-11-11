uniform vec3 u_camera;

uniform float u_k_diffuse;
uniform float u_k_specular;

uniform sampler2D text;

vec3 lightPosition = vec3(0.0,0.0,10.0);

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec4 v_color;
varying vec2 texcoord;

void main(void)
{
    vec3 n_normal=normalize(v_normal);

    vec3 lightvector = normalize(lightPosition - v_vertex);

    vec3 lookvector = normalize(u_camera - v_vertex);

    float diffuse = u_k_diffuse * max(dot(n_normal, lightvector), 0.0);

    vec3 reflectvector = reflect(-lightvector, n_normal);

    float specular = u_k_specular * pow( max(dot(lookvector,reflectvector),0.0), 8.0 );

    vec4 diffColor = vec4(1.0,1.0,1.0,1.0);
    vec4 specColor = vec4(1.0,1.0,1.0,1.0);

    gl_FragColor = diffuse*diffColor+specular*specColor+texture2D(text,texcoord);

}
