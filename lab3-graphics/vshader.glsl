attribute vec3 vPosition;
uniform mat4 model_view;
uniform mat4 projection;
uniform mat4 transformation;
uniform mat4 cube_rotate;

void main()
{
    gl_Position =  projection *model_view*cube_rotate*transformation*vec4(vPosition,1.0);
}

