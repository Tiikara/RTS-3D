uniform float u_interp;
uniform vec3 u_camera;
uniform mat4 u_modelViewMatrix;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec4 v_color;
varying vec3 v_camera;
varying vec2 texcoord;
varying vec3 v_lightPosition;

attribute vec3 next_v;
attribute vec3 next_n;

void main(void)
{
    v_lightPosition = vec3(gl_ModelViewMatrix * vec4(1.0,0.0,1.0,1.0));
    v_camera = vec3(gl_ModelViewMatrix * vec4(u_camera,1.0));

    vec3 interp_normal = gl_Normal + (next_n-gl_Normal) * u_interp;

    vec3 n_normal=normalize(gl_NormalMatrix * mat3(u_modelViewMatrix) * interp_normal);

    vec4 interp_vertex = gl_Vertex + (vec4(next_v,1.0)-gl_Vertex) * u_interp;

    v_vertex = vec3(gl_ModelViewMatrix * u_modelViewMatrix * interp_vertex);

    v_normal=n_normal;
    v_color=gl_Color;

    texcoord = vec2(gl_MultiTexCoord0);

    gl_Position = gl_ModelViewProjectionMatrix * u_modelViewMatrix * interp_vertex;
}
