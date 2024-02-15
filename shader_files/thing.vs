#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform int mirror;
out vec2 tex_coords;

void main()
{
   if(mirror == 1)
   {
		tex_coords = vec2(1 - vertex.z, vertex.w);
   }
   else
   {
		tex_coords = vec2(vertex.z, vertex.w);
   }

   gl_Position = proj * view * model * vec4(vertex.xy, 0.0, 1.0);
}