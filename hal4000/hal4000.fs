// HAL 4 kb intro by the SandS

//precision highp float;

//#define t (mod(iTime,108.))

//uniform int v;
//float t=v*.001;

uniform vec3 V;
float t=V.z*.001;

float torus(vec3 o, float k)
{
   o.z -= 4.5; // pivot point
   vec3 r = vec3(t/2.3, t/1.95, t/2.7), s = sin(r), c = cos(r); // rotate over axis x,y,z
   o *= mat3( // rotation
        c.y,     s.z*s.y,            -s.y*c.z,
       -s.x*s.y, c.x*c.z+s.x*s.z*c.y, s.z*c.x-s.x*c.z*c.y,
        c.x*s.y, s.x*c.z-c.x*s.z*c.y, s.z*s.x+c.x*c.z*c.y
   );
   float s1=clamp((t-33.)/14.,0.,1.); // morph to sphere on 0:33
   k = mix(k,0.,s1*s1*s1);
   float n = 8.-6.*k, l = 3.8*k-2.;
   n = mix(n,2.,s1*s1*s1);
   o = abs(o);
   return pow(pow(o.x,n)+pow(o.y,n)+pow(o.z,n)+l,2.)-k*8.*(o.x*o.x + o.y*o.y);
}

float shape(vec3 o)
{
    return sin(o.x*5.)/15. + cos(o.x+1.5)/3. + sin(o.z+t)/5. - cos(o.x+2.)*cos(o.z+t)/3.;
}

float rt(vec3 o)
{
    return length(o-vec3(o.x,shape(o) - 1.5,o.z));
}

float s(float a, float z) // move star field
{
    return ( a-a*z+1. ) * 991.;
}

float wire(float dist) // convert distance to color
{
    return pow(1. - min(1.,abs(dist)),4.);
}

