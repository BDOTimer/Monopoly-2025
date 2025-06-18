#version 120

uniform vec2      iResolution;
uniform float     iTime      ;
uniform sampler2D texture    ;

#define NUM_EXPLOSIONS 5.
#define NUM_PARTICLES 75.

vec2 Hash12(float t){

float x = fract(sin(t*674.3)*453.2);
float y = fract(sin((t+x)*714.3)*263.2);

return vec2(x, y);
}

vec2 Hash12_Polar(float t){

float p_Angle = fract(sin(t*674.3)*453.2)*6.2832;
float p_Dist = fract(sin((t+p_Angle)*714.3)*263.2);

return vec2(sin(p_Angle), cos(p_Angle))*p_Dist;
}

float Explosion(vec2 uv, float t){

 float sparks = 0.;

    for(float i = 0.; i<NUM_PARTICLES; i++){

        vec2 dir = Hash12_Polar(i+1.)*.5;
        float dist = length(uv-dir*t);
        float brightness = mix(.0005, .0005, smoothstep(.05, 0., t));

        brightness *= sin(t*20.+i)*.5+.5;
        brightness*= smoothstep(1., .6, t);
        sparks += brightness/dist;
    }
    return sparks;
}

/// void mainImage( out vec4 fragColor, in vec2 fragCoord )
void main()
{
    vec2 uv = (gl_FragCoord.xy -.5*iResolution.xy)/iResolution.y;

    vec3 col = vec3(0);

    for(float  i = 0.; i<NUM_EXPLOSIONS; i++)
    {
        float t  = iTime+i/NUM_EXPLOSIONS;
        float ft = floor(t);

        vec3 color = sin(4.*vec3(.34,.54,.43)*ft)*.25+.75;

        vec2 offset = Hash12(i+1.+ft)-.5;
        offset*=vec2(1.77, 1.);
        //col+=.0004/length(uv-offset);

         col += Explosion(uv-offset, fract(t))*color;
       }

    col*=2.;
    gl_FragColor = vec4(col,1.0);
}
