#version 410 core

in vec2 tex_pos;

out vec4 frag_colour;

uniform sampler2D img_texture;

uniform int grayscale;
uniform int quantize;

// quantizes to 2 bits
float two_bit_quantize(float colour) {
    if (colour > 0.75f) {
        return 1.f;
    } else if (colour > 0.5f) {
        return (2.f / 3.f);
    } else if (colour > 0.25f) {
        return (1.f / 3.f);
    } else {
        return 0.f;
    }
}

void main()
{
    frag_colour = texture(img_texture, tex_pos);

    if (grayscale == 1) {
        float gray = dot(frag_colour.rgb, vec3(0.299, 0.587, 0.114));
        frag_colour = vec4(gray, gray, gray, 1.f);
    }

    if (quantize == 1) {
        frag_colour.r = two_bit_quantize(frag_colour.r);
        frag_colour.g = two_bit_quantize(frag_colour.g);
        frag_colour.b = two_bit_quantize(frag_colour.b);
    }
}
