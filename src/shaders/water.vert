varying vec3 pos;

void main(void)
{
    vec4 vpos=gl_Vertex;
    gl_Position = gl_ModelViewProjectionMatrix * vpos;

    pos=gl_Vertex.xyz;
}
