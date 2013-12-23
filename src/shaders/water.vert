uniform vec3 view_pos;
varying vec3 pos;

void main(void)
{
    vec4 vpos=gl_Vertex + vec4(view_pos.xy, 0.0, 0.0);
    gl_Position = gl_ModelViewProjectionMatrix * vpos;

    pos=vpos.xyz;
}
