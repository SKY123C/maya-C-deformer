#include"bulgeDeformer.h"

MObject BulgeDeformer::aBugleAmout;
MTypeId BulgeDeformer::id(0x00000233);


void* BulgeDeformer::creator() {

	return new BulgeDeformer();
}


BulgeDeformer::BulgeDeformer() {

}

BulgeDeformer::~BulgeDeformer() {

}

MStatus BulgeDeformer::deform(MDataBlock& data,
							  MItGeometry& itGeo,
							  const MMatrix& localtoWorldMatrix,
							  unsigned int geomIndex)
{
	MStatus status;
	// ��ȡinput Plug�������
	MArrayDataHandle hInput = data.outputArrayValue(input, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//����input��geomIndex���
	status = hInput.jumpToElement(geomIndex);
	//��ȡ��ǰ������ݵ�����DataHandle
	MDataHandle hInputElement = hInput.outputValue(&status);
	//��ȡinput[0].Geometry��Ϊmesh��ֵ
	MObject oInputGeom = hInputElement.child(inputGeom).asMesh();

	MFnMesh fnMesh(oInputGeom, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MFloatVectorArray normals;
	fnMesh.getVertexNormals(false, normals);

	float bulgeAmout = data.inputValue(aBugleAmout).asFloat();
	float env = data.inputValue(envelope).asFloat();
	MPoint point;
	float w;
	for (; !itGeo.isDone(); itGeo.next())
	{
		//��ȡȨ��ֵ
		w = weightValue(data, geomIndex, itGeo.index());
		
		point = itGeo.position();

		point += normals[itGeo.index()] * bulgeAmout * w * env;

		itGeo.setPosition(point);
	}

	return MS::kSuccess;


}


MStatus BulgeDeformer::initialize() {

	MStatus status;

	MFnNumericAttribute nAttr;
	aBugleAmout = nAttr.create("bulgeAmout", "bulgeAmout", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	addAttribute(aBugleAmout);
	attributeAffects(aBugleAmout, outputGeom);

	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer bulgeDeformer weights;");

	return MS::kSuccess;


}

