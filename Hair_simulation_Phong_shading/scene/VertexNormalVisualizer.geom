#version 150 compatibility
layout(triangles) in;
layout(line_strip, max_vertices=100) out;

in Vertex{
    vec3 normal;
}vertex[];

void main(){
    float length = 2.5f;
    for(int i = 0; i < gl_in.length(); i++){
        //start point
        gl_Position = gl_ProjectionMatrix * gl_in[i].gl_Position;
        EmitVertex();

        //end point   //vertex[i].normal
        gl_Position = gl_ProjectionMatrix * (gl_in[i].gl_Position + vertex[i].normal * length);
        EmitVertex();

        EndPrimitive();
	  }
	  
}
