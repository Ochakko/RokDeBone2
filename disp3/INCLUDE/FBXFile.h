#ifndef FBXFILEH
#define FBXFILEH

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include <coef.h>
#include <basedat.h>
#include <matrix2.h>


class CTreeHandler2;
class CShdHandler;
class CMotHandler;
class CShdElem;

class CPolyMesh;
class CPolyMesh2;

#include <fbxsdk.h>
#include <fbxfilesdk_nsuse.h>

class CFBXFile
{
public:
	CFBXFile();
	~CFBXFile();

	int LoadFBXFile( char* filename, CTreeHandler2* srclpth, CShdHandler* srclpsh, CMotHandler* srclpmh, int offset, float datamult );
	int SetInfElem();
	int UnLoad();

private:
	void ConvertNurbsAndPatch(FbxManager* pSdkManager, FbxScene* pScene);
	void ConvertNurbsAndPatchRecursive(FbxManager* pSdkManager, FbxNode* pNode);

	int MakeRDB2Obj( FbxManager* pSdkManager, FbxScene* pScene );
	void MakeRDB2ObjRec( FbxManager* pSdkManager, FbxNode* pNode, int* errcnt );

	int AddShape2Tree( int shdtype, char* srcname );

	int MakePart( FbxNode* pNode );
	int MakePolyMesh( FbxNode* pNode );
	int MakeJoint( FbxNode* pNode );

	void SetJointPos( FbxScene* pScene );
	void SetJointPosRec( FbxNode* pNode );

	FbxAMatrix GetGlobalDefaultPosition(FbxNode* pNode);

	int SetInfElem( FbxScene* pScene );
	void SetInfElemRec( FbxNode* pNode, int* errcnt );


private:
	CTreeHandler2* lpth;
	CShdHandler* lpsh;
	CMotHandler* lpmh;

	FbxManager* m_SdkManager;
	FbxScene* m_Scene;


	///
	int curseri, befseri;
	int curdepth, befdepth;
	int curshdtype, befshdtype;
	CMeshInfo	tempinfo;

	int m_writeno;
	int m_offset;

	char* exttexlist[MAXEXTTEXNUM];
	char exttexbuf[ MAXEXTTEXNUM ][ 256 ];

	float m_datamult;

};


#endif