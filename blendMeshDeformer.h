#ifndef BLENDMESHDEFORMER_H
#define BLENDMESHDEFORMER_H

#include<maya/MPxDeformerNode.h>
#include<maya/MItGeometry.h>
#include<maya/MFnNumericAttribute.h>
#include<maya/MFnTypedAttribute.h>
#include<maya/MFnMesh.h>
#include<maya/MPointArray.h>


class BlendMesh : public MPxDeformerNode
{

public:
	BlendMesh();
	virtual ~BlendMesh();

	static void* creator();

	virtual MStatus deform(MDataBlock& data,
						   MItGeometry& itGeo,
						   const MMatrix& localtoWorldMatrix,
						   unsigned int geomIndex);
	
	static MStatus initialize();

	static MTypeId id;

	static MObject aBlendMesh;
	static MObject aBlendWeight;
};
#endif // !1

