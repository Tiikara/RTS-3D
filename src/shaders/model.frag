
uniform sampler2D text;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec2 texcoord;
varying vec3 v_lightPosition;

vec3 cameravector = vec3(0.0,0.0,0.0);

void main(void)
{
    vec3 n_normal=normalize(v_normal);

    vec3 lightvector = normalize(v_lightPosition - v_vertex);

    vec3 lookvector = normalize(cameravector - v_vertex);

    float diffuse = min( max(dot(n_normal, lightvector), 0.0) + 0.5,1.0 );

    vec3 reflectvector = reflect(-lightvector, n_normal);

    float specular = texture2D(text,texcoord).a * pow( max(dot(lookvector,reflectvector),0.0), 8.0 );

    vec4 specColor = vec4(1.0,1.0,1.0,1.0);

    gl_FragColor = specular*specColor+vec4(texture2D(text,texcoord).rgb*diffuse, 1.0);

}
