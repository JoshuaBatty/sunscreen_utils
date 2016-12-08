#version 410

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;
uniform sampler2DRect iChannel0;          // input channel. XX = 2D/Cube

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;

// params
uniform float brightness;
uniform float contrast;
uniform float saturation;


mat4 brightnessMatrix( float brightness )
{
    return mat4( 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                brightness, brightness, brightness, 1 );
}

mat4 contrastMatrix( float contrast )
{
    float t = ( 1.0 - contrast ) / 2.0;
    
    return mat4( contrast, 0, 0, 0,
                0, contrast, 0, 0,
                0, 0, contrast, 0,
                t, t, t, 1 );
    
}

mat4 saturationMatrix( float saturation )
{
    vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
    
    float oneMinusSat = 1.0 - saturation;
    
    vec3 red = vec3( luminance.x * oneMinusSat );
    red+= vec3( saturation, 0, 0 );
    
    vec3 green = vec3( luminance.y * oneMinusSat );
    green += vec3( 0, saturation, 0 );
    
    vec3 blue = vec3( luminance.z * oneMinusSat );
    blue += vec3( 0, 0, saturation );
    
    return mat4( red,     0,
                green,   0,
                blue,    0,
                0, 0, 0, 1 );
}


void main( void )
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec4 color = texture( iChannel0, gl_FragCoord.xy);
    
     outputColor = brightnessMatrix( brightness ) *
     contrastMatrix( contrast ) *
     saturationMatrix( saturation ) *
     color;
    
//    outputColor = vec4(uv.x, uv.y, 1.0, 1.0);
//    outputColor = color;
}