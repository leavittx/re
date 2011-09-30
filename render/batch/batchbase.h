#pragma once

namespace render {

////////////////////////////////////////////////////////////////////
// This base class is a pure virtual class with one single virtual
// function, Draw(). The GLBegin class and GLTriangleBatch classes
// are derived from this. Having a virtual Draw() function allows
// these classes to be collected by container classes that can
// then iterate over them and call their draw methods.
class GLBatchBase
{
public:
	virtual void Draw(void) = 0;
};

} // end of namespace 'render'
