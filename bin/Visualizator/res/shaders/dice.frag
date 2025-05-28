// Original shader from https://www.shadertoy.com/view/csySDG
// Created by fizzer in 2023-04-14

#version 120

uniform vec2      resolution  ;
uniform float     currentAngle;
uniform sampler2D texture     ;
///uniform vec4   mouse       ;

const int     MAX_STEPS = 100 ;
const float   MAX_DIST  = 10.0;
const float   EPSILON   = 1e-4;

float hole(vec3 p, float r)
{   float c = min(0.0,   length(p.xz) - r);
    return (p.y > 0.0) ? length(vec2(c, p.y)) : r - length(p);
}

float sdRoundBox(vec3 p, vec3 b, float r)
{   vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y,q.z)),0.0) - r;
}

float die(vec3   p)
{   vec3  q    = abs(p);
    vec3  b    = p.xzy * sign(p.z);
    bvec3 dots = bvec3     (false);

    if (sign(p.z) > 0.0) dots = bvec3(false, true, true );
    else                 dots = bvec3(true , true, false);

    if (q.x >  q.y  && q.x > q.z)
    {   b   =  p.zxy * sign (p.x);
        b.z = -b.z;
        if (sign(p.x) > 0.0) dots = bvec3(false, true, false);
        else                 dots = bvec3(true , true, true );
    }
    else if (q.y   > q.z)
    {   b =  p.zyx * sign(p.y);
        if (sign(p.y) > 0.0) dots = bvec3(false, false, true );
        else                 dots = bvec3(false, false, false);
    }

    float r    = 0.02, r2 = 0.04, r3 = 0.13;
          b.y -= 0.5 - r;
    float d;

    if (all(not(dots)))
    {   b.x = abs(b.x);
        b.z = mod(b.z + 0.15, 0.3) - 0.15;
        d = hole(b - vec3(0.2, 0.0, 0.0), r3);
    }
    else
    {   if (b.x > -b.z) b.xz = -b.zx;
        if (b.x > b.z) b.xz = b.zx;

        if (b.z - b.x > 0.25)
            d = dots[1] ? hole(b - vec3(-0.25, 0.0, 0.25), r3) : b.y;
        else if (-b.x - b.z > 0.25)
            d = dots[0] ? hole(b - vec3(-0.25, 0.0,-0.25), r3) : b.y;
        else
            d = dots[2] ? hole(b, r3) : b.y;
    }

    return max(d - r, sdRoundBox(p, vec3(0.5) - r2, r2));
}

float dist(vec3 p)
{   return die(p);
}

vec3 norm(vec3 p)
{   vec2  e = vec2(1e-4, 0.0);
    float d = dist(p);
    return normalize(vec3( dist(p + e.xyy) - d,
                           dist(p + e.yxy) - d,
                           dist(p + e.yyx) - d
    ));
}

void main()
{
    vec2 uv    = (gl_FragCoord.xy - resolution * 0.5) / resolution.y;
/// vec2 uv2   =  gl_FragCoord.xy / resolution.xy;
///      uv2.y = 1.0 - uv2.y;

    vec3 campos = vec3(
        cos(currentAngle) * 2.0,
        //1.0,
        sin(currentAngle  * 0.5) * 2.0,
        //0.0,
        sin(currentAngle) * 2.0
        //1.0
    );

    vec3 camtarget = vec3(0.0);
    vec3 ro        = campos;
    vec3 rd        = normalize(vec3(uv, 1.0));

    vec3 w = normalize(camtarget - ro);
    vec3 u = normalize(cross(vec3(0.0, 1.0, 0.0), w));
    vec3 v = cross(w, u);
    rd     = mat3 (u, v, w) * rd;

    float     t = 0.0;
    for(int   i = 0; i < MAX_STEPS; ++i)
    {   float d = dist(ro + rd * t);
        if  ( d < EPSILON || t > MAX_DIST ) break;
        t  += d;
    }

/// vec3 col = texture2D(texture, gl_TexCoord[0].xy).rgb;
/// vec3 col = texture2D(texture, uv2).rgb;

    if(t < MAX_DIST)
    {   vec3 rp      = ro + rd * t;
        vec3 l       = normalize(vec3(1.0, 2.0, 1.5));
        vec3 n       = norm(rp);
        vec3 baseCol = mix (vec3(0.0), vec3(1.0),
            smoothstep     (0.01, 0.02, sdRoundBox(rp, vec3(0.38), 0.1))
        );

        vec3  rr      = reflect(rd, n);
        vec3  spec    = texture2D(texture, rr.xy * 0.5 + 0.5).rgb;
        float fresnel = mix(0.01, 0.4,
            pow(clamp(1.0 + dot(rd, n), 0.0, 1.0), 2.0)
        );

        vec3     col  = mix(baseCol * (0.5 + 0.5 * dot(n, l)), spec, fresnel);
        gl_FragColor  = vec4(pow(col, vec3(1.0 / 2.2)), 1.0);
    }
    else gl_FragColor = vec4(0.0);
}
