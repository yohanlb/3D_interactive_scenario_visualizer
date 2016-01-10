#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform float blurValue;

const float offset = 1.0 / 300;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );



   float m1 = blurValue * 1 / 16;
   float m2 = blurValue * 2 / 16;
   float m3 = 1 - (4*m1 + 4*m2);
    
    float kernel[9] = float[](
        m1, m2, m1,
        m2, m3, m2,
        m1, m2, m1
    );

/*
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    

   float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0
        1.0, 1.0, 1.0
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
*/  


    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col;
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    color = vec4(col, 1.0);
} 