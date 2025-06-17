#define PI 3.141592653589793

#define EXPLOSION_COUNT 8.
#define SPARKS_PER_EXPLOSION 128.
#define EXPLOSION_DURATION 20.
#define EXPLOSION_SPEED 5.
#define EXPLOSION_RADIUS_THESHOLD .06

// Hash function by Dave_Hoskins.
#define MOD3 vec3(.1031,.11369,.13787)
vec3 hash31(float p) {
   vec3 p3 = fract(vec3(p) * MOD3);
   p3 += dot(p3, p3.yzx + 19.19);
   return fract(vec3((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y, (p3.y + p3.z) * p3.x));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    float aspectRatio = iResolution.x / iResolution.y;
    vec2 uv = fragCoord / iResolution.y;
    float t = mod(iTime + 10., 7200.);;
	vec3 col = vec3(0.); 
    vec2 origin = vec2(0.);
    
    for (float j = 0.; j < EXPLOSION_COUNT; ++j)
    {
        vec3 oh = hash31((j + 1234.1939) * 641.6974);
        origin = vec2(oh.x, oh.y) * .6 + .2; // .2 - .8 to avoid boundaries
        origin.x *= aspectRatio;
        // Change t value to randomize the spawning of explosions
        t += (j + 1.) * 9.6491 * oh.z;
        for (float i = 0.; i < SPARKS_PER_EXPLOSION; ++i)
    	{
            // Thanks Dave_Hoskins for the suggestion
            vec3 h = hash31(j * 963.31 + i + 497.8943);
            // random angle (0 - 2*PI)
            float a = h.x * PI * 2.;
            // random radius scale for spawning points anywhere in a circle
            float rScale = h.y * EXPLOSION_RADIUS_THESHOLD;
            // explosion loop based on time
            if (mod(t * EXPLOSION_SPEED, EXPLOSION_DURATION) > 2.)
            {
                // random radius 
                float r = mod(t * EXPLOSION_SPEED, EXPLOSION_DURATION) * rScale;
                // explosion spark polar coords 
                vec2 sparkPos = vec2(r * cos(a), r * sin(a));
               	// sparkPos.y -= pow(abs(sparkPos.x), 4.); // fake gravity
                // fake-ish gravity
                float poopoo = 0.04;
                float peepee = (length(sparkPos) - (rScale - poopoo)) / poopoo;
                sparkPos.y -= pow(peepee, 3.0) * 6e-5;
                // shiny spark particles
                float spark = .0002/pow(length(uv - sparkPos - origin), 1.65);
                // Make the explosion spark shimmer/sparkle
                float sd = 2. * length(origin-sparkPos);
                float shimmer = max(0., sqrt(sd) * (sin((t + h.y * 2. * PI) * 20.)));
                float shimmerThreshold = EXPLOSION_DURATION * .32;
                // fade the particles towards the end of explosion
                float fade = max(0., (EXPLOSION_DURATION - 5.) * rScale - r);
                // mix it all together
                col += spark * mix(1., shimmer, smoothstep(shimmerThreshold * rScale,
					(shimmerThreshold + 1.) * rScale , r)) * fade * oh;
            }
    	}
    }
    
    // evening-sh background gradient
    col = max(vec3(.1), col);
    col += vec3(.12, .06, .02) * (1.-uv.y);
    fragColor = vec4(col, 1.0);
} 