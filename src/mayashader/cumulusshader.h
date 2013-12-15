#include <maya/MPxHwShaderNode.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MImage.h>

class CumulusShader : public MPxHwShaderNode 
{
public:
	// MPxNode methods
    static void *creator();
    static MStatus initialize();

    // MPxNode virtuals
    virtual void    postConstructor();
    virtual MStatus compute(const MPlug &plug, MDataBlock &data);
    virtual bool    getInternalValueInContext(const MPlug &plug,       MDataHandle &handle, MDGContext &);
    virtual bool    setInternalValueInContext(const MPlug &plug, const MDataHandle &handle, MDGContext &);

    // MPxHwShaderNode virtuals
    virtual MStatus renderSwatchImage(MImage & image);
private:

};