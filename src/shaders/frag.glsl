#version 330 core

out vec4 FragColor;

in vec2 fitMaps;

uniform bool edges;
uniform bool sFitMap;
uniform bool mFitMap;

void main(){
    if(edges){
        FragColor = vec4(vec3(0.0), 1.0);
    } else if(sFitMap){
        FragColor = vec4(1.0-fitMaps.x, 1.0-fitMaps.x, fitMaps.x, 1.0);
    } else if(mFitMap){
        FragColor = vec4(1.0-fitMaps.y, 1.0-fitMaps.y, fitMaps.y, 1.0);
    } else {
        FragColor = vec4(1.0);
    }
}