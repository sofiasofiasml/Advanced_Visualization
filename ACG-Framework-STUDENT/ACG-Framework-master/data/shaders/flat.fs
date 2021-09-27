varying vec2 v_uv;

uniform vec4 u_color;
uniform sampler2D Texture;

void main()
{
	gl_FragColor = texture(Texture, v_uv);

}
