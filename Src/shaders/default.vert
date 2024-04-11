#version 410 core
in vec2 point;
void main() {
	gl_Position = vec4(point, 0, 1);
}