#pragma once

#include <GLTools.h>

class GLGeometryTransform
{
public:
	GLGeometryTransform(void) {}

	inline void SetModelViewMatrixStack(GLMatrixStack& mModelView) { _mModelView = &mModelView; }

	inline void SetProjectionMatrixStack(GLMatrixStack& mProjection) { _mProjection = &mProjection; }

	inline void SetMatrixStacks(GLMatrixStack& mModelView, GLMatrixStack& mProjection) {
		_mModelView = &mModelView;
		_mProjection = &mProjection;
	}

	const M3DMatrix44f& GetModelViewProjectionMatrix(void)
	{
		m3dMatrixMultiply44(_mModelViewProjection, _mProjection->GetMatrix(), _mModelView->GetMatrix());
		return _mModelViewProjection;
	}

	inline const M3DMatrix44f& GetModelViewMatrix(void) { return _mModelView->GetMatrix(); }
	inline const M3DMatrix44f& GetProjectionMatrix(void) { return _mProjection->GetMatrix(); }

	const M3DMatrix33f& GetNormalMatrix(bool bNormalize = false)
	{
		m3dExtractRotationMatrix33(_mNormalMatrix, GetModelViewMatrix());

		if (bNormalize) {
			m3dNormalizeVector3(&_mNormalMatrix[0]);
			m3dNormalizeVector3(&_mNormalMatrix[3]);
			m3dNormalizeVector3(&_mNormalMatrix[6]);
		}

		return _mNormalMatrix;
	}

protected:
	M3DMatrix44f	_mModelViewProjection;
	M3DMatrix33f	_mNormalMatrix;

	GLMatrixStack*  _mModelView;
	GLMatrixStack* _mProjection;
};
