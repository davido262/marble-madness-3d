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


#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include "scalar.hpp"

class Quaternion;

class Vector3 {
public:
    Vector3();
    Vector3(const Vector3& v);
    Vector3(const scalar_t& x, const scalar_t& y, const scalar_t& z);
    Vector3(const scalar_t& num);

    Vector3& operator=(const Vector3& v);
    Vector3 operator-() const;
    Vector3& operator+=(const Vector3& v);
    Vector3& operator+=(const scalar_t& s);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator-=(const scalar_t& s);
    Vector3& operator*=(const Vector3& v);
    Vector3& operator*=(const scalar_t& s);
    Vector3& operator/=(const Vector3& v);
    Vector3& operator/=(const scalar_t& s);
    const Vector3 operator+(const Vector3& v) const;
    const Vector3 operator+(const scalar_t& s) const;
    const Vector3 operator-(const Vector3& v) const;
    const Vector3 operator-(const scalar_t& s) const;
    const Vector3 operator*(const Vector3& v) const;
    const Vector3 operator*(const scalar_t& s) const;
    const Vector3 operator/(const Vector3& v) const;
    const Vector3 operator/(const scalar_t& s) const;
    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;

    const scalar_t& get(const int i) const;
    const scalar_t& getX() const;
    const scalar_t& getY() const;
    const scalar_t& getZ() const;
    void setX(const scalar_t& x);
    void setY(const scalar_t& y);
    void setZ(const scalar_t& z);
    void setValue(const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setValue(const Vector3& v);
    void setValue(const scalar_t& num);

    scalar_t length() const;
    scalar_t lengthSquared() const;
    scalar_t distance(const Vector3& v) const;
    scalar_t distanceSquared(const Vector3& v) const;
    scalar_t angle(const Vector3& v) const;
    scalar_t dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;
    Vector3& normalize();
    Vector3 normalized() const;
    Vector3 rotate(const Vector3& axis, const scalar_t& angle) const;
    Vector3 rotate(const Quaternion& q) const;
    Vector3 lerp(const Vector3& v, const scalar_t& t) const;

private:
    scalar_t m_data[3];
};

const Vector3 VECTOR3_ZERO = Vector3(0);
const Vector3 VECTOR3_UNIT = Vector3(1);
const Vector3 VECTOR3_UNIT_NEG = Vector3(-1);
const Vector3 VECTOR3_UNIT_X = Vector3(1, 0, 0);
const Vector3 VECTOR3_UNIT_Y = Vector3(0, 1, 0);
const Vector3 VECTOR3_UNIT_Z = Vector3(0, 0, 1);
const Vector3 VECTOR3_UNIT_X_NEG = Vector3(-1, 0, 0);
const Vector3 VECTOR3_UNIT_Y_NEG = Vector3(0, -1, 0);
const Vector3 VECTOR3_UNIT_Z_NEG = Vector3(0, 0, -1);

#include "quaternion.hpp"



inline Vector3::Vector3() {
}

inline Vector3::Vector3(const Vector3& v) {
    setValue(v.getX(), v.getY(), v.getZ());
}

inline Vector3::Vector3(const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setValue(x, y, z);
}

inline Vector3::Vector3(const scalar_t& num) {
    setValue(num);
}



inline Vector3& Vector3::operator=(const Vector3& v) {
    setValue(v.getX(), v.getY(), v.getZ());
    return *this;
}

inline Vector3 Vector3::operator-() const {
    return Vector3(-getX(), -getY(), -getZ());
}

inline Vector3& Vector3::operator+=(const Vector3& v) {
    m_data[0] += v.m_data[0];
    m_data[1] += v.m_data[1];
    m_data[2] += v.m_data[2];
    return *this;
}

inline Vector3& Vector3::operator+=(const scalar_t& s) {
    m_data[0] += s;
    m_data[1] += s;
    m_data[2] += s;
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v) {
    m_data[0] -= v.m_data[0];
    m_data[1] -= v.m_data[1];
    m_data[2] -= v.m_data[2];
    return *this;
}

inline Vector3& Vector3::operator-=(const scalar_t& s) {
    return *this += -s;
}

inline Vector3& Vector3::operator*=(const Vector3& v) {
    m_data[0] *= v.m_data[0];
    m_data[1] *= v.m_data[1];
    m_data[2] *= v.m_data[2];
    return *this;
}

inline Vector3& Vector3::operator*=(const scalar_t& s) {
    m_data[0] *= s;
    m_data[1] *= s;
    m_data[2] *= s;
    return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& v) {
    m_data[0] /= v.m_data[0];
    m_data[1] /= v.m_data[1];
    m_data[2] /= v.m_data[2];
    return *this;
}

inline Vector3& Vector3::operator/=(const scalar_t& s) {
    return *this *= ONE / s;
}

inline const Vector3 Vector3::operator+(const Vector3& v) const {
    return Vector3(*this) += v;
}

inline const Vector3 Vector3::operator+(const scalar_t& s) const {
    return Vector3(*this) += s;
}

inline const Vector3 Vector3::operator-(const Vector3& v) const {
    return Vector3(*this) -= v;
}

inline const Vector3 Vector3::operator-(const scalar_t& s) const {
    return Vector3(*this) -= s;
}

inline const Vector3 Vector3::operator*(const Vector3& v) const {
    return Vector3(*this) *= v;
}

inline const Vector3 Vector3::operator*(const scalar_t& s) const {
    return Vector3(*this) *= s;
}

inline const Vector3 Vector3::operator/(const Vector3& v) const {
    return Vector3(*this) /= v;
}

inline const Vector3 Vector3::operator/(const scalar_t& s) const {
    return Vector3(*this) /= s;
}

inline bool Vector3::operator==(const Vector3& v) const {
    return getX() == v.getX() &&
           getY() == v.getY() &&
           getZ() == v.getZ();
}

inline bool Vector3::operator!=(const Vector3& v) const {
    return !(*this == v);
}



inline const scalar_t& Vector3::get(const int i) const {
    return m_data[i];
}

inline const scalar_t& Vector3::getX() const {
    return m_data[0];
}

inline const scalar_t& Vector3::getY() const {
    return m_data[1];
}

inline const scalar_t& Vector3::getZ() const {
    return m_data[2];
}

inline void Vector3::setX(const scalar_t& x) {
    m_data[0] = x;
}

inline void Vector3::setY(const scalar_t& y) {
    m_data[1] = y;
}

inline void Vector3::setZ(const scalar_t& z) {
    m_data[2] = z;
}

inline void Vector3::setValue(const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setX(x);
    setY(y);
    setZ(z);
}

inline void Vector3::setValue(const Vector3& v) {
    setValue(v.getX(), v.getY(), v.getZ());
}

inline void Vector3::setValue(const scalar_t& num) {
    setValue(num, num, num);
}



inline scalar_t Vector3::length() const {
    return std::sqrt(lengthSquared());
}

inline scalar_t Vector3::lengthSquared() const {
    return dot(*this);
}

inline scalar_t Vector3::distance(const Vector3& v) const {
    return (v - *this).length();
}

inline scalar_t Vector3::distanceSquared(const Vector3& v) const {
    return (v - *this).lengthSquared();
}

inline scalar_t Vector3::angle(const Vector3& v) const {
    const scalar_t s = std::sqrt(lengthSquared() * v.lengthSquared());
    return std::acos(dot(v) / s);
}

inline scalar_t Vector3::dot(const Vector3& v) const {
    return getX() * v.getX() +
           getY() * v.getY() +
           getZ() * v.getZ();
}

inline Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(
        getY() * v.getZ() - getZ() * v.getY(),
        getZ() * v.getX() - getX() * v.getZ(),
        getX() * v.getY() - getY() * v.getX()
    );
}

inline Vector3& Vector3::normalize() {
    return *this /= length();
}

inline Vector3 Vector3::normalized() const {
    return *this / length();
}

inline Vector3 Vector3::rotate(const Vector3& axis, const scalar_t& angle) const {
    Vector3 o = axis * axis.dot(*this);
    Vector3 x = *this - o;
    Vector3 y = axis.cross(*this);
    return o + x * std::cos(angle) + y * std::sin(angle);
}

inline Vector3 Vector3::rotate(const Quaternion& q) const {
    // nVidia SDK implementation
    Vector3 uv, uuv;
    const Vector3 qvec(q.getX(), q.getY(), q.getZ());
    uv = qvec.cross(*this);
    uuv = qvec.cross(uv);
    uv *= (2.0f * q.getW());
    uuv *= 2.0f;
    return *this + uv + uuv;
}

inline Vector3 Vector3::lerp(const Vector3& v, const scalar_t& t) const {
    return Vector3(
        getX() + (v.getX() - getX()) * t,
        getY() + (v.getY() - getY()) * t,
        getZ() + (v.getZ() - getZ()) * t
    );

}

#endif // VECTOR3_HPP
