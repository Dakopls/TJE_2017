
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D u_texture;

void main()
{
    vec4 color = (0.0, 0.0, 0.0, 1.0);
	gl_FragColor = color;

}
