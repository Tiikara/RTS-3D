uniform vec3 u_camera;

uniform float u_ambient;
uniform float u_k_diffuse;
uniform float u_k_specular;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec4 v_color;

varying vec2 texcoord;

void main(void)
{
    vec3 n_normal=normalize(gl_Normal);

    v_vertex = gl_Vertex;

    v_normal=n_normal;
    v_color=gl_Color;

    texcoord = vec2(gl_MultiTexCoord0);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
