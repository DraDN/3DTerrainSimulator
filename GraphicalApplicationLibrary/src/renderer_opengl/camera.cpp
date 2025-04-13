#include <renderer_opengl/camera.hpp>
#include <renderer_opengl/shader.hpp>

float gal::renderer_opengl::Camera::_angle_between(glm::vec3 a, glm::vec3 b, glm::vec3 origin, bool acos_or_asin) {
	glm::vec3 da = glm::normalize(a-origin);
	glm::vec3 db = glm::normalize(b-origin);

	if (acos_or_asin)
		return glm::degrees(glm::acos(glm::dot(da, db)));
	else 
		return glm::degrees(-glm::asin(glm::dot(da, db)));
}

void gal::renderer_opengl::Camera::_calculate_projection() {
	if (is_ortho) {
		projection = glm::ortho(size.x/2.f*glm::root_two<float>(), -size.y/2.f*glm::root_two<float>(),
								size.x/2.f,                        -size.y/2.f,
								glm::length(glm::vec2(position.x, position.z) - size.y/2.f*glm::root_two<float>()),
								glm::length(glm::vec2(position.x, position.z) + size.y*glm::root_two<float>()));
	} else {
		projection = glm::perspective(fov, size.x/size.y, NEAR, FAR);
	}
}

gal::renderer_opengl::Camera::Camera(glm::vec2 proj_size, glm::vec3 pos, glm::vec3 look_at, float fov = 0.f) {
	is_ortho = (fov == 0.f);
	size = proj_size;
	this->fov = fov;
	position = pos;

	_calculate_projection();

	front = glm::normalize(look_at - position);
	up = glm::vec3(0.f, 1.f, 0.f);

	yaw   = -_angle_between(glm::vec3(front.x, 0.f, front.z), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f), true);
	pitch = -_angle_between(front                           , glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), false);

	view = glm::lookAt(position, position+front, up);
}

void gal::renderer_opengl::Camera::send_shader_uniforms(Shader& shader) {
	shader.bind();
	shader.set("projection", projection);
	shader.set("view", view);
	shader.set("view_pos", position);
	shader.unbind();
}

void gal::renderer_opengl::Camera::set_size(float width, float height) {
	size.x = width;
	size.y = height;
	_calculate_projection();
}

void gal::renderer_opengl::Camera::rotate_by(float d_angle, glm::vec3 axis) {
	d_angle = glm::radians(d_angle);
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.f), d_angle, axis);

	view = glm::rotate(view, d_angle, axis);
	position = glm::vec3(rotation_matrix * glm::vec4(position, 1.f));
}

void gal::renderer_opengl::Camera::rotate(float yaw, float pitch) {
	pitch = (pitch >  89.f) ?  89.f : pitch;
	pitch = (pitch < -89.f) ? -89.f : pitch;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	view = glm::lookAt(position, position+front, up);
}

void gal::renderer_opengl::Camera::rotate_by(float d_yaw, float d_pitch) {
	yaw += d_yaw;
	pitch += d_pitch;

	rotate(yaw, pitch);
}

void gal::renderer_opengl::Camera::zoom(float zoom) {
	if (is_ortho) return;

	fov += zoom;

	fov = (fov <  1.f) ?  1.f : fov;
	fov = (fov > 90.f) ? 90.f : fov; 

	projection = glm::perspective(glm::radians(fov), size.x/size.y, NEAR, FAR);
}

void gal::renderer_opengl::Camera::set_position(glm::vec3 new_pos) {
	position = new_pos;
	view = glm::lookAt(position, position+front, up);
}

void gal::renderer_opengl::Camera::move_by(glm::vec3 d_pos) {
	position += d_pos.z * front;
	position += d_pos.x * glm::normalize(glm::cross(front, up));
	position += d_pos.y * up;

	view = glm::lookAt(position, position+front, up);
}

glm::mat4 gal::renderer_opengl::Camera::get_projection() {
	return projection;
}

glm::mat4 gal::renderer_opengl::Camera::get_view() {
	return view;
}
