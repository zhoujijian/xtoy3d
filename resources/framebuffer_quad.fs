#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;

const float offset = 1.0 / 300.0;  

vec4 convolution_matrix(float kernel[9]) {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

vec4 convolution() {
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    return convolution_matrix(kernel);
}

vec4 blur() {
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    return convolution_matrix(kernel);
}

vec4 edge_dectection() {
    float kernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
    return convolution_matrix(kernel);
}

vec4 inversion() {
    return (vec4(1.0) - (texture(texture1, TexCoords)));
}

vec4 grayscale() {
    vec4 color = texture(texture1, TexCoords);
    float average = (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
    return vec4(average, average, average, color.a);
}

void main() {
    // FragColor = inversion();
    // FragColor = grayscale();
    // FragColor = convolution();
    // FragColor = blur();
    FragColor = edge_dectection();
}