static const char* fragment = \
"uniform int v;"
"float m=v*.001;"
"float s(vec3 f,float x)"
"{"
"f.z-=4.5;"
"vec3 p=vec3(m/2.3,m/1.95,m/2.7),c=sin(p),s=cos(p);"
"f*=mat3(s.y,c.z*c.y,-c.y*s.z,-c.x*c.y,s.x*s.z+c.x*c.z*s.y,c.z*s.x-c.x*s.z*s.y,s.x*c.y,c.x*s.z-s.x*c.z*s.y,c.z*c.x+s.x*s.z*s.y);"
"float y=clamp((m-33.)/14.,0.,1.);"
"x=mix(x,0.,y*y*y);"
"float i=8.-6.*x,z=3.8*x-2.;"
"i=mix(i,2.,y*y*y);"
"f=abs(f);"
"return pow(pow(f.x,i)+pow(f.y,i)+pow(f.z,i)+z,2.)-x*8.*(f.x*f.x+f.y*f.y);"
"}"
"float s(vec3 s)"
"{"
"return sin(s.x*5.)/15.+cos(s.x+1.5)/3.+sin(s.z+m)/5.-cos(s.x+2.)*cos(s.z+m)/3.;"
"}"
"float p(vec3 m)"
"{"
"return length(m-vec3(m.x,s(m)-1.5,m.z));"
"}"
"float p(float m,float s)"
"{"
"return(m-m*s+1.)*991.;"
"}"
"float x(float m)"
"{"
"return pow(1.-min(1.,abs(m)),4.);"
"}"
"void main()"
"{"
#if YRES == 720
"vec2 f=gl_FragCoord.xy/vec2(1280,720);"
"float i=16./9.;"
#elif YRES == 1080
"vec2 f=gl_FragCoord.xy/vec2(1920,1080);"
"float i=16./9.;"
#else
#if YRES == 768
"vec2 f=gl_FragCoord.xy/vec2(1024,768);"
"float i=4./3.;"
#endif
#endif
"vec2 c=(f-vec2(.5))/vec2(1.,i)*1.4;"
"float y=0.,z=0.,a=0.,l=0.,r=0.,o=3.1416,n=1.,b,e=.0001,k,t,w=0.,h,g,u;"
"vec3 d,q=vec3(0.,0.,0.),F=q,C=vec3(0.,1.,12.+max(0.,(m-47.)/8.));"
"if(m<62.)"
"{"
"float Z=clamp(m/4.-5.,0.,1.),Y=cos(m/3.1)/2.5+.4,X=m-22.,W=1.+max(X*X*X/999.,0.)/9.;"
"g=min(m*m*m/451.,17.7)+.3;"
"g=mix(g,3.,clamp((m-42.)/5.,0.,1.));"
"u=30.-fract(m);"
"for(int V=30;V>0;V--)"
"d=vec3(c*(1.+u),u),y=max(y,pow(x(-d.y-1.5+Z*s(d+vec3(0.,0.,1.))),7.)*-c.y*2.6),u--;"
"u=4.5+2.*(g-1.)/(g+1.);"
"for(int V=18;V>0;V--)"
"d=vec3(c*(2.+u)*W,u),a=max(a,float(s(d,Y)<.1)),z=max(z,x(s(d,Y))/(u-2.6)),u-=4./(g+1.);"
"a=1.-a;"
"if(m>46.)"
"{"
"d=normalize(vec3(c,1.));"
"for(int V=0;V<40;V++)"
"{"
"b=p(d*n);"
"if(b<.001)"
"break;"
"n+=b;"
"if(n>40.)"
"break;"
"}"
"if(n<40.)"
"d*=n,F=normalize(vec3(p(d+vec3(e,0.,0.))-p(d-vec3(e,0.,0.)),p(d+vec3(0.,e,0.))-p(d-vec3(0.,e,0.)),p(d+vec3(0.,0.,e))-p(d-vec3(0.,0.,e)))),l=a*pow((length(F+normalize(C-d))-1.1)*max(1.2-length(C-d)/15.,0.),4.);"
"}"
"e=2371.;"
"for(int V=0;V<40;V++)"
"{"
"g=m/15.;"
"if(m>=47.1)"
"g=3.14+(m-47.1)/20.;"
"u=fract(g+w);"
"t=p(c.y,u);"
"if(t>935.)"
"{"
"k=p(c.x,u);"
"b=fract(sin(pow(floor(k),floor(t)/941.))*e);"
"if(b>.9985)"
"b=distance(vec2(k,t),vec2(floor(k)+.5,floor(t)+.5))*2.,r=max(r,u*min(1.,pow(1.5-b,15.))),r=max(r,u*max(0.,1.-b));"
"}"
"e-=.9;"
"w+=.05;"
"}"
"y*=a;"
"r*=a;"
"y*=clamp(m/4.-2.5,0.,1.);"
"r*=clamp(m-31.,0.,1.);"
"l+=a*max(2.-n/20.,0.)*max(F.z,0.);"
"l*=pow(clamp(m*2.-93.2,0.,1.),3.);"
"l=max(0.,l);"
"q=vec3(r/2.,r*r,sqrt(r))+vec3(0.,l*l,l)+vec3(0.,z*z,z)+vec3(y*y/2.,0.,y);"
"q*=min(1.,m/4.);"
"b=2.-clamp(m/4.-12.75,0.,2.);"
"y=smoothstep(1.,0.,abs(b-length(c)));"
"q+=vec3(0.,y*y,y);"
"b=2.-clamp(m/4.-13.5,0.,2.2);"
"y=smoothstep(b-.1,b+.1,length(c));"
"q*=1.-y;"
"}"
"else"
"{"
"if(m<104.)"
"{"
"k=min(m,85.)-62.;"
"h=31.-k;"
"u=fract(cos(k*3.9)+sin(k)*cos(k*3.7));"
"if(m<65.)"
"u=3.-k;"
"w=length(c)*h;"
"y=max(0.,.7-min(1.,w));"
"z=pow(w/99.*float(w<.6),.4)+(.8-w)*float(w<.02);"
"a=min(1.,pow(max(0.,1.25-w),8.))/(10.-u*3.)+(u*.8+.3)*min(.7,pow(max(0.,.15-w)*9.,12.));"
"g=c.y*h;"
"t=.3+.6*cos(cos(9.*(c.x-.4)*o+m/3.)+9.*c.y+m/7.)-.6+.6*cos(cos(9.*c.y*o+m/5.)+9.*(c.x-.4)-m/9.);"
"y+=clamp(t,0.,1.)/3.*smoothstep(.1,0.,w-.4);"
"y=mix(y,floor(y*16.)/16.,max(0.,-g));"
"d=vec3(y+a,a,z);"
"y=atan(abs(c.x),c.y);"
"a=smoothstep(.1,0.,w-.53)*clamp(pow(1.3-abs(y),7.)+pow(1.05-abs(.8-y),7.),0.,1.);"
"l=max(0.,g)*pow(1.-sqrt(abs(w-.55)),10.);"
"l+=-pow(abs(g),3.)*pow(1.-sqrt(abs(w-.5)),14.);"
"l*=.5+a/2.;"
"y=smoothstep(.97,.99,1.035-abs(w-.68));"
"y-=(1.-u/3.)*.5*float(w>.61)*smoothstep(0.,.01,.685-w);"
"w=length(c*vec2(.7,1.))*h;"
"t=1.35-fract(k/24.);"
"z=0.;"
"for(int V=1;V<4;V++)"
"z+=min(.8,max(0.,g)*pow(1.2-sqrt(abs(length(w-t/float(V)))),12.))/((float(V)+.5)/1.5);"
"z*=a;"
"l+=z;"
"C=vec3(l*.9,l*.9,l);"
"F=.4*sqrt(g+.8)*vec3(y/2.7,y/2.,y/1.5);"
"q=float(w<2.)*(d+C+F);"
"q*=min(1.,(m-62.)/4.);"
"}"
"}"
"gl_FragColor=vec4(q,1.);"
"}";