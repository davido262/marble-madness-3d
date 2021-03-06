/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  David Cavazos <davido262@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "engine/renderer/camera.hpp"

#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/kernel/entity.hpp"

using namespace std;

const string CAMERA_DESCRIPTION = "$camera";
const float DEFAULT_PERSP_FOV = 45.0f;
const float DEFAULT_ORTHO_HEIGHT = 10.0f;
const float DEFAULT_NEAR_DISTANCE = 0.1f;
const float DEFAULT_FAR_DISTANCE = 1000.0f;

Camera::Camera(Entity* const entity, const camera_t type):
    Component(COMPONENT_CAMERA, entity),
    m_type(type),
    m_hasChanged(true),
    m_viewport(),
    m_aspectRatio(0.0f),
    m_perspectiveFOV(DEFAULT_PERSP_FOV),
    m_orthoHeight(DEFAULT_ORTHO_HEIGHT),
    m_nearDistance(DEFAULT_NEAR_DISTANCE),
    m_farDistance(DEFAULT_FAR_DISTANCE)
{
    m_description = CAMERA_DESCRIPTION + "_";
    switch (m_type) {
    case CAMERA_ORTHOGRAPHIC:
        m_description.append("ORTHOGRAPHIC");
        break;
    case CAMERA_PROJECTION:
        m_description.append("PROJECTION");
        break;
    default:
        m_description.append("INVALID");
    }

    RenderManager::getRenderer().m_cameras.insert(this);
    RenderManager::getRenderer().m_activeCamera = this;

    m_entity.registerAttribute("type", boost::bind(&Camera::cmdType, this, _1));
    m_entity.registerAttribute("perspective-fov", boost::bind(&Camera::cmdPerspectiveFOV, this, _1));
    m_entity.registerAttribute("ortho-height", boost::bind(&Camera::cmdOrthoHeight, this, _1));
    m_entity.registerAttribute("near-distance", boost::bind(&Camera::cmdNearDistance, this, _1));
    m_entity.registerAttribute("far-distance", boost::bind(&Camera::cmdFarDistance, this, _1));
}

Camera::~Camera() {
    RenderManager::getRenderer().m_cameras.erase(this);
}



void Camera::cmdType(const std::string& arg) {
    int type;
    stringstream ss(arg);
    ss >> type;
    setType(static_cast<camera_t>(type));
}

void Camera::cmdPerspectiveFOV(const std::string& arg) {
    float perspectiveFOV;
    stringstream ss(arg);
    ss >> perspectiveFOV;
    setPerspectiveFOV(perspectiveFOV);
}

void Camera::cmdOrthoHeight(const std::string& arg) {
    float orthoHeight;
    stringstream ss(arg);
    ss >> orthoHeight;
    setOrthoHeight(orthoHeight);
}

void Camera::cmdNearDistance(const std::string& arg) {
    float nearDistance;
    stringstream ss(arg);
    ss >> nearDistance;
    setNearDistance(nearDistance);
}

void Camera::cmdFarDistance(const std::string& arg) {
    float farDistance;
    stringstream ss(arg);
    ss >> farDistance;
    setFarDistance(farDistance);
}
