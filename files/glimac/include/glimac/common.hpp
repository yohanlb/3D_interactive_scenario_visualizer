#pragma once

#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

struct ShapeVertex {
	ShapeVertex():
	position(glm::vec3()),
	normal(glm::vec3()),
	texCoords(glm::vec2()) {}

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

}
