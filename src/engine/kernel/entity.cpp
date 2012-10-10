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


#include "engine/kernel/entity.hpp"

#include <iostream>
#include <cmath>
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/scenemanager.hpp"
#include "engine/kernel/component.hpp"

using namespace std;

const size_t INDENT_SIZE = 2;

Entity::Entity(const Entity* parent, const string& objectName):
    CommandObject(objectName),
    m_parent(*parent),
    m_children(),
    m_components(TOTAL_COMPONENTS_CONTAINER_SIZE, 0),
    m_positionAbs(VECTOR3_ZERO),
    m_positionRel(VECTOR3_ZERO),
    m_orientation(QUATERNION_IDENTITY),
    m_lastOrientation(QUATERNION_IDENTITY)
{
    registerAttribute("position", boost::bind(&Entity::setPosition, this, _1));
    registerCommand("move-xyz", boost::bind(&Entity::moveXYZ, this, _1));
    registerCommand("move-x", boost::bind(&Entity::moveX, this, _1));
    registerCommand("move-y", boost::bind(&Entity::moveY, this, _1));
    registerCommand("move-z", boost::bind(&Entity::moveZ, this, _1));
    registerCommand("move-xyz-parent", boost::bind(&Entity::moveXYZ_parent, this, _1));
    registerCommand("move-x-parent", boost::bind(&Entity::moveX_parent, this, _1));
    registerCommand("move-y-parent", boost::bind(&Entity::moveY_parent, this, _1));
    registerCommand("move-z-parent", boost::bind(&Entity::moveZ_parent, this, _1));
    registerCommand("move-xyz-global", boost::bind(&Entity::moveXYZ_global, this, _1));
    registerCommand("move-x-global", boost::bind(&Entity::moveX_global, this, _1));
    registerCommand("move-y-global", boost::bind(&Entity::moveY_global, this, _1));
    registerCommand("move-z-global", boost::bind(&Entity::moveZ_global, this, _1));
    registerCommand("yaw", boost::bind(&Entity::yaw, this, _1));
    registerCommand("pitch", boost::bind(&Entity::pitch, this, _1));
    registerCommand("roll", boost::bind(&Entity::roll, this, _1));
    registerCommand("yaw-parent", boost::bind(&Entity::yaw_parent, this, _1));
    registerCommand("pitch-parent", boost::bind(&Entity::pitch_parent, this, _1));
    registerCommand("roll-parent", boost::bind(&Entity::roll_parent, this, _1));
    registerCommand("yaw-global", boost::bind(&Entity::yaw_global, this, _1));
    registerCommand("pitch-global", boost::bind(&Entity::pitch_global, this, _1));
    registerCommand("roll-global", boost::bind(&Entity::roll_global, this, _1));
}

Entity::~Entity() {
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        delete *it;
    for (size_t i = 0; i < m_components.size(); ++i)
        delete m_components[i];
}

void Entity::translate(const Vector3& displacement, const transform_space_t& relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
        setPositionRel(m_positionRel + displacement.rotate(m_orientation));
        break;
    case SPACE_PARENT:
        setPositionRel(m_positionRel + displacement.rotate(m_parent.m_orientation));
        break;
    case SPACE_GLOBAL:
        setPositionAbs(m_positionAbs + displacement);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Entity::rotate(const Quaternion& deltaRotation, const transform_space_t& relativeTo) {
    switch (relativeTo) {
    case SPACE_LOCAL:
        setOrientation(m_orientation * deltaRotation);
        break;
    case SPACE_PARENT:
        // Qc' = Qp' * Inv(Qp) * Qc
        // Where:
        // Qp  = Parent orientation last frame
        // Qp' = Parent orientation this frame
        // Qc  = Child orientation last frame
        // Qc' = Child orientation this frame
        setOrientation(m_parent.m_orientation * m_parent.m_lastOrientation.inverse() * deltaRotation * m_orientation);
        break;
    case SPACE_GLOBAL:
        setOrientation(deltaRotation * m_orientation);
        break;
    default:
        cerr << "Invalid transform_space_t: " << relativeTo << endl;
    }
}

void Entity::setDirection(const Vector3& target) {
    if (target == VECTOR3_ZERO)
        return;
    cerr << "Transform.setDirection(vector3) not implemented yet!" << endl;
}

void Entity::calcOpenGLMatrix(float* m) const {
    scalar_t s = static_cast<scalar_t>(2.0) / m_orientation.lengthSquared();
    scalar_t xs = m_orientation.getX() * s;
    scalar_t ys = m_orientation.getY() * s;
    scalar_t zs = m_orientation.getZ() * s;
    scalar_t wx = m_orientation.getW() * xs;
    scalar_t wy = m_orientation.getW() * ys;
    scalar_t wz = m_orientation.getW() * zs;
    scalar_t xx = m_orientation.getX() * xs;
    scalar_t xy = m_orientation.getX() * ys;
    scalar_t xz = m_orientation.getX() * zs;
    scalar_t yy = m_orientation.getY() * ys;
    scalar_t yz = m_orientation.getY() * zs;
    scalar_t zz = m_orientation.getZ() * zs;
    m[0]  = static_cast<float>(1.0 - yy - zz);
    m[1]  = static_cast<float>(xy + wz);
    m[2]  = static_cast<float>(xz - wy);
    m[3]  = static_cast<float>(0.0);
    m[4]  = static_cast<float>(xy - wz);
    m[5]  = static_cast<float>(1.0 - xx - zz);
    m[6]  = static_cast<float>(yz + wx);
    m[7]  = static_cast<float>(0.0);
    m[8]  = static_cast<float>(xz + wy);
    m[9]  = static_cast<float>(yz - wx);
    m[10] = static_cast<float>(1.0 - xx - yy);
    m[11] = static_cast<float>(0.0);
    m[12] = static_cast<float>(m_positionAbs.getX());
    m[13] = static_cast<float>(m_positionAbs.getY());
    m[14] = static_cast<float>(m_positionAbs.getZ());
    m[15] = static_cast<float>(1.0);
}

void Entity::applyTranslationToChildren() {
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(child.m_positionRel);
    }
}

