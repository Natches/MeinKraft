#version 450
in vec2 st;
layout(binding = 0)
uniform sampler2D tex;
uniform vec4 text_colour;
out vec4 frag_colour;
void main () 
{
  frag_colour = texture (tex, st) * text_colour;
}
