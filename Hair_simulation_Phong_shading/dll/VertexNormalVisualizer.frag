#version 150 compatibility
uniform int seg;
in float special;
void main(){
    float red=0.0f,green=0.0f,blue=0.0f,alpha=0.0f,m;
	m=special/seg;
	red=mix(0.0f,0.3f,m);
	green=mix(0.0f,0.3f,m);
	blue=mix(0.0f,0.7f,m);
	alpha=mix(1.0f,0.0f,m);
    gl_FragColor = vec4(red, green, blue, alpha);
}
