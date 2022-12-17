attribute vec3 a_vertexPosition;
varying vec4 fragmentColor;

void main() {
    gl_Position = vec4(a_vertexPosition, 1.0);
    fragmentColor = vec4(1.0, 1.0, 1.0, 1.0f);
}