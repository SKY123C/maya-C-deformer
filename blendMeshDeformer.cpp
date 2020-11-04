#include"blendMeshDeformer.h"

MTypeId BlendMesh::id(0x00000232);
MObject BlendMesh::aBlendMesh;
MObject BlendMesh::aBlendWeight;


BlendMesh::BlendMesh() {

}

BlendMesh::~BlendMesh() {

}

void* BlendMesh::creator()
{
	return new BlendMesh();

}
//类似compute
MStatus BlendMesh::deform(MDataBlock& data, MItGeometry& itGeo, const MMatrix& localtoWorldMatrix, unsigned int geomIndex)
{
	MStatus status;
    //取出Blend Mesh的数据
	MDataHandle hBlendMesh = data.inputValue(aBlendMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	//即输入要根据哪个对象进行变形
	MObject oBlendMesh = hBlendMesh.asMesh();
	if (oBlendMesh.isNull())
	{
		return MS::kFailure;
	}

	MFnMesh fnMesh(oBlendMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MPointArray blendPoints;
	fnMesh.getPoints(blendPoints);
	float blendWeight = data.inputValue(aBlendWeight).asFloat();
	float env = data.inputValue(envelope).asFloat();

	MPoint point;
	//itGeo是output Geometry输出给予对象
	for (;!itGeo.isDone();itGeo.next())
	{
		point = itGeo.position();
		point += (blendPoints[itGeo.index()] - point) * blendWeight*env;

		itGeo.setPosition(point);
	}


	return MS::kSuccess;
}


MStatus BlendMesh::initialize() {
	
	MStatus status;

	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	//创建一个接受mesh参数的Plug
	aBlendMesh = tAttr.create("blendMesh", "blendMesh", MFnData::kMesh);
	addAttribute(aBlendMesh);
	attributeAffects(aBlendMesh, outputGeom);

	aBlendWeight = nAttr.create("blendValue", "blendValue", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	addAttribute(aBlendWeight);
	attributeAffects(aBlendWeight, outputGeom);


	return MS::kSuccess;

}


