uniform vec3 view_pos;
uniform sampler1D noise_table;
uniform sampler2D sky_map;

varying vec3 pos;

uniform vec2 l0_dv;
uniform vec2 l1_dv;
uniform vec2 l2_dv;


vec4 lpos=vec4(0.0,0.0,10.0,1.0);


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

float NoiseSum(in float A, in float FR, in float x, in float y, out vec3 norm)
{
    float cur_a=A;
    float cur_fr=FR;
    float h=0.0;
    vec3 n;

    norm=vec3(0.0,0.0,1.0);
    h=Noise2D(x*cur_fr+l0_dv.x,y*cur_fr+l0_dv.y,n);
    norm.x+=n.x*cur_a*cur_fr;
    norm.y+=n.y*cur_a*cur_fr;
    cur_fr*=2.0;//1.333;
    cur_a*=0.5;//0.75;

    h+=Noise2D(x*cur_fr+l1_dv.x,y*cur_fr+l1_dv.y,n);
    norm.x+=n.x*cur_a*cur_fr;
    norm.y+=n.y*cur_a*cur_fr;
    cur_fr*=2.0;//1.333;
    cur_a*=0.5;//0.75;

    h+=Noise2D(x*cur_fr+l2_dv.x,y*cur_fr+l2_dv.y,n);
    norm.x+=n.x*cur_a*cur_fr;
    norm.y+=n.y*cur_a*cur_fr;

    norm=normalize(norm);
    return h;
}


void main(void)
{
    vec3 N;
    vec3 p=pos;
    p.z+=NoiseSum(0.01,20.0,pos.x,pos.y,N);

    vec3 L=normalize(lpos.xyz-pos);
    vec3 V=normalize(view_pos-pos);
    float dkf=max(0.0,dot(N,L));

    vec3 R=-reflect(L,N);
    float skf=max(0.0,pow(dot(R,V),8.0));


    vec4 sh_color=vec4(1.0,1.0,1.0,1.0);
    vec3 ref_color=texture2D(sky_map,vec2((N.x+1.0)*0.5,(N.y+1.0)*0.5)).xyz;

    float fr_k=dot(V,N);
    float alpha=1.0-fr_k;

    vec3 col=dkf*ref_color;

    gl_FragColor = vec4(col.x,col.y,col.z,alpha)+sh_color*skf;
}
