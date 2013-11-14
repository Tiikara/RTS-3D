
uniform float size;
uniform sampler3D tex;
uniform sampler1D noise_table;
uniform sampler1D tex1Borders;

varying vec3 n;
varying vec3 l;
varying float h;
varying vec2 planar_c;
varying vec3 v;

uniform float flayerC;
uniform int layerC;

void main(void)
{
    float H=h;

    vec3 N=normalize(n);
    vec3 V=normalize(v);

    vec3 L=normalize(l);

    float t[10];
    vec4 c[10];
    int ln1;
    int ln2;
    int ln3;
    float mingr2;
    float maxgr2;
    float mingr1;
    float maxgr1;
    float lk=max(dot(N,L),0.0);
    vec3 R=-reflect(L,N);
    float spec_k=pow(max(dot(R,V),0.0),8.0);
    vec2 tex_c=vec2(planar_c.x/0.75,planar_c.y/0.5);
    float dint=0.2*size;
    vec4 col;

    float lm=1.0/flayerC;
    for(int j=0;j<layerC;j++)
    {
        float corz=((j*lm)+(0.5*lm));
        c[j]=texture3D(tex,vec3(tex_c.x,tex_c.y,corz));
    }

    for(int i=1;i<layerC;i++)
        t[i]=texture1D(tex1Borders,float(i-1)/(flayerC-1.0)).r;
    t[0] = -10.0;
    t[layerC] = 10.0;

    int k=0;
    bool g=false;
    while(!g)
    {
        if(H>t[k+1])
        {
           k++;
        }
        else
        {
           ln1=k;
           g=true;
        }
    }
    ln2=ln1+1;
    ln3=ln1-1;
    float cr=(t[ln1]+t[ln2])*0.5;

    if(H>cr)
    {
        col=smoothstep(t[ln2]-dint,t[ln2]+dint,H)*c[ln2]+(1.0-smoothstep(t[ln2]-dint,t[ln2]+dint,H))*c[ln1];
    }
    else
    {
        col=smoothstep(t[ln1]-dint,t[ln1]+dint,H)*c[ln1]+(1.0-smoothstep(t[ln1]-dint,t[ln1]+dint,H))*c[ln3];
    }

    vec4 fcol=col*lk+spec_k*vec4(0.3);
    gl_FragColor = vec4(fcol.rgb,1.0);
}
