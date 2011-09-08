/*
 * SimpleShaders.h
 *
 *  Created on: 1 juil. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef SIMPLESHADERS_H_
#define SIMPLESHADERS_H_

namespace { // empty namespace

const char* IMAGE_DIM = "imageDim";
const char* DISPLAY_DIM = "displayDim";
const char* DISPLAY_MODE = "displayMode";
const char* IMAGE_RATIO = "imageRatio";

const char* fittableTransformVs = "float displayMode;\n"
    "float3 displayDim;\n"
    "float3 imageDim;\n"
    "float imageRatio;\n"
    "\n"
    "struct vertexInput {\n"
    "	float3 Position :POSITION;\n"
    "	float2 UV :TEXCOORD0;\n"
    "};\n"
    "\n"
    "struct vertexOutput {\n"
    "	float4 HPosition :POSITION;\n"
    "	float2 UV :TEXCOORD0;\n"
    "};\n"
    "\n"
    "float4x4 projMatrixRH(float width,float height){\n"
    " \n"
    "\tconst float zNear = 0f;\n"
    "\tconst float zFar = 10f;\n"
    "	return float4x4(\n"
    "		2/width,	0,			0,					0,\n"
    "		0,			2/height,	0,					0,\n"
    "		0,			0,			1/(zNear-zFar),		0,\n"
    "		0,			0,			zNear/(zNear-zFar),	1\n"
    "	);\n"
    "}\n"
    "\n"
    "float4x4 scale(float x,float y,float z){\n"
    "	return float4x4(\n"
    "		x,  0,  0,  0,\n"
    "		0,  y,  0,  0,\n"
    "		0,  0,  z,  0,\n"
    "		0,  0,  0,  1\n"
    "	);\n"
    "}\n"
    "\n"
    "float3x3 scale(float x,float y){\n"
    "	return float3x3(\n"
    "		x,	0,	0,\n"
    "		0,	y,	0,\n"
    "		0,	0,	1);\n"
    "}\n"
    "\n"
    "float3x3 translate(float x,float y){\n"
    "	return float3x3(\n"
    "		1,	0,	0,\n"
    "		0,	1,	0,\n"
    "		x,	y,	1);\n"
    "}\n"
    "\n"
    "vertexOutput main(vertexInput IN) {\n"
    "	const float4x4 view={\n"
    "		1,  0,  0,  0,\n"
    "		0,  1,  0,  0,\n"
    "		0,  0,  1,  0,\n"
    "		0,  0,  -10,  1\n"
    "	};\n"
    "\n"
    "	if(imageRatio != 0) imageDim.z = imageRatio;"
    "\n"
    "	float width = 1;\n" // stretch
    "	float height = 1 / imageDim.z;\n"
    "	\n"
    "	if(displayMode==0){\n" // 1:1 pixels
    "		const float constant = imageDim.x;\n"
    "		width = displayDim.x / constant ;\n"
    "		height = displayDim.y / constant ;\n"
    "	} else if(displayMode==1) {\n" // fit display Y
    "		width = displayDim.z;\n"
    "		height = 1;\n"
    "		width /= imageDim.z ;\n"
    "		height /= imageDim.z ;\n"
    "	} else if(displayMode==2){\n" // fit display X
    "		width = 1;\n"
    "		height = 1 / displayDim.z ;\n"
    "	}\n"
    "	\n"
    "	vertexOutput OUT;\n"
    "\n"
    "	const float4x4 objectScale = scale(1,1/imageDim.z,1);\n"
    "	const float4x4 proj = projMatrixRH( width, height );\n"
    "	const float4x4 mWVP = mul( mul( objectScale, view ), proj );\n"
    "	OUT.HPosition = mul( float4(IN.Position.xyz,1) , mWVP);\n"
    "	\n"
    "	float3x3 uvTransform = translate(.5,-.5);\n"
    "	uvTransform = mul( uvTransform, scale(1,-1) );\n"
    "	OUT.UV = mul( float3(IN.Position.xy,1), uvTransform ).xy ;\n"
    "	\n"
    "	return OUT;\n"
    "}\n";

//const char* simpleSamplerPs = "struct vertexOutput {\n"
//    "    float4 HPosition: POSITION;\n"
//    "    float2 UV		: TEXCOORD0;\n"
//    "};\n"
//    "\n"
//    "sampler2D ColorSampler;\n"
//    "\n"
//    "float4 sampleTx1(float2 uv){\n"
//    "	return tex2D( ColorSampler, uv );\n"
//    "}\n"
//    "\n"
//    "float4 main(vertexOutput IN) : COLOR {\n"
//    "	return sampleTx1(IN.UV);\n"
//    "}\n";

}

#endif /* SIMPLESHADERS_H_ */
