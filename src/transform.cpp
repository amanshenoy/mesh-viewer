#include <iostream>

#include "transform.h"

namespace gl {
mat3 Transform::rotate(const float degrees, const vec3& axis){
	const float radian = degrees * (3.14159265f / 180.0f);  
	glm::mat3 dot = glm::mat3(
		axis.x * axis.x, axis.x * axis.y, axis.x * axis.z,
		axis.x * axis.y, axis.y * axis.y, axis.y * axis.z,
		axis.x * axis.z, axis.y * axis.z, axis.z * axis.z
		);
	glm::mat3 cross = glm::mat3(
		0, -axis.z, axis.y,
		axis.z, 0, -axis.x,
		-axis.y, axis.x, 0
		);
	return mat3(mat3(cos(radian)) * mat3(1.0f) + mat3(1.0f - cos(radian)) * dot \
			+ mat3(sin(radian)) * glm::transpose(cross));
}

void Transform::left(float degrees, vec3& eye, vec3& up){
	mat3 rotation = (rotate(degrees, normalize(up))); 
	eye = rotation * eye; 
}

void Transform::up(float degrees, vec3& eye, vec3& up){
	vec3 axis_for_up = glm::cross(glm::normalize(up), glm::normalize(eye));
	mat3 rotation = rotate(degrees, glm::normalize(axis_for_up)); 
	eye = eye * rotation; 
	up = up * rotation; 
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz){
  	mat4 ret;
	ret = glm::transpose(mat4(
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1
		));
	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz){
  	mat4 ret;
	ret = transpose(mat4(
		1, 0, 0, tx,
		0, 1, 0, ty,
		0, 0, 1, tz,
		0, 0, 0, 1
		));	
	return ret;
}

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec){
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}
}