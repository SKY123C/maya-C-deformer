#include"bulgeDeformer.h"
#include"blendMeshDeformer.h"
#include"sphereCollideDeformer.h"
#include<maya/MFnPlugin.h>



MStatus initializePlugin(MObject obj) {

	MStatus status;

	MFnPlugin fnPlugin(obj, "TaoChen", "1.0", "Any");

	status = fnPlugin.registerNode("BlendMesh", BlendMesh::id, BlendMesh::creator,
		BlendMesh::initialize, MPxNode::kDeformerNode);

	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerNode("bulgeDeformer", BulgeDeformer::id, BulgeDeformer::creator,
		BulgeDeformer::initialize, MPxNode::kDeformerNode);

	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.registerNode("sphereCollide", sphereCollideDeformer::id,
		sphereCollideDeformer::creator,
		sphereCollideDeformer::initialize,
		MPxNode::kDeformerNode);

	CHECK_MSTATUS_AND_RETURN_IT(status);
	return MS::kSuccess;

}

MStatus uninitializePlugin(MObject obj) {

	MStatus  status;

	MFnPlugin  fnPlugin(obj);

	status = fnPlugin.deregisterNode(BulgeDeformer::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterNode(BlendMesh::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterNode(sphereCollideDeformer::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}
