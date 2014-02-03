
#ifndef GL_MATH_H
#define GL_MATH_H

#include <stack>
#include <cstddef>

#define RADIANS(x) ((x) * 3.1415 / 180)

namespace gl
{
    class Vector3;
    class Vector4;

    class Vector2
    {
    public:
        Vector2();
        Vector2(float x, float y);
        explicit Vector2(const Vector3& v);
        explicit Vector2(const Vector4& v);

        float& operator[](size_t index);
        const float& operator[](size_t index) const;

        Vector2& operator*=(float s);
        Vector2& operator/=(float s);

        float length() const;

    private:
        float m_values[2];
    };

    class Vector3
    {
    public:
        Vector3();
        Vector3(float x, float y, float z);
        Vector3(const Vector2& v, float z);
        explicit Vector3(const Vector4& v);

        float& operator[](size_t index);
        const float& operator[](size_t index) const;

        Vector3& operator*=(float s);
        Vector3& operator/=(float s);

        float length() const;

    private:
        float m_values[3];
    };

    class Vector4
    {
    public:
        Vector4();
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector2& v, float z, float w);
        Vector4(const Vector3& v, float w);

        float& operator[](size_t index);
        const float& operator[](size_t index) const;

        Vector4& operator*=(float s);
        Vector4& operator/=(float s);

        float length() const;

    private:
        float m_values[4];
    };

    class Matrix4
    {
    public:
        Matrix4();
        Matrix4(const Vector4& column1,
                const Vector4& column2,
                const Vector4& column3,
                const Vector4& column4);
        Matrix4(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33);

        Vector4& operator[](size_t index);
        const Vector4& operator[](size_t index) const;
        Matrix4& operator*=(const Matrix4& other);
        Matrix4& operator*=(float s);
        Matrix4& operator/=(float s);

        void transpose();

        void loadIdentity();

        void translate(float x, float y, float z);
        void scale(float x, float y, float z);
        void rotate(float angle, float x, float y, float z);
        void lookAt(float eyeX, float eyeY, float eyeZ,
                    float centerX, float centerY, float centerZ,
                    float upX, float upY, float upZ);

        void translate(Vector3 v);
        void scale(Vector3 v);
        void rotate(float angle, Vector3 v);
        void lookAt(Vector3 eye, Vector3 center, Vector3 up);

        void frustum(float l, float r, float b, float t, float n, float f);
        void prespective(float angle, float aspect, float near, float far);

    private:
        Vector4 m_columns[4];
    };

    class Matrix4Stack
    {
    public:
        Matrix4Stack();

        const Matrix4& top() const;

        void push();
        void pop();

        void loadIdentity();

        void translate(float x, float y, float z);
        void scale(float x, float y, float z);
        void rotate(float angle, float x, float y, float z);
        void lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

        void translate(Vector3 v);
        void scale(Vector3 v);
        void rotate(float angle, Vector3 v);
        void lookAt(Vector3 eye, Vector3 center, Vector3 up);

        void frustum(float l, float r, float b, float t, float n, float f);
        void prespective(float angle, float aspect, float near, float far);

    private:
        std::stack<Matrix4> m_stack;
    };

    // Vector Operations
    float dot(const Vector2& v1, const Vector2& v2);
    float dot(const Vector3& v1, const Vector3& v2);
    float dot(const Vector4& v1, const Vector4& v2);

    Vector3 cross(const Vector3& v1, const Vector3& v2);

    Vector2 operator*(const Vector2& v1, const Vector2& v2);
    Vector3 operator*(const Vector3& v1, const Vector3& v2);
    Vector4 operator*(const Vector4& v1, const Vector4& v2);

    Vector2 operator+(const Vector2& v1, const Vector2& v2);
    Vector3 operator+(const Vector3& v1, const Vector3& v2);
    Vector4 operator+(const Vector4& v1, const Vector4& v2);

    Vector2 operator-(const Vector2& v1, const Vector2& v2);
    Vector3 operator-(const Vector3& v1, const Vector3& v2);
    Vector4 operator-(const Vector4& v1, const Vector4& v2);

    Vector2 operator*(float s, const Vector2& v);
    Vector3 operator*(float s, const Vector3& v);
    Vector4 operator*(float s, const Vector4& v);

    Vector2 operator*(const Vector2& v, float s);
    Vector3 operator*(const Vector3& v, float s);
    Vector4 operator*(const Vector4& v, float s);

    Vector2 operator/(const Vector2& v, float s);
    Vector3 operator/(const Vector3& v, float s);
    Vector4 operator/(const Vector4& v, float s);

    Vector2 operator-(const Vector2& v);
    Vector3 operator-(const Vector3& v);
    Vector4 operator-(const Vector4& v);

    // Matrix operations
    Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);

    Vector4 operator*(const Vector4& v, const Matrix4& m);
    Vector4 operator*(const Matrix4& m, const Vector4& v);

    Matrix4 operator*(float s, const Matrix4& m);
    Matrix4 operator*(const Matrix4& m, float s);
    Matrix4 operator/(const Matrix4& m, float s);

    Matrix4 operator-(const Matrix4& m);
}

#endif
