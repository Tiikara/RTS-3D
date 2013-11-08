varying vec3 n;
varying vec3 l;
uniform float size;
varying float h;
uniform sampler3D tex;
varying vec2 planar_c;
uniform sampler1D noise_table;
varying vec3 v;

uniform float start_kf;
uniform float grow_pow;
uniform vec2 x_val;
uniform vec3 curve0;
uniform vec3 curve1;
uniform vec2 kb_line;

float corr_func(in float x)
{
    float g;
    x*=1.41421;
    if (x<x_val.x) return x*x*curve0[0]+x*curve0[1]+curve0[2];
    if (x<x_val.y) return x*kb_line.x+kb_line.y;
    return x*x*curve1[0]+x*curve1[1]+curve1[2];
    return x*kb_line.x+kb_line.y;
}

vec2 grad(in float x, in float y)
{
    float id1=texture1D(noise_table,x*0.00390625+0.001953125).r*0.99609375+0.001953125;
    float id2=texture1D(noise_table,y*0.00390625+0.001953125).g*0.99609375+0.001953125;
    vec2 r=texture1D(noise_table,id1+id2-0.001953125).ba;
    return (r-vec2(0.5))*2.0;
}

float Noise2D(in float x, in float y)
{
    float x0=floor(x);
    float y0=floor(y);
    float x1=x0+1.0;
    float y1=y0+1.0;
    float s,t,u,v;
    s=dot(grad(x0,y0),vec2(x-x0,y-y0));
    t=dot(grad(x1,y0),vec2(x-x1,y-y0));
    u=dot(grad(x0,y1),vec2(x-x0,y-y1));
    v=dot(grad(x1,y1),vec2(x-x1,y-y1));

    float d2=(x-x0);
    d2*=d2;
    float S=3.0*d2-2.0*d2*(x-x0);
    float a=s+S*(t-s);
    float b=u+S*(v-u);

    d2=y-y0;
    d2*=d2;
    S=3.0*d2-2.0*d2*(y-y0);
    return a+S*(b-a);
}

float NoiseSum(in int an, in float A, in float FR, in float x, in float y)
{
    float cur_a=A;
    float cur_fr=FR;
    float H=0.0;
    for (int i=0; i<an; i++)
    {
        H+=Noise2D(x*cur_fr,y*cur_fr)*cur_a;
        cur_fr*=2.0;
        cur_a*=0.5;
    }
    return H;
}
float Noise2D(in float x, in float y, out vec3 n)
{
    float x0=floor(x);
    float y0=floor(y);
    float x1=x0+1.0;
    float y1=y0+1.0;
    float s,t,u,v;
    vec2 g0,g1,g2,g3;
    g0=grad(x0,y0);
    g1=grad(x1,y0);
    g2=grad(x0,y1);
    g3=grad(x1,y1);
    s=dot(g0,vec2(x-x0,y-y0));
    t=dot(g1,vec2(x-x1,y-y0));
    u=dot(g2,vec2(x-x0,y-y1));
    v=dot(g3,vec2(x-x1,y-y1));

    float dx2=x-x0;
    dx2*=dx2;
    float Sx=3.0*dx2-2.0*dx2*(x-x0);
    float a=s+Sx*(t-s);
    float b=u+Sx*(v-u);

    float dy2=y-y0;
    dy2*=dy2;
    float Sy=3.0*dy2-2.0*dy2*(y-y0);

    float v0,v1,v2,v3,v4;
    v0=6.0*x-6.0*x0-6.0*dx2;
    v1=v0*(t-s);
    v2=Sx*(g1[0]-g0[0]);
    v3=6.0*y-6.0*y0-6.0*dy2;
    v4=Sx*(g1[1]-g0[1]);

    n[0]=-(g0[0]+v1+v2+Sy*(g2[0]+v0*(v-u)+Sx*(g3[0]-g2[0])-g0[0]-v1-v2));
    n[1]=-(g0[1]+v4+v3*(u+Sx*(v-u)-s-Sx*(t-s))+Sy*(g2[1]+Sx*(g3[1]-g2[1])-g0[1]-v4));
    n[2]=1.0;

    return a+Sy*(b-a);
}

float NoiseSum(in int an, in float A, in float FR, in float x, in float y, out vec3 norm)
{
    float pfk=0.707;
    float cur_a=A;
    float cur_fr=FR;
    float h=0.0;
    vec3 n;
    float cur_hscale=1.0;
    float step_max_h=A;
    norm=vec3(0.0,0.0,1.0);
    for (int i=0; i<an; i++)
    {
        cur_a*=cur_hscale;
        h+=corr_func(Noise2D(x*cur_fr,y*cur_fr,n))*cur_a;
        cur_hscale=pow(1.0-(step_max_h-abs(h))/(start_kf*step_max_h),grow_pow);
        norm.x+=n.x*cur_a*cur_fr;
        norm.y+=n.y*cur_a*cur_fr;
        cur_fr*=2.0;
        cur_a*=0.5;
    }
    norm=normalize(norm);
    return h;
}

void main(void)
{
    float H=h+NoiseSum(5, size, 0.2, planar_c.y, planar_c.x);

    vec3 N=normalize(n);
    vec3 V=normalize(v);
    NoiseSum(10,size,0.5,planar_c.x,planar_c.y,N);
    vec3 L=normalize(l);

    float lk=max(dot(N,L),0.0);

    vec3 R=-reflect(L,N);
    float spec_k=pow(max(dot(R,V),0.0),8.0);

    float b1=0.4*2.0*size-size;
    float b2=0.6*2.0*size-size;

    vec2 tex_c=vec2(planar_c.x/0.75,planar_c.y/0.5);

    vec4 c1=texture3D(tex,vec3(tex_c.x,tex_c.y,0.16666));
    vec4 c2=texture3D(tex,vec3(tex_c.x,tex_c.y,0.5));
    vec4 c3=texture3D(tex,vec3(tex_c.x,tex_c.y,0.8333));

    float dint=0.1*size;
    float t1=b1-dint;
    float t2=b1+dint;
    float t3=b2-dint;
    float t4=b2+dint;

    vec4 col;

    if (H<t3) col=smoothstep(t1,t2,H)*c2+(1.0-smoothstep(t1,t2,H))*c1;
    else col=smoothstep(t3,t4,H)*c3+(1.0-smoothstep(t3,t4,H))*c2;

    vec4 fcol=col*lk+spec_k*vec4(0.3);
    gl_FragColor = vec4(fcol.rgb,1.0);
}
