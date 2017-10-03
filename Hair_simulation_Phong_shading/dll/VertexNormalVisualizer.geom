#version 150 compatibility
layout(triangles) in;
layout(line_strip, max_vertices=255) out;
out float special;
uniform float len;
uniform int seg;
uniform vec3 gravity;
vec4 pos;
vec4 normal2;
in Vertex{
    vec3 normal;
}vertex[];

void main(){
    float length = len;
    for(int i = 0; i < gl_in.length(); i++){
	     pos=gl_in[i].gl_Position;
		 normal2=normalize(vec4(vertex[i].normal+gravity, 0.0f));
        for(int j=0;j<seg;j++)
		{
        gl_Position = gl_ProjectionMatrix * pos;
        EmitVertex();

        //end point   //vertex[i].normal
        gl_Position = gl_ProjectionMatrix * (pos +normal2 * len);
		pos+= normal2*len;
	    normal2=normalize(normal2+vec4(gravity, 0.0f));
        EmitVertex();
        special=j;
        EndPrimitive();
		}
	  }
	  
}
