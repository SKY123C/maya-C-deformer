#include "SphereCollideDeformer.h"

MTypeId     sphereCollideDeformer::id(0x00007175);
MObject     sphereCollideDeformer::aCollideMatrix;

sphereCollideDeformer::sphereCollideDeformer() {


}

void* sphereCollideDeformer::creator() {

	return new sphereCollideDeformer();
}

sphereCollideDeformer::~sphereCollideDeformer() {


}

MObject& sphereCollideDeformer::accessoryAttribute() const {

	return aCollideMatrix;
}

MStatus sphereCollideDeformer::deform(MDataBlock& data,MItGeometry& itGeo, const MMatrix& localtoWorldMatrix, unsigned int geomIndex) {

	MStatus status;
	MMatrix collideMatrix = data.inputValue(aCollideMatrix).asMatrix();
	MMatrix collideMatrixInverse = collideMatrix.inverse();
	MMatrix worldToLocalMatrix = localtoWorldMatrix.inverse();

	float env = data.inputValue(envelope).asFloat();

	MPoint point;
	for (; !itGeo.isDone(); itGeo.next())
	{
		point = itGeo.position();
		//
		point *= localtoWorldMatrix;
		point *= collideMatrixInverse;
		if (MVector(point).length() < 1.0)
		{
			point = MVector(point).normal();
		}
		point *= collideMatrix;
		point *= worldToLocalMatrix;
		itGeo.setPosition(point);
	}

	return MS::kSuccess;

}


MStatus sphereCollideDeformer::initialize() {

	MStatus status;
	MFnMatrixAttribute mAttr;

	aCollideMatrix = mAttr.create("collideMatrix", "collideMatrix");

	addAttribute(aCollideMatrix);

	attributeAffects(aCollideMatrix, outputGeom);

	return MS::kSuccess;
}

MStatus sphereCollideDeformer::accessoryNodeSetup(MDagModifier& dagMod) {

	MStatus status;
	//����Locator(����shape)
	MObject oLocator = dagMod.createNode("locator", MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//����DG�ڵ㣨Locator��
	MFnDependencyNode fnLocator(oLocator, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//����Locator�ڵ��е�worldMatrix MPlug
	MPlug plugWorldMatrix = fnLocator.findPlug("worldMatrix", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//����worldMatrix����еĵ�һ��Mplug(worldMatrix[0])
	status = plugWorldMatrix.selectAncestorLogicalIndex(0, plugWorldMatrix.attribute());
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//���ر��ڵ�(sphereCollide)
	MObject oThis = thisMObject();
	//���ر��ڵ��е�collideMatrix����
	MPlug plugCollideMatrix(oThis, aCollideMatrix);
	//����worldMatrix[0]��collideMatrix
	status = dagMod.connect(plugWorldMatrix, plugCollideMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;


}
