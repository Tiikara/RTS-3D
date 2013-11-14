uniform float u_interp;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec4 v_color;
varying vec2 texcoord;

attribute vec3 next_v;
attribute vec3 next_n;

void main(void)
{
    vec3 interp_normal = gl_Normal + (next_n-gl_Normal) * u_interp;

    vec3 n_normal=normalize(gl_NormalMatrix * interp_normal);

    vec4 interp_vertex = gl_Vertex + (vec4(next_v,1.0)-gl_Vertex) * u_interp;

    v_vertex = gl_ModelViewMatrix * interp_vertex;

    v_normal=n_normal;
    v_color=gl_Color;

    texcoord = vec2(gl_MultiTexCoord0);

    gl_Position = gl_ModelViewProjectionMatrix * interp_vertex;
}