void Entity::applyOrientationToChildren() {
    // Qc1 = Qp1 * Inv(Qp0) * Qc0
    // Where:
    // Qp0 = Parent orientation last frame
    // Qp1 = Parent orientation this frame
    // Qc0 = Child orientation last frame
    // Qc1 = Child orientation this frame
    Quaternion relativeRotation = m_orientation * m_lastOrientation.inverse();
    set<Entity*>::iterator it, itend;
    itend = m_children.end();
    for (it = m_children.begin(); it != itend; ++it) {
        Entity& child = **it;
        child.setPositionRel(child.m_positionRel.rotate(relativeRotation));
        child.setOrientation(relativeRotation * child.m_orientation);
    }
}

Entity* Entity::addChild(const string& childName) {
    Entity* child = new Entity(this, childName);
    m_children.insert(child);
    SceneManager::ms_entities.insert(pair<string, Entity*>(childName, child));
    return child;
}

void Entity::removeChild(Entity* const child) {
    map<string, Entity*>::iterator smIt = SceneManager::ms_entities.find(child->getObjectName());
    if (smIt != SceneManager::ms_entities.end())
        SceneManager::ms_entities.erase(smIt);

    set<Entity*>::iterator it = m_children.find(child);
    if (it != m_children.end()) {
        delete *it;
        m_children.erase(it);
    }
}

string Entity::treeToString(const size_t indent) const {
    stringstream ss;
    for (size_t i = 0; i < indent; ++i)
        ss << " ";
    ss << m_objectName << endl;
    set<Entity*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it)
        ss << (*it)->treeToString(indent + INDENT_SIZE);
    return ss.str();
}

void Entity::setPosition(const string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    setPositionAbs(x, y, z);
}

void Entity::moveXYZ(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(x, y, z);
}

void Entity::moveX(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime());
}

void Entity::moveY(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime());
}

void Entity::moveZ(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime());
}

void Entity::moveXYZ_parent(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(
        x * DeviceManager::getDeltaTime(),
        y * DeviceManager::getDeltaTime(),
        z * DeviceManager::getDeltaTime(),
        SPACE_PARENT
    );
}

void Entity::moveX_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveY_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveZ_parent(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::moveXYZ_global(const std::string& arg) {
    float x, y, z;
    stringstream ss(arg);
    ss >> x >> y >> z;
    translate(
        x * DeviceManager::getDeltaTime(),
        y * DeviceManager::getDeltaTime(),
        z * DeviceManager::getDeltaTime(),
        SPACE_GLOBAL
    );
}

void Entity::moveX_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateX(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::moveY_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateY(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::moveZ_global(const std::string& arg) {
    float dist;
    stringstream ss(arg);
    ss >> dist;
    translateZ(dist * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::yaw(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime());
}

void Entity::pitch(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime());
}

void Entity::roll(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime());
}

void Entity::yaw_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::pitch_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::roll_parent(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime(), SPACE_PARENT);
}

void Entity::yaw_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    yaw(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::pitch_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    pitch(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

void Entity::roll_global(const std::string& arg) {
    float radians;
    stringstream ss(arg);
    ss >> radians;
    roll(radians * DeviceManager::getDeltaTime(), SPACE_GLOBAL);
}

ostream& operator<<(ostream& out, const Entity& rhs) {
    out << "position(" << rhs.getPositionAbs().getX() << ", " <<
            rhs.getPositionAbs().getY() << ", " <<
            rhs.getPositionAbs().getZ() << ")" << endl;

    out << "rotation(" << rhs.getOrientation().getW() << ", " <<
            rhs.getOrientation().getX() << ", " <<
            rhs.getOrientation().getY() << ", " <<
            rhs.getOrientation().getZ() << ")" << endl;

    for (size_t i = 0; i < rhs.m_components.size(); ++i) {
        if (rhs.m_components[i] != 0)
            out << "    " << *rhs.m_components[i] << endl;
    }
    return out;
}
