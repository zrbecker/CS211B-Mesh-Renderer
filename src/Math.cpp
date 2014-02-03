
#include "Math.h"

#include <cmath>

#ifndef M_PI
#define M_PI   3.14159265358979323846264338327950288f
#endif

namespace gl
{
    //
    // Vector2 Implementation
    //

    Vector2::Vector2()
    {
        m_values[0] = 0;
        m_values[1] = 0;
    }

    Vector2::Vector2(float x, float y)
    {
        m_values[0] = x;
        m_values[1] = y;
    }

    Vector2::Vector2(const Vector3& v)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
    }

    Vector2::Vector2(const Vector4& v)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
    }

    float& Vector2::operator[](size_t index)
    {
        return m_values[index];
    }

    const float& Vector2::operator[](size_t index) const
    {
        return m_values[index];
    }

    Vector2& Vector2::operator*=(float s)
    {
        m_values[0] *= s;
        m_values[1] *= s;
        return (*this);
    }

    Vector2& Vector2::operator/=(float s)
    {
        m_values[0] /= s;
        m_values[1] /= s;
        return (*this);
    }

    float Vector2::length() const
    {
        return sqrt(m_values[0] * m_values[0] + 
                    m_values[1] * m_values[1]);
    }

    //
    // Vector3 Implementation
    //

    Vector3::Vector3()
    {
        m_values[0] = 0;
        m_values[1] = 0;
    }

    Vector3::Vector3(float x, float y, float z)
    {
        m_values[0] = x;
        m_values[1] = y;
        m_values[2] = z;
    }

    Vector3::Vector3(const Vector2& v, float z)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
        m_values[2] = z;
    }

    Vector3::Vector3(const Vector4& v)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
        m_values[2] = v[2];
    }

    float& Vector3::operator[](size_t index)
    {
        return m_values[index];
    }

    const float& Vector3::operator[](size_t index) const
    {
        return m_values[index];
    }

    Vector3& Vector3::operator*=(float s)
    {
        m_values[0] *= s;
        m_values[1] *= s;
        m_values[2] *= s;
        return (*this);
    }

    Vector3& Vector3::operator/=(float s)
    {
        m_values[0] /= s;
        m_values[1] /= s;
        m_values[2] /= s;
        return (*this);
    }

    float Vector3::length() const
    {
        return sqrt(m_values[0] * m_values[0] +
                    m_values[1] * m_values[1] +
                    m_values[2] * m_values[2]);
    }

    //
    // Vector4 Implementation
    //

    Vector4::Vector4()
    {
        m_values[0] = 0;
        m_values[1] = 0;
    }

    Vector4::Vector4(float x, float y, float z, float w)
    {
        m_values[0] = x;
        m_values[1] = y;
        m_values[2] = z;
        m_values[3] = w;
    }

    Vector4::Vector4(const Vector2& v, float z, float w)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
        m_values[2] = z;
        m_values[3] = w;
    }

    Vector4::Vector4(const Vector3& v, float w)
    {
        m_values[0] = v[0];
        m_values[1] = v[1];
        m_values[2] = v[2];
        m_values[3] = w;
    }

    float& Vector4::operator[](size_t index)
    {
        return m_values[index];
    }

    const float& Vector4::operator[](size_t index) const
    {
        return m_values[index];
    }

    Vector4& Vector4::operator*=(float s)
    {
        m_values[0] *= s;
        m_values[1] *= s;
        m_values[2] *= s;
        m_values[3] *= s;
        return (*this);
    }

    Vector4& Vector4::operator/=(float s)
    {
        m_values[0] /= s;
        m_values[1] /= s;
        m_values[2] /= s;
        m_values[3] /= s;
        return (*this);
    }

    float Vector4::length() const
    {
        return sqrt(m_values[0] * m_values[0] +
                    m_values[1] * m_values[1] +
                    m_values[2] * m_values[2] +
                    m_values[3] * m_values[3]);
    }

    //
    // Matrix4 implementation
    //

    Matrix4::Matrix4()
    {
        m_columns[0] = Vector4(1, 0, 0, 0);
        m_columns[1] = Vector4(0, 1, 0, 0);
        m_columns[2] = Vector4(0, 0, 1, 0);
        m_columns[3] = Vector4(0, 0, 0, 1);
    }

    Matrix4::Matrix4(const Vector4& column1,
            const Vector4& column2,
            const Vector4& column3,
            const Vector4& column4)
    {
        m_columns[0] = column1;
        m_columns[1] = column2;
        m_columns[2] = column3;
        m_columns[3] = column4;
    }

    Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33)
    {
        m_columns[0] = Vector4(m00, m01, m02, m03);
        m_columns[1] = Vector4(m10, m11, m12, m13);
        m_columns[2] = Vector4(m20, m21, m22, m23);
        m_columns[3] = Vector4(m30, m31, m32, m33);
    }

    Vector4& Matrix4::operator[](size_t index)
    {
        return m_columns[index];
    }

    const Vector4& Matrix4::operator[](size_t index) const
    {
        return m_columns[index];
    }

    Matrix4& Matrix4::operator*=(const Matrix4& other)
    {
        Matrix4 result = (*this) * other;
        return (*this = result);
    }

    Matrix4& Matrix4::operator*=(float s)
    {
        m_columns[0] *= s;
        m_columns[1] *= s;
        m_columns[2] *= s;
        return (*this);
    }

    Matrix4& Matrix4::operator/=(float s)
    {
        m_columns[0] /= s;
        m_columns[1] /= s;
        m_columns[2] /= s;
        return (*this);
    }

    void Matrix4::transpose()
    {
        Matrix4& M = (*this);
        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < col; ++row)
                std::swap(M[col][row], M[row][col]);
        }
    }

    void Matrix4::loadIdentity()
    {
        m_columns[0] = Vector4(1, 0, 0, 0);
        m_columns[1] = Vector4(0, 1, 0, 0);
        m_columns[2] = Vector4(0, 0, 1, 0);
        m_columns[3] = Vector4(0, 0, 0, 1);
    }

    void Matrix4::translate(float x, float y, float z)
    {
        Matrix4& M = (*this);
        M[3][0] = x * M[0][0] + y * M[1][0] + z * M[2][0] + M[3][0];
        M[3][1] = x * M[0][1] + y * M[1][1] + z * M[2][1] + M[3][1];
        M[3][2] = x * M[0][2] + y * M[1][2] + z * M[2][2] + M[3][2];
        M[3][3] = x * M[0][3] + y * M[1][3] + z * M[2][3] + M[3][3];
    }

    void Matrix4::scale(float x, float y, float z)
    {
        m_columns[0] *= x;
        m_columns[1] *= y;
        m_columns[2] *= z;
    }

    void Matrix4::rotate(float angle, float x, float y, float z)
    {
        float cosine = cosf(angle * M_PI / 180);
        float sine = sinf(angle * M_PI / 180);

        float len = sqrt(x * x + y * y + z * z);
        x /= len;
        y /= len;
        z /= len;

        Matrix4 R;

        R[0][0] = cosine + x * x * (1 - cosine);
        R[0][1] = z *   sine + y * x * (1 - cosine);
        R[0][2] = -y *   sine + z * x * (1 - cosine);
        R[0][3] = 0.0f;

        R[1][0] = -z *   sine + x * y * (1 - cosine);
        R[1][1] = cosine + y * y * (1 - cosine);
        R[1][2] = x *   sine + z * y * (1 - cosine);
        R[1][3] = 0.0f;

        R[2][0] = y *   sine + x * z * (1 - cosine);
        R[2][1] = -x *   sine + y * z * (1 - cosine);
        R[2][2] = cosine + z * z * (1 - cosine);
        R[2][3] = 0.0f;

        R[3][0] = 0.0f;
        R[3][1] = 0.0f;
        R[3][2] = 0.0f;
        R[3][3] = 1.0f;

        Matrix4& M = (*this);
        M *= R;
    }

    void Matrix4::lookAt(float eyeX, float eyeY, float eyeZ,
                         float centerX, float centerY, float centerZ,
                         float upX, float upY, float upZ)
    {
        Vector3 forward(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
        forward /= forward.length();

        Vector3 side = cross(forward, Vector3(upX, upY, upZ));
        side /= side.length();

        Vector3 up = cross(side, forward);

        Matrix4 L;
        L[0] = Vector4(side, 0);
        L[1] = Vector4(up, 0);
        L[2] = Vector4(-forward, 0);
        L.transpose();

        Matrix4& M = (*this);
        M *= L;
        M.translate(-eyeX, -eyeY, -eyeZ);
    }

    void Matrix4::translate(Vector3 v)
    {
        translate(v[0], v[1], v[2]);
    }

    void Matrix4::scale(Vector3 v)
    {
        scale(v[0], v[1], v[2]);
    }

    void Matrix4::rotate(float angle, Vector3 v)
    {
        rotate(angle, v[0], v[1], v[2]);
    }

    void Matrix4::lookAt(Vector3 eye, Vector3 center, Vector3 up)
    {
        lookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    }

    void Matrix4::frustum(float l, float r, float b, float t, float n, float f)
    {
        Matrix4 F;

        F[0][0] = 2 * n / (r - l); F[1][0] = 0;               F[2][0] = (r + l) / (r - l);  F[3][0] = 0;
        F[0][1] = 0;               F[1][1] = 2 * n / (t - b); F[2][1] = (t + b) / (t - b);  F[3][1] = 0;
        F[0][2] = 0;               F[1][2] = 0;               F[2][2] = -(f + n) / (f - n); F[3][2] = -2 * f * n / (f - n);
        F[0][3] = 0;               F[1][3] = 0;               F[2][3] = -1;                 F[3][3] = 0;

        Matrix4& M = (*this);
        M *= F;
    }

    void Matrix4::prespective(float angle, float aspect, float near, float far)
    {
        float t = near * tan((angle / 2) * M_PI / 180);
        this->frustum(-aspect * t, aspect * t, -t, t, near, far);
    }

    //
    // Matrix4Stack implementation
    //

    Matrix4Stack::Matrix4Stack()
        : m_stack()
    {
        m_stack.push(Matrix4());
    }

    const Matrix4& Matrix4Stack::top() const
    {
        return m_stack.top();
    }

    void Matrix4Stack::push()
    {
        m_stack.push(m_stack.top());
    }

    void Matrix4Stack::pop()
    {
        m_stack.pop();
        if (m_stack.size() == 0)
            m_stack.push(Matrix4()); // Always have at least 1 matrix
    }

    void Matrix4Stack::loadIdentity()
    {
        m_stack.top().loadIdentity();
    }

    void Matrix4Stack::translate(float x, float y, float z)
    {
        m_stack.top().translate(x, y, z);
    }

    void Matrix4Stack::scale(float x, float y, float z)
    {
        m_stack.top().scale(x, y, z);
    }

    void Matrix4Stack::rotate(float angle, float x, float y, float z)
    {
        m_stack.top().rotate(angle, x, y, z);
    }

    void Matrix4Stack::lookAt(float eyeX, float eyeY, float eyeZ,
                              float centerX, float centerY, float centerZ,
                              float upX, float upY, float upZ)
    {
        m_stack.top().lookAt(eyeX, eyeY, eyeZ,
                             centerX, centerY, centerZ,
                             upX, upY, upZ);
    }

    void Matrix4Stack::translate(Vector3 v)
    {
        translate(v[0], v[1], v[2]);
    }

    void Matrix4Stack::scale(Vector3 v)
    {
        scale(v[0], v[1], v[2]);
    }

    void Matrix4Stack::rotate(float angle, Vector3 v)
    {
        rotate(angle, v[0], v[1], v[2]);
    }

    void Matrix4Stack::lookAt(Vector3 eye, Vector3 center, Vector3 up)
    {
        lookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    }

    void Matrix4Stack::frustum(float l, float r, float b, float t, float n, float f)
    {
        m_stack.top().frustum(l, r, b, t, n, f);
    }

    void Matrix4Stack::prespective(float angle, float aspect, float near, float far)
    {
        m_stack.top().prespective(angle, aspect, near, far);
    }

    //
    // Vector operations
    //

    float dot(const Vector2& v1, const Vector2& v2)
    {
        return v1[0] * v2[0] + v1[1] * v2[1];
    }

    float dot(const Vector3& v1, const Vector3& v2)
    {
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    }

    float dot(const Vector4& v1, const Vector4& v2)
    {
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
    }

    Vector3 cross(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1[1] * v2[2] - v1[2] * v2[1],
                       v1[2] * v2[0] - v1[0] * v2[2],
                       v1[0] * v2[1] - v1[1] * v2[0]);
    }

    Vector2 operator*(const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1[0] * v2[0],
                       v1[1] * v2[1]);
    }

    Vector3 operator*(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1[0] * v2[0],
                       v1[1] * v2[1],
                       v1[2] * v2[2]);
    }

    Vector4 operator*(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1[0] * v2[0],
                       v1[1] * v2[1],
                       v1[2] * v2[2],
                       v1[3] * v2[3]);
    }

    Vector2 operator+(const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1[0] + v2[0],
                       v1[1] + v2[1]);
    }

    Vector3 operator+(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1[0] + v2[0],
                       v1[1] + v2[1],
                       v1[2] + v2[2]);
    }

    Vector4 operator+(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1[0] + v2[0],
                       v1[1] + v2[1],
                       v1[2] + v2[2],
                       v1[3] + v2[3]);
    }

    Vector2 operator-(const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1[0] - v2[0],
                       v1[1] - v2[1]);
    }

    Vector3 operator-(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1[0] - v2[0],
                       v1[1] - v2[1],
                       v1[2] - v2[2]);
    }

    Vector4 operator-(const Vector4& v1, const Vector4& v2)
    {
        return Vector4(v1[0] - v2[0],
                       v1[1] - v2[1],
                       v1[2] - v2[2],
                       v1[3] - v2[3]);
    }

    Vector2 operator*(float s, const Vector2& v)
    {
        return Vector2(s * v[0],
                       s * v[1]);
    }

    Vector3 operator*(float s, const Vector3& v)
    {
        return Vector3(s * v[0],
                       s * v[1],
                       s * v[2]);
    }

    Vector4 operator*(float s, const Vector4& v)
    {
        return Vector4(s * v[0],
                       s * v[1],
                       s * v[2],
                       s * v[3]);
    }

    Vector2 operator*(const Vector2& v, float s)
    {
        return Vector2(s * v[0],
                       s * v[1]);
    }

    Vector3 operator*(const Vector3& v, float s)
    {
        return Vector3(s * v[0],
                       s * v[1],
                       s * v[2]);
    }

    Vector4 operator*(const Vector4& v, float s)
    {
        return Vector4(s * v[0],
                       s * v[1],
                       s * v[2],
                       s * v[3]);
    }

    Vector2 operator/(const Vector2& v, float s)
    {
        return Vector2(v[0] / s,
                       v[1] / s);
    }

    Vector3 operator/(const Vector3& v, float s)
    {
        return Vector3(v[0] / s,
                       v[1] / s,
                       v[2] / s);
    }

    Vector4 operator/(const Vector4& v, float s)
    {
        return Vector4(v[0] / s,
                       v[1] / s,
                       v[2] / s,
                       v[3] / s);
    }

    Vector2 operator-(const Vector2& v)
    {
        return -1 * v;
    }

    Vector3 operator-(const Vector3& v)
    {
        return -1 * v;
    }

    Vector4 operator-(const Vector4& v)
    {
        return -1 * v;
    }

    //
    // Matrix operations
    //

    Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
    {
        Matrix4 result;
        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                result[col][row] = 0;
                for (int k = 0; k < 4; ++k)
                    result[col][row] += m1[k][row] * m2[col][k];
            }
        }
        return result;
    }

    Vector4 operator*(const Vector4& v, const Matrix4& m)
    {
        Vector4 result;
        for (int k = 0; k < 4; ++k)
            result[k] = m[k][0] * v[0] + m[k][1] * v[1] + m[k][2] * v[2] + m[k][3] * v[3];
        return result;
    }

    Vector4 operator*(const Matrix4& m, const Vector4& v)
    {
        Vector4 result;
        for (int k = 0; k < 4; ++k)
            result[k] = m[0][k] * v[0] + m[1][k] * v[1] + m[2][k] * v[2] + m[3][k] * v[3];
        return result;
    }

    Matrix4 operator*(float s, const Matrix4& m)
    {
        Matrix4 result = m;
        result *= s;
        return result;
    }

    Matrix4 operator*(const Matrix4& m, float s)
    {
        Matrix4 result = m;
        result *= s;
        return result;
    }

    Matrix4 operator/(const Matrix4& m, float s)
    {
        Matrix4 result = m;
        result /= s;
        return result;
    }

    Matrix4 operator-(const Matrix4& m)
    {
        return -1 * m;
    }
}