void main()
{    
	vec2 coordVar = gl_FragCoord.xy/V.xy;
    //coordVar /= iResolution.xy;
    //float aspect = 16. / 9.;
	float aspect = V.x/V.y;
    vec2 uv = (coordVar - vec2(.5)) / vec2(1., aspect) *1.4;

    float cf=0., ct=0., cm=0., cl=0., cs = 0., pi = 3.1416; // init colors: field, torus, torus, light, stars
    float l = 1., d, e = .0001, m1, m2, r = 0., size, n, z; // ray length, current distance, epsilon
    vec3 xyz, c = vec3(0.,0.,0.), norm = c, light = vec3(0.,1.,12.+max(0.,(t-47.)/8.)); // 3d-coordinates, final color, normal, light position

if (t<62.) // Scene 1 from 0:00 to 1:02
{
// morphing
    float kf = clamp(t/4.-5.,0.,1.); // plane -> landscape on 0:20
    float kt = cos((t)/3.1)/2.5+.4; // cube -> toruns, from 0 to 1
    float s1 = t - 22., td = 1.+max(s1*s1*s1/999.,0.)/9.; // torus distance
    n = min(t*t*t/451.,17.7)+.3; // number of torus slices
    n = mix(n,3.,clamp((t-42.)/5.,0.,1.));

// draw field
    z=30.-fract(t);
    for (int i=30; i>0; i--) // z-slices from far to near
    {
        xyz = vec3(uv*(1.+z),z); // form 3D coordinates from screen coordinates and z
        cf = max(cf, pow(wire( -xyz.y-1.5 + kf*shape(xyz+vec3(0.,0.,1.)) ),7.)*-uv.y*2.6); // draw field slice
        z--;
    };

// draw torus
// using SandS' super original proprietary rendering technic, let's call it "Z-sliced space" :)
    z = 4.5 + 2.*(n-1.)/(n+1.); // start z
    for (int i=18; i>0; i--) // z-slices from far to near
    {
        xyz = vec3(uv*(2.+z)*td,z); // form 3D coordinates from screen coordinates and z
        cm = max(cm, float(torus(xyz,kt)<.1)); // draw torus mask
        ct = max(ct, wire(torus(xyz,kt))/(z-2.6)); // draw torus slice
        z -= 4./(n+1.); // density of render, smaller is better
    };

    cm = 1.-cm; // torus mask

    if (t>46.)
    {
    // raymarching
        xyz = normalize(vec3(uv, 1.)); // view direction

        for (int i=0; i<40; i++)
        {
            d = rt(xyz*l);
            if (d<.001) break;
            l += d;
            if (l>40.) break;
        }
        if (l<40.)
        {
            xyz *= l; // actual point
            norm = normalize(vec3(
            rt(xyz+vec3(e,0.,0.)) - rt(xyz-vec3(e,0.,0.)),
            rt(xyz+vec3(0.,e,0.)) - rt(xyz-vec3(0.,e,0.)),
            rt(xyz+vec3(0.,0.,e)) - rt(xyz-vec3(0.,0.,e))));
            cl = cm*pow((length(norm+normalize(light-xyz))-1.1)*max(1.2-length(light-xyz)/15.,0.),4.);
        }
    }

// draw stars
    e = 2371.;

    for(int i=0; i<40; i++)
    {
        n = t/15.;
        if (t>=47.1)
        {
            n = 47.1/15. + (t-47.1)/20.;
        }
        z = fract(n+r); // n = slow time
        m2 = s(uv.y,z);
        if(m2 > 935.) // if higher than field
        {
            m1 = s(uv.x,z);
            d = fract( sin( pow(floor(m1), floor(m2)/941.) ) * e );
            if(d > .9985)
            {
                d = distance(vec2(m1,m2),vec2(floor(m1)+.5,floor(m2)+.5))*2.;
                cs = max ( cs, z * min(1.,pow(1.5-d,15.)) );
                cs = max ( cs, z * max(0.,1.-d) );
            }
        }
        e-=.9;
        r += .05;
    }

    cf *= cm; // apply torus mask to the field
    cs *= cm; // apply torus mask to stars

    cf *= clamp(t/4.-2.5,0.,1.); // fade in field on 0:10
    cs *= clamp(t-31.,0.,1.); // fade in stars on 0:31

    cl += cm*max(2.-l/20.,0.)*max(norm.z,0.); // add z-normals to the field light

    cl *= pow(clamp(t*2.-93.2,0.,1.),3.); // fade in the field light at 0:46.6 - 0:47.1
	cl=max(0.,cl);
	
    c = vec3(cs/2., cs*cs, sqrt(cs)) +    // mix stars
        vec3(0., cl*cl, cl) +    // mix lights
        vec3(0., ct*ct, ct) +    // mix torus
        vec3(cf*cf/2., 0., cf);  // mix field

    c *= min(1.,t/4.); // global fade in

    d = 2.-clamp(t/4.-12.75,0.,2.); // run big circle at 0:51
    cf = smoothstep(1.,0.,abs(d-length(uv))) ; // global fade out
    c += vec3(0., cf*cf, cf);    // mix big circle

    d = 2.-clamp(t/4.-13.5,0.,2.2); // global fadeout at 54:00
    cf = smoothstep(d-.1,d+.1,length(uv)) ;
    c *= 1. - cf;
} // endif t < 1:02



else // Scene 2 from 1:02
{

if (t<104.)
{
    m1 = min(t,85.)-62.; // stop move at 1:25
    size = 31. - m1; // HAL size

    z = fract(cos(m1*3.9)+sin(m1)*cos(m1*3.7)) ;// random blinking
    if (t<65.) z = 3.-m1;

    r = length(uv) * size;
    cf = max(0.,.7-min(1.,r)); // red
    ct = pow( r/99.*float(r<.6) , .4)    +    (.8-r)*float(r<.02); // blue
    cm = min(1.,pow(max(0.,1.25-r),8.))/(10.-z*3.)    +    (z*.8+.3)*min(.7,pow(max(0.,.15-r)*9.,12.)); // yellow;

    n = uv.y*size; // gradient

// add plasma to red
    m2 =
    .3+.6*cos(cos(9.*(uv.x-.4)*pi + t/3.) + 9.*uv.y + t/7.) -
    .6+.6*cos(cos(9.*uv.y*pi + t/5.) + 9.*(uv.x-.4) - t/9.);
    cf += clamp(m2, 0., 1.)/3. * smoothstep(.1, 0., r-.4); // plasma * mask
// end plasma
    cf = mix(cf, floor(cf*16.)/16., max(0.,0.-n)); // discret red


    xyz = vec3(cf+cm,cm,ct); // HAL 9000

	cf = atan(abs(uv.x),uv.y);

    //cm = clamp(pow(1.3-abs(cf),7.), 0.,1.); // mask
    //cm+= clamp(pow(1.05-abs(.8-cf),7.), 0.,1.); // mask
    //cm *= smoothstep(.1, 0., r-.53); // mask red radius
	
	cm = smoothstep(.1, 0., r-.53) * clamp(pow(1.3-abs(cf),7.) + pow(1.05-abs(.8-cf),7.), 0.,1.); // mask
	
    cl = max(0.,n) * pow(1.-sqrt(abs(r-.55)), 10.); // lights
    cl += (0.-pow(abs(n),3.)) * pow(1.-sqrt(abs(r-.5)), 14.);


    cl *= (.5 + cm/2.); // apply mask to the lights


    cf = smoothstep(.97, .99, 1.035-abs(r-.68)); // rim
    cf -= (1.-z/3.) * .5 * float(r>.61) * smoothstep(.0,.01,.685-r);

    r = length(uv*vec2(.7,1.))*size; // lamps perspective


    m2 = 1.35-fract(m1/24.); // ideal radius is .36
    ct = 0.;
    for(int i=1; i<4; i++)
    {
        ct += min(.8, max(0.,n) * pow(1.2-sqrt(abs(length(r-m2/float(i)))), 12.) )/((float(i)+.5)/1.5); // cool lamp
    }

    ct*=cm;

    cl += ct;

    light = vec3(cl*.9,cl*.9,cl);
    norm = .4 * sqrt(n + .8) * vec3(cf/2.7,cf/2.,cf/1.5); // border

    c = float(r<2.)*(xyz+light+norm);
    c *= min(1.,(t-62.)/4.); // global fade in at 1:02

}
}
    gl_FragColor = vec4(c, 1.);
}