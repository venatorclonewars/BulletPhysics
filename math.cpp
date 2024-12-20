#include "math.h"

void Matrix4f::initCameraTransform(const Vector3f& pos, const Vector3f& target, const Vector3f& up)
{
    Matrix4f cameraTranslation;
    cameraTranslation.initTranslationTransform(-pos.x, -pos.y, -pos.z);

    Matrix4f CameraRotateTrans;
    CameraRotateTrans.initCameraTransform(target, up);

    *this = CameraRotateTrans * cameraTranslation;
}

void Matrix4f::initTranslationTransform(float x, float y, float z)
{
    mat[0][0] = 1.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f; mat[0][3] = x;
    mat[1][0] = 0.0f; mat[1][1] = 1.0f; mat[1][2] = 0.0f; mat[1][3] = y;
    mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 1.0f; mat[2][3] = z;
    mat[3][0] = 0.0f; mat[3][1] = 0.0f; mat[3][2] = 0.0f; mat[3][3] = 1.0f;
}

void Matrix4f::initCameraTransform(const Vector3f& target, const Vector3f& up)
{
    Vector3f N = target;
    N.normalize();

    Vector3f upNorm = up;
    upNorm.normalize();

    Vector3f U;
    U = upNorm.cross(N);
    U.normalize();

    Vector3f V = N.cross(U);

    mat[0][0] = U.x;   mat[0][1] = U.y;   mat[0][2] = U.z;   mat[0][3] = 0.0f;
    mat[1][0] = V.x;   mat[1][1] = V.y;   mat[1][2] = V.z;   mat[1][3] = 0.0f;
    mat[2][0] = N.x;   mat[2][1] = N.y;   mat[2][2] = N.z;   mat[2][3] = 0.0f;
    mat[3][0] = 0.0f;  mat[3][1] = 0.0f;  mat[3][2] = 0.0f;  mat[3][3] = 1.0f;
}

void Matrix4f::calcClipPlanes(Vector4f& l, Vector4f& r, Vector4f& b, Vector4f& t, Vector4f& n, Vector4f& f) const
{
    Vector4f row1(mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    Vector4f row2(mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    Vector4f row3(mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    Vector4f row4(mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

    l = row1 + row4;
    r = row1 - row4;
    b = row2 + row4;
    t = row2 - row4;
    n = row3 + row4;
    f = row3 - row4;
    
}
