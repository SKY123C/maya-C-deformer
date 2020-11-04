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

//创建Node的时候使用
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
		//获取Local position
		point = itGeo.position();
		//化物体坐标为世界坐标
		point *= localtoWorldMatrix;
		//获取点经过Local相反位移后的坐标
		point *= collideMatrixInverse;
		/* 如果满足下方if语句。当point乘了一个Locator的逆矩阵，Point的坐标变成他的相反方向（可能），
		极限距离不能超过小球的半径1（结合point *= collideMatrix理解）*/
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
	//创建Locator(包含shape)
	MObject oLocator = dagMod.createNode("locator", MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//创建DG节点（Locator）
	MFnDependencyNode fnLocator(oLocator, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//返回Locator节点中的worldMatrix MPlug
	MPlug plugWorldMatrix = fnLocator.findPlug("worldMatrix", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//进入worldMatrix组件中的第一个Mplug(worldMatrix[0])
	status = plugWorldMatrix.selectAncestorLogicalIndex(0, plugWorldMatrix.attribute());
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//返回本节点(sphereCollide)
	MObject oThis = thisMObject();
	//返回本节点中的collideMatrix属性
	MPlug plugCollideMatrix(oThis, aCollideMatrix);
	//连接worldMatrix[0]和collideMatrix
	status = dagMod.connect(plugWorldMatrix, plugCollideMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;


}
