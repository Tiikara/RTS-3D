uniform float amplit;
uniform float freq;
uniform sampler1D noise_table;
uniform vec3 view_pos;

varying vec3 n;
varying vec3 l;
varying float h;
varying vec3 v;
varying vec2 planar_c;

uniform float start_kf;
uniform float grow_pow;
uniform vec2 x_val;
uniform vec3 curve0;
uniform vec3 curve1;
uniform vec2 kb_line;

vec4 lpos=vec4(0.0,0.0,10.0,1.0);

float corr_func(in float x)
{
    x*=1.41421;
    if (x<x_val.x) return x*x*curve0[0]+x*curve0[1]+curve0[2];
    if (x<x_val.y) return x*kb_line.x+kb_line.y;
    return x*x*curve1[0]+x*curve1[1]+curve1[2];
}

vec2 grad(in float x, in float y)
{
    float id1=texture1D(noise_table,x*0.00390625+0.001953125).r*0.99609375+0.001953125;
    float id2=texture1D(noise_table,y*0.00390625+0.001953125).g*0.99609375+0.001953125;
    vec2 r=texture1D(noise_table,id1+id2-0.001953125).ba;
    return (r-vec2(0.5))*2.0;
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
    vec4 vpos=gl_Vertex + vec4(view_pos.xy, 0.0, 0.0);
    vpos.z=NoiseSum(10,amplit,freq,vpos.x,vpos.y,n);

    h=vpos.z;
    gl_Position = gl_ModelViewProjectionMatrix * vpos;

    l=lpos.xyz-vpos.xyz;
    planar_c.x=vpos.x;
    planar_c.y=vpos.y;

    v=normalize(view_pos-vec3(vpos));

}
