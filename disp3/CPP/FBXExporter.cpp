#include <stdafx.h> //ダミー

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>

#include <windows.h>

#include <FBXExporter.h>

#define	DBGH
#include <dbg.h>

#include <crtdbg.h>
#include <coef.h>

#include <shdhandler.h>
#include <treehandler2.h>
#include <mothandler.h>
#include <shdelem.h>
#include <treeelem2.h>
#include <motionctrl.h>
#include <motioninfo.h>

#include <d3ddisp.h>

#include <polymesh.h>
#include <polymesh2.h>
#include <part.h>

#include <morph.h>
#include <MMotElem.h>
#include <MMotKey.h>

#include <motionpoint2.h>
#include <boneinfo.h>
#include <InfElem.h>

#include <quaternion.h>
#include <BoneProp.h>

#include <mqomaterial.h>

#include <FBXBone.h>

#include <vector>
#include <map>
using namespace std;

#include "..\..\RokDeBone2DX\inidata.h"
extern CIniData* g_inidata;

//#include <fbxaxissystem.h>

#include <writefile.h>


#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pSdkManager->GetIOSettings()))
#endif

typedef struct tag_blsindex
{
	int serialno;
	int blendshapeno;
	int channelno;
}BLSINDEX;
typedef struct tag_blsinfo
{
	BLSINDEX blsindex;
	KFbxNode* basenode;
	CMorph* morph;
	CShdElem* target;
}BLSINFO;
static map<int, BLSINFO> s_blsinfo;

typedef struct tag_animinfo
{
	int motid;
	int maxframe;
	KFbxAnimLayer* animlayer;
	char engmotname[256];
}ANIMINFO;
static CWriteFile* s_writefile = 0;
static int s_createbunkiflag = 0;

static map<CShdElem*, map<int, int>> s_linkdirty;

static float s_fbxmult = 1.0f;
static double s_timescale = 30.0;
//static double s_timescale = 60.0;
//static double s_timescale = 1.0;
static int s_itscale = 30;
//static int s_itscale = 60;

static int s_firstoutmot;
static ANIMINFO* s_ai = 0;
static int s_ainum = 0;

static CFBXBone* s_fbxbone = 0;
static CFBXBone* s_firsttopbone = 0;
static int s_fbxbonenum = 0;

static CShdHandler* s_lpsh;
static CTreeHandler2* s_lpth;
static CMotHandler* s_lpmh;
static CShdHandler* s_lpdtrish;
static CTreeHandler2* s_lpdtrith;
static CShdElem* s_topjoint[256];
static int s_topnum;
static LPDIRECT3DDEVICE9 s_pdev;


static void CreateDummyInfDataReq( CFBXBone* fbxbone, KFbxSdkManager*& pSdkManager, KFbxScene*& pScene, KFbxNode* lMesh, KFbxSkin* lSkin, int* bonecnt, CShdElem** ppsetbone );


static CFBXBone* CreateFBXBone( KFbxScene* pScene );
static void CreateFBXBoneReq( KFbxScene* pScene, CShdElem* pbone, CFBXBone* parfbxbone );

static void InitializeSdkObjects(KFbxSdkManager*& pSdkManager, KFbxScene*& pScene);
static void DestroySdkObjects(KFbxSdkManager* pSdkManager);
static void CreateAndFillIOSettings(KFbxSdkManager* pSdkManager);
static bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat=-1, bool pEmbedMedia=false);

static bool CreateScene(KFbxSdkManager* pSdkManager, KFbxScene* pScene );
static KFbxNode* CreateFbxMeshPM( KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem* curse, CPolyMesh* pm, CMeshInfo* mi );
static KFbxNode* CreateFbxMeshPM2( KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int mbno, int totalfacenum );
static KFbxNode* CreateDummyFbxMesh( KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem** ppsetbone );
static KFbxNode* CreateSkeleton(KFbxScene* pScene);
static void CreateSkeletonReq( KFbxScene* pScene, CShdElem* pbone, CShdElem* pparbone, KFbxNode* pparnode );

static void LinkMeshToSkeletonPMReq(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh* pm, CMeshInfo* mi);
static void LinkMeshToSkeletonPM2Req(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum);
static BOOL LinkMeshToSkeletonPM2Func(KFbxCluster* lCluster, CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum);
static void LinkDummyMeshToSkeleton(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, int bonecnt);
static void LinkToTopBone(KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* lMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum);
static BOOL LinkToTopBoneFunc(KFbxCluster* lCluster, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* lMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum);

static int ValidatePm2Index(CPolyMesh2* pm2, int optindex);

//void StoreRestPose(KFbxScene* pScene, KFbxNode* pSkeletonRoot);
static void AnimateSkeleton(KFbxScene* pScene);
static void AnimateBoneReq( CFBXBone* fbxbone, KFbxAnimLayer* lAnimLayer, int curmotid, int motmax, int bonecnt );
static int AnimateMorph(KFbxScene* pScene);

//void AddThumbnailToScene(KFbxScene* pScene);
static void AddNodeRecursively(KArrayTemplate<KFbxNode*>& pNodeArray, KFbxNode* pNode);
static void SetXMatrix(KFbxXMatrix& pXMatrix, const KFbxMatrix& pMatrix);
static KFbxTexture*  CreateTexture( KFbxSdkManager* pSdkManager, char* texname );
static int ExistBoneInInfPM( int boneno, CPolyMesh* pm, int facenum );
static int ExistBoneInInfPM2(int boneno, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int totalfacenum );
static int ConvName2English( int motid, char* srcptr, char* dstptr, int dstleng );

static int MapShapesOnMeshPM2( KFbxScene* pScene, KFbxNode* pNode, CMorph* morph, MATERIALBLOCK* pmb, int mbno, BLSINDEX* blsindex );
static int MapTargetShapePM2( KFbxBlendShapeChannel* lBlendShapeChannel, KFbxScene* pScene, CMorph* morph, CShdElem* targetse, MATERIALBLOCK* pmb, int mbno );

static int WriteBindPose(KFbxScene* pScene);
static void WriteBindPoseReq( CFBXBone* fbxbone, KFbxPose* lPose );
static int DestroyFBXBoneReq( CFBXBone* fbxbone );

static int sortfunc_leng( void *context, const void *elem1, const void *elem2);


static D3DXMATRIX CalcBindMatrixZA(CFBXBone* fbxbone, int srcmotid);
static D3DXMATRIX CalcBindMatrix(CFBXBone* fbxbone);
static D3DXMATRIX CalcAxisMatX_aft(D3DXVECTOR3 curpos, D3DXVECTOR3 chilpos);


int sortfunc_leng( void *context, const void *elem1, const void *elem2)
{
	ANIMINFO* info1 = (ANIMINFO*)elem1;
	ANIMINFO* info2 = (ANIMINFO*)elem2;

	int diffleng = info1->maxframe - info2->maxframe;
	return diffleng;
}


void InitializeSdkObjects(KFbxSdkManager*& pSdkManager, KFbxScene*& pScene)
{
    // The first thing to do is to create the FBX SDK manager which is the 
    // object allocator for almost all the classes in the SDK.
    pSdkManager = KFbxSdkManager::Create();

    if (!pSdkManager)
    {
        printf("Unable to create the FBX SDK manager\n");
        exit(0);
    }

	// create an IOSettings object
	KFbxIOSettings * ios = KFbxIOSettings::Create(pSdkManager, IOSROOT );
	pSdkManager->SetIOSettings(ios);

	// Load plugins from the executable directory
	KString lPath = KFbxGetApplicationDirectory();
#if defined(KARCH_ENV_WIN)
	KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
	KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
	KString lExtension = "so";
#endif
	pSdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

    // Create the entity that will hold the scene.
    pScene = KFbxScene::Create(pSdkManager,"");

/*
	2013.2の話
    int dir;
    pScene->GetGlobalSettings().GetAxisSystem().GetUpVector(dir); // this returns the equivalent of FbxAxisSystem::eYAxis
    FbxAxisSystem axissystem = OpenGL; // we desire to convert the scene from Y-Up to Z-Up
    axissystem.ConvertScene(pScene);
    //pScene->GetGlobalSettings().GetAxisSystem().GetUpVector(dir); // this 
*/

}

void DestroySdkObjects(KFbxSdkManager* pSdkManager)
{
    // Delete the FBX SDK manager. All the objects that have been allocated 
    // using the FBX SDK manager and that haven't been explicitly destroyed 
    // are automatically destroyed at the same time.
    if (pSdkManager) pSdkManager->Destroy();
    pSdkManager = NULL;
}

bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia)
{
    int lMajor, lMinor, lRevision;
    bool lStatus = true;

    // Create an exporter.
    KFbxExporter* lExporter = KFbxExporter::Create(pSdkManager, "");

    if( pFileFormat < 0 || pFileFormat >= pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
    {
        // Write in fall back format in less no ASCII format found
        pFileFormat = pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();
		/*
        //Try to export in ASCII if possible
        int lFormatIndex, lFormatCount = pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

        for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
        {
            if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
            {
                KString lDesc =pSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
                char *lASCII = "ascii";
                if (lDesc.Find(lASCII)>=0)
                {
                    pFileFormat = lFormatIndex;
                    break;
                }
            }
        }
		*/
    }

    // Set the export states. By default, the export states are always set to 
    // true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
    // shows how to change these states.

    IOS_REF.SetBoolProp(EXP_FBX_MATERIAL,        true);
    IOS_REF.SetBoolProp(EXP_FBX_TEXTURE,         true);
    IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED,        pEmbedMedia);
    IOS_REF.SetBoolProp(EXP_FBX_SHAPE,           true);
    IOS_REF.SetBoolProp(EXP_FBX_GOBO,            true);
    IOS_REF.SetBoolProp(EXP_FBX_ANIMATION,       true);
    IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

    // Initialize the exporter by providing a filename.
    if(lExporter->Initialize(pFilename, pFileFormat, pSdkManager->GetIOSettings()) == false)
    {
        printf("Call to KFbxExporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lExporter->GetLastErrorString());
        return false;
    }

    KFbxSdkManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
    printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n\n", lMajor, lMinor, lRevision);

    // Export the scene.
    lStatus = lExporter->Export(pScene); 

    // Destroy the exporter.
    lExporter->Destroy();
    return lStatus;
}

int WriteFBXFile( LPDIRECT3DDEVICE9 pdev, CTreeHandler2* lpth, CShdHandler* lpsh, CMotHandler* lpmh, char* pfilename, float fbxmult, CTreeHandler2* lpdtrith, CShdHandler* lpdtrish, int createbunkiflag )
{
	s_createbunkiflag = createbunkiflag;

	if( s_writefile ){
		delete s_writefile;
		s_writefile = 0;
	}
	char animfilename[MAX_PATH];
	strcpy_s( animfilename, MAX_PATH, "bopeetest01.anim" );
	s_writefile = new CWriteFile( animfilename );
	if( !s_writefile ){
		_ASSERT( 0 );
		return 1;
	}

	s_fbxmult = fbxmult;
	s_timescale = (double)g_inidata->opt_previewfps;

	if( s_fbxbone ){
		DestroyFBXBoneReq( s_fbxbone );
		s_fbxbone = 0;
	}

	s_firstoutmot = -1;

	s_lpth = lpth;
	s_lpsh = lpsh;
	s_lpmh = lpmh;
	s_pdev = pdev;
	s_lpdtrith = lpdtrith;
	s_lpdtrish = lpdtrish;

	int ret;
	ret = s_lpth->ReplaceToEnglishName();
	if( ret ){
		_ASSERT( 0 );
		if( s_writefile ){
			delete s_writefile;
			s_writefile = 0;
		}
		return 1;
	}

    KFbxSdkManager* lSdkManager = NULL;
    KFbxScene* lScene = NULL;
    bool lResult;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);

    // Create the scene.
    lResult = CreateScene( lSdkManager, lScene );

    if(lResult == false)
    {
        printf("\n\nAn error occurred while creating the scene...\n");
        DestroySdkObjects(lSdkManager);
		if( s_writefile ){
			delete s_writefile;
			s_writefile = 0;
		}

        return 0;
    }

    // Save the scene.

	int lFileFormat = lSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

    // The example can take an output file name as an argument.
    lResult = SaveScene( lSdkManager, lScene, pfilename, lFileFormat );

    if(lResult == false)
    {
        printf("\n\nAn error occurred while saving the scene...\n");
        DestroySdkObjects(lSdkManager);
		if( s_writefile ){
			delete s_writefile;
			s_writefile = 0;
		}
		return 0;
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager);

	if( s_fbxbone ){
		DestroyFBXBoneReq( s_fbxbone );
		s_fbxbone = 0;
	}

	if( s_writefile ){
		delete s_writefile;
		s_writefile = 0;
	}

	return 0;
}

bool CreateScene( KFbxSdkManager *pSdkManager, KFbxScene* pScene )
{
    // create scene info
    KFbxDocumentInfo* sceneInfo = KFbxDocumentInfo::Create(pSdkManager,"SceneInfo");
    sceneInfo->mTitle = "scene made by RokDeBone2";
    sceneInfo->mSubject = "skinmesh and animation";
	sceneInfo->mAuthor = "OchakkoLab";
	sceneInfo->mRevision = "rev. 2.0";
	sceneInfo->mKeywords = "skinmesh animation";
	sceneInfo->mComment = "no particular comments required.";

    // we need to add the sceneInfo before calling AddThumbNailToScene because
    // that function is asking the scene for the sceneInfo.
    pScene->SetSceneInfo(sceneInfo);

//    AddThumbnailToScene(pScene);

    KFbxNode* lRootNode = pScene->GetRootNode();

	int ret;
	ZeroMemory( s_topjoint, sizeof( CShdElem* ) * 256 );
	s_topnum = 0;
	ret = s_lpsh->GetTopJoints( 256, s_topjoint, &s_topnum );
	if( ret || (s_topnum <= 0) ){
		_ASSERT( 0 );
		return 0;
	}

	s_fbxbone = CreateFBXBone(pScene);
	lRootNode->AddChild( s_fbxbone->skelnode );

	if( !s_fbxbone ){
		_ASSERT( 0 );
		return 0;
	}

	BLSINDEX blsindex;
	ZeroMemory( &blsindex, sizeof( BLSINDEX ) );
	s_blsinfo.clear();

	CShdElem** ppsetbone = (CShdElem**)malloc(s_lpsh->s2shd_leng * sizeof(CShdElem*));
	_ASSERT(ppsetbone);
	ZeroMemory(ppsetbone, s_lpsh->s2shd_leng * sizeof(CShdElem*));


	int seri;
	for( seri = 0; seri < s_lpsh->s2shd_leng; seri++ ){
		CShdElem* curse = (*s_lpsh)( seri );
		//if (curse->notuse != 1){
			if (curse->type == SHDPOLYMESH){
				CPolyMesh* pm = curse->polymesh;
				_ASSERT(pm);
				if (curse->m_mtype != M_TARGET){
					CMeshInfo* mi = curse->GetMeshInfo();
					_ASSERT(mi);
					KFbxNode* lMesh = CreateFbxMeshPM(pSdkManager, pScene, curse, pm, mi);
					lRootNode->AddChild(lMesh);

					KFbxGeometry* lMeshAttribute = (KFbxGeometry*)lMesh->GetNodeAttribute();
					KFbxSkin* lSkin = KFbxSkin::Create(pScene, "");
					LinkMeshToSkeletonPMReq(s_fbxbone, lSkin, pScene, lMesh, curse, pm, mi);
					lMeshAttribute->AddDeformer(lSkin);

				}
				else{
					_ASSERT(0);
				}
			}
			else if (curse->type == SHDPOLYMESH2){
				CPolyMesh2* pm2 = curse->polymesh2;
				_ASSERT(pm2);
				if (curse->m_mtype != M_TARGET){
					int matcnt;
					int facenum = 0;
					//for (matcnt = 0; matcnt < (pm2->m_materialnum - 1); matcnt++){
					for (matcnt = 0; matcnt < pm2->m_materialnum; matcnt++){
						MATERIALBLOCK* pmb = pm2->m_materialblock + matcnt;
						//_ASSERT(0);
						KFbxNode* lMesh = CreateFbxMeshPM2(pSdkManager, pScene, curse, pm2, pmb, matcnt, facenum);

						/*
						
						// morphのeByControllPoint対応まで一時的にコメントアウト

						if (curse->m_mtype == M_BASE){
							CMorph* morph = 0;
							s_lpsh->GetMorphObj(curse, &morph);
							if (morph){
								MapShapesOnMeshPM2(pScene, lMesh, morph, pmb, matcnt, &blsindex);
							}
						}
						*/

						lRootNode->AddChild(lMesh);

						KFbxGeometry* lMeshAttribute = (KFbxGeometry*)lMesh->GetNodeAttribute();
						KFbxSkin* lSkin = KFbxSkin::Create(pScene, "");
						int* psetflag = (int*)malloc(pm2->optpleng * sizeof(int));
						//int* psetflag = (int*)malloc(pm2->meshinfo->n * 3 * sizeof(int));
						_ASSERT(psetflag);
						ZeroMemory(psetflag, pm2->optpleng * sizeof(int));
						//ZeroMemory(psetflag, pm2->meshinfo->n * 3 * sizeof(int));
						
						s_linkdirty.clear();
						LinkMeshToSkeletonPM2Req(s_fbxbone, lSkin, pScene, lMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, facenum);
						//LinkMeshToSkeletonPM2Req(s_firsttopbone, lSkin, pScene, lMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, facenum);
						LinkToTopBone(lSkin, pScene, lMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, facenum);
						

						free(psetflag);
						psetflag = 0;
						
						lMeshAttribute->AddDeformer(lSkin);

						facenum += (pmb->endface - pmb->startface);//!!!!!!!!!!!!!!!!!!!!!!
					}
					_ASSERT(facenum == curse->GetMeshInfo()->n);
				}
			}
			else{
				//_ASSERT(0);
			}
		//}
	}



	KFbxNode* lMesh = CreateDummyFbxMesh(pSdkManager, pScene, ppsetbone);
	if (lMesh){
		lRootNode->AddChild(lMesh);
		KFbxGeometry* lMeshAttribute = (KFbxGeometry*)lMesh->GetNodeAttribute();
		KFbxSkin* lSkin = KFbxSkin::Create(pScene, "");
		int bonecnt = 0;
		CreateDummyInfDataReq(s_fbxbone, pSdkManager, pScene, lMesh, lSkin, &bonecnt, ppsetbone);
		lMeshAttribute->AddDeformer(lSkin);
	}

	if (ppsetbone){
		free(ppsetbone);
		ppsetbone = 0;
	}


	if( s_ai ){
		free( s_ai );
		s_ai = 0;
	}
	s_ainum = 0;

    AnimateSkeleton(pScene);
	AnimateMorph(pScene);

	WriteBindPose(pScene);

	if( s_ai ){
		free( s_ai );
		s_ai = 0;
	}
	s_ainum = 0;

    return true;
}

int ValidatePm2Index(CPolyMesh2* pm2, int optindex)
{
	if ((optindex >= 0) && (optindex < pm2->optpleng)){
		return 1;
	}
	else{
		return 0;
	}
}


KFbxNode* CreateFbxMeshPM2(KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int mbno, int totalfacenum)
{
	static int s_createcnt = 0;

	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT(d3ddisp);

	int curseri = curse->serialno;
	CTreeElem2* curte = (*s_lpth)(curseri);
	_ASSERT(curte);

	char meshname[256] = { 0 };
	char nodename[256] = { 0 };
	//sprintf_s(meshname, 256, "%s_m%d", curte->engname, pmb->materialno);
	//sprintf_s(nodename, 256, "%s_m%d", curte->engname, pmb->materialno);
	sprintf_s(meshname, 256, "%sShape%d", curte->engname, s_createcnt);
	sprintf_s(nodename, 256, "%sShape%d", curte->engname, s_createcnt);
	s_createcnt++;


	//int facenum = pmb->endface - pmb->startface + 1;
	int facenum = pmb->endface - pmb->startface;

	KFbxMesh* lMesh = KFbxMesh::Create(pScene, meshname);
	lMesh->InitControlPoints(facenum * 3);
	//lMesh->InitControlPoints(pm2->optpleng);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	KFbxVector4* lcp = lMesh->GetControlPoints();


	KFbxGeometryElementNormal* lElementNormal = lMesh->CreateElementNormal();
	//lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	lElementNormal->SetReferenceMode(KFbxGeometryElement::eDIRECT);

	KFbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
	K_ASSERT(lUVDiffuseElement != NULL);
	//lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	//lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eDIRECT);
	lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	//lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eDIRECT);


	//int vsetno;
	//d3ddisp->CalcInitialNormal(pm2);
	//_ASSERT(d3ddisp->m_orgNormal);
	//for (vsetno = 0; vsetno < facenum * 3; vsetno++){
	//	D3DTLVERTEX* tlv = pm2->opttlv + vsetno;
	//	*(lcp + vsetno) = KFbxVector4(tlv->sx * s_fbxmult, tlv->sy * s_fbxmult, -tlv->sz * s_fbxmult, 1.0f);

	//	//KFbxVector2 fbxuv = KFbxVector2(tlv->tu, -tlv->tv);
	//	//lUVDiffuseElement->GetDirectArray().Add(fbxuv);

	//	D3DXVECTOR3 curn = *(d3ddisp->m_orgNormal + vsetno);
	//	KFbxVector4 fbxn = KFbxVector4(curn.x, curn.y, -curn.z, 0.0f);
	//	lElementNormal->GetDirectArray().Add(fbxn);
	//}
	////for (vsetno = 0; vsetno < pm2->optpleng; vsetno++){//!!!!!!!!!!!!!!
	////	D3DTLVERTEX* tlv = pm2->opttlv + vsetno;
	////	*(lcp + vsetno + pm2->optpleng) = KFbxVector4(tlv->sx * s_fbxmult, tlv->sy * s_fbxmult, -tlv->sz * s_fbxmult, 1.0f);

	////	KFbxVector2 fbxuv = KFbxVector2(tlv->tu, -tlv->tv);
	////	lUVDiffuseElement->GetDirectArray().Add(fbxuv);

	////	D3DXVECTOR3 curn = *(d3ddisp->m_orgNormal + vsetno);
	////	KFbxVector4 fbxn = KFbxVector4(curn.x, curn.y, -curn.z, 0.0f);
	////	lElementNormal->GetDirectArray().Add(fbxn);
	////}


	int curvno = 0;
	int faceno;
	for (faceno = pmb->startface; faceno < pmb->endface; faceno++) {
		int vcnt;
		D3DXVECTOR3 spos[3];
		D3DXVECTOR3 snorm[3];
		D3DXVECTOR2 suv[3];
		for (vcnt = 0; vcnt < 3; vcnt++) {
			pm2->GetOptPosByMaterialVNo(mbno, faceno, vcnt, spos + vcnt);
			pm2->GetOptNormalByMaterialVNo(mbno, faceno, vcnt, snorm + vcnt);
			pm2->GetOptUVByMaterialVNo(mbno, faceno, vcnt, suv + vcnt);
		}

		int sindex[3] = { 0, 2, 1 };

		for (vcnt = 0; vcnt < 3; vcnt++) {
			//0 2 1のインデックス順で書き出して　読み込み時の　０　１　２の順に直す
			*(lcp + curvno) = KFbxVector4(spos[sindex[vcnt]].x * s_fbxmult, spos[sindex[vcnt]].y * s_fbxmult, -spos[sindex[vcnt]].z * s_fbxmult, 1.0);

			KFbxVector2 fbxuv = KFbxVector2(suv[sindex[vcnt]].x, -suv[sindex[vcnt]].y);
			lUVDiffuseElement->GetDirectArray().Add(fbxuv);

			KFbxVector4 fbxn = KFbxVector4(snorm[sindex[vcnt]].x, snorm[sindex[vcnt]].y, -snorm[sindex[vcnt]].z, 0.0);
			lElementNormal->GetDirectArray().Add(fbxn);

			curvno++;
		}

	}
	lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);

	//vsetno = 0;
	//int faceno;
	//for (faceno = pmb->startface; faceno < pmb->endface; faceno++){
	//	int vno[3];
	//	vno[0] = *(pm2->m_optindexbuf2 + faceno * 3);
	//	vno[2] = *(pm2->m_optindexbuf2 + faceno * 3 + 1);
	//	vno[1] = *(pm2->m_optindexbuf2 + faceno * 3 + 2);

	//	int valid0, valid1, valid2;
	//	valid0 = ValidatePm2Index(pm2, vno[0]);
	//	valid1 = ValidatePm2Index(pm2, vno[1]);
	//	valid2 = ValidatePm2Index(pm2, vno[2]);
	//	_ASSERT((valid0 == 1) && (valid1 == 1) && (valid2 == 1));

	//	int vcnt;
	//	for (vcnt = 0; vcnt < 3; vcnt++){
	//		SKINVERTEX* skinv = d3ddisp->m_skinv + vno[vcnt];
	//		KFbxVector2 fbxuv = KFbxVector2(skinv->tex1[0], -skinv->tex1[1]);
	//		lUVDiffuseElement->GetDirectArray().Add(fbxuv);

	//		vsetno++;
	//	}
	//}
	//lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);

	//int faceno;
	curvno = 0;
	for (faceno = pmb->startface; faceno < pmb->endface; faceno++) {
		lMesh->BeginPolygon(-1, -1, -1, false);
		int i;
		for (i = 0; i < 3; i++)
		{
			// Control point index
			lMesh->AddPolygon(curvno);
			lUVDiffuseElement->GetIndexArray().SetAt(curvno, curvno);
			curvno++;
		}
		lMesh->EndPolygon();
	}


	// create a KFbxNode
	//		KFbxNode* lNode = KFbxNode::Create(pSdkManager,pName);
	//KFbxNode* lNode = KFbxNode::Create(pScene, meshname);
	KFbxNode* lNode = KFbxNode::Create(pScene, nodename);
	// set the node attribute
	lNode->SetNodeAttribute(lMesh);
	// set the shading mode to view texture
	//lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	// rotate the plane
	lNode->LclRotation.Set(KFbxVector4(0, 0, 0));

	
	// Set material mapping.
	KFbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
	lMaterialElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON);
	lMaterialElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	if (!lMesh->GetElementMaterial(0)){
		_ASSERT(0);
		return NULL;
	}
	D3DXMATERIAL* xmat = (pm2->m_material + mbno);
	if (!xmat){
		_ASSERT(0);
		return NULL;
	}

	D3DMATERIAL9* xmat9 = &(xmat->MatD3D);

	// add material to the node. 
	// the material can't in different document with the geometry node or in sub-document
	// we create a simple material here which belong to main document
	static int s_matcnt = 0;
	s_matcnt++;


	char matname[256];
	sprintf_s(matname, 256, "material_%d", s_matcnt);
	//KString lMaterialName = mqomat->name;
	KString lMaterialName = matname;
	KString lShadingName = "Phong";
	KFbxSurfacePhong* lMaterial = KFbxSurfacePhong::Create(pScene, lMaterialName.Buffer());

	//	lMaterial->Diffuse.Set(fbxDouble3(mqomat->dif4f.r, mqomat->dif4f.g, mqomat->dif4f.b));
	lMaterial->Diffuse.Set(fbxDouble3(xmat9->Diffuse.r, xmat9->Diffuse.g, xmat9->Diffuse.b));
	lMaterial->Emissive.Set(fbxDouble3(xmat9->Emissive.r, xmat9->Emissive.g, xmat9->Emissive.b));
	lMaterial->Ambient.Set(fbxDouble3(xmat9->Ambient.r, xmat9->Ambient.g, xmat9->Ambient.b));
	lMaterial->AmbientFactor.Set(1.0);
	KFbxTexture* curtex = CreateTexture(pSdkManager, xmat->pTextureFilename);
	if (curtex){
		lMaterial->Diffuse.ConnectSrcObject(curtex);
		lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	}
	else{
		lNode->SetShadingMode(KFbxNode::eHARD_SHADING);
	}
	lMaterial->DiffuseFactor.Set(1.0);
	lMaterial->TransparencyFactor.Set(xmat9->Diffuse.a);
	lMaterial->ShadingModel.Set(lShadingName);
	lMaterial->Shininess.Set(0.5);
	lMaterial->Specular.Set(fbxDouble3(xmat9->Specular.r, xmat9->Specular.g, xmat9->Specular.b));
	lMaterial->SpecularFactor.Set(0.3);

	lNode->AddMaterial(lMaterial);
	// We are in eBY_POLYGON, so there's only need for index (a plane has 1 polygon).
	lMaterialElement->GetIndexArray().SetCount(lMesh->GetPolygonCount());
	// Set the Index to the material
	for (int i = 0; i<lMesh->GetPolygonCount(); ++i){
		lMaterialElement->GetIndexArray().SetAt(i, 0);
	}
	

	return lNode;
}

/*
KFbxNode* CreateFbxMeshPM2(KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int mbno)
{
	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT( d3ddisp );

	int curseri = curse->serialno;
	CTreeElem2* curte = (*s_lpth)( curseri );
	_ASSERT( curte );

	char meshname[256] = {0};
	sprintf_s( meshname, 256, "%s_m%d", curte->engname, pmb->materialno );
	int facenum = pmb->endface - pmb->startface;

	KFbxMesh* lMesh = KFbxMesh::Create( pScene, meshname );
	lMesh->InitControlPoints( facenum * 3 );
	KFbxVector4* lcp = lMesh->GetControlPoints();


	KFbxGeometryElementNormal* lElementNormal= lMesh->CreateElementNormal();
	lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	lElementNormal->SetReferenceMode(KFbxGeometryElement::eDIRECT);
	//lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	//lElementNormal->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);

	KFbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV( "DiffuseUV");
	K_ASSERT( lUVDiffuseElement != NULL);
	lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	//lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	//lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eDIRECT);

	int vsetno = 0;
	int faceno;
	for( faceno = pmb->startface; faceno < pmb->endface; faceno++ ){
		int vno[3];
		vno[0] = *(pm2->m_optindexbuf2 + faceno * 3);
		vno[2] = *(pm2->m_optindexbuf2 + faceno * 3 + 1);
		vno[1] = *(pm2->m_optindexbuf2 + faceno * 3 + 2);

		int vcnt;
		for( vcnt = 0; vcnt < 3; vcnt++ ){
			SKINVERTEX* skinv = d3ddisp->m_skinv + vno[vcnt];
			*( lcp + vsetno ) = KFbxVector4( skinv->pos[0] * s_fbxmult, skinv->pos[1] * s_fbxmult, -skinv->pos[2] * s_fbxmult, 1.0f );
			KFbxVector4 fbxn = KFbxVector4( skinv->normal[0], skinv->normal[1], -skinv->normal[2], 0.0f );
			lElementNormal->GetDirectArray().Add( fbxn );
			KFbxVector2 fbxuv = KFbxVector2( skinv->tex1[0], -skinv->tex1[1] );
			lUVDiffuseElement->GetDirectArray().Add( fbxuv );

			vsetno++;
		}
	}
	lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);

	vsetno = 0;
	for( faceno = pmb->startface; faceno < pmb->endface; faceno++ ){
		lMesh->BeginPolygon(-1, -1, -1, false);
		int i;
		for(i = 0; i < 3; i++)
		{
			// Control point index
			lMesh->AddPolygon( vsetno );  
			// update the index array of the UVs that map the texture to the face
			//lElementNormal->GetIndexArray().SetAt(vsetno, vsetno);
			lUVDiffuseElement->GetIndexArray().SetAt(vsetno, vsetno);
			vsetno++;
		}
		lMesh->EndPolygon ();
	}


	// create a KFbxNode
//		KFbxNode* lNode = KFbxNode::Create(pSdkManager,pName);
	KFbxNode* lNode = KFbxNode::Create( pScene, meshname );
	// set the node attribute
	lNode->SetNodeAttribute(lMesh);
	// set the shading mode to view texture
	lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	// rotate the plane
	lNode->LclRotation.Set(KFbxVector4(0, 0, 0));

	// Set material mapping.
	KFbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
	lMaterialElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON);
	lMaterialElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	if( !lMesh->GetElementMaterial(0) ){
		_ASSERT( 0 );
		return NULL;
	}
	D3DXMATERIAL* xmat = (pm2->m_material + mbno);
	if( !xmat ){
		_ASSERT( 0 );
		return NULL;
	}

	D3DMATERIAL9* xmat9 = &(xmat->MatD3D);

	// add material to the node. 
	// the material can't in different document with the geometry node or in sub-document
	// we create a simple material here which belong to main document
	static int s_matcnt = 0;
	s_matcnt++;


	char matname[256];
	sprintf_s( matname, 256, "material_%d", s_matcnt );
	//KString lMaterialName = mqomat->name;
	KString lMaterialName = matname;
	KString lShadingName  = "Phong";
	KFbxSurfacePhong* lMaterial = KFbxSurfacePhong::Create( pScene, lMaterialName.Buffer() );

//	lMaterial->Diffuse.Set(fbxDouble3(mqomat->dif4f.r, mqomat->dif4f.g, mqomat->dif4f.b));
	lMaterial->Diffuse.Set(fbxDouble3(xmat9->Diffuse.r, xmat9->Diffuse.g, xmat9->Diffuse.b));
    lMaterial->Emissive.Set(fbxDouble3(xmat9->Emissive.r, xmat9->Emissive.g, xmat9->Emissive.b));
    lMaterial->Ambient.Set(fbxDouble3(xmat9->Ambient.r, xmat9->Ambient.g, xmat9->Ambient.b));
    lMaterial->AmbientFactor.Set(1.0);
	KFbxTexture* curtex = CreateTexture(pSdkManager, xmat->pTextureFilename);
	if( curtex ){
		lMaterial->Diffuse.ConnectSrcObject( curtex );
		lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	}
	else{
		lNode->SetShadingMode(KFbxNode::eHARD_SHADING);
	}
    lMaterial->DiffuseFactor.Set(1.0);
    lMaterial->TransparencyFactor.Set(xmat9->Diffuse.a);
    lMaterial->ShadingModel.Set(lShadingName);
    lMaterial->Shininess.Set(0.5);
    lMaterial->Specular.Set(fbxDouble3(xmat9->Specular.r, xmat9->Specular.g, xmat9->Specular.b));
    lMaterial->SpecularFactor.Set(0.3);

	lNode->AddMaterial(lMaterial);
	// We are in eBY_POLYGON, so there's only need for index (a plane has 1 polygon).
	lMaterialElement->GetIndexArray().SetCount( lMesh->GetPolygonCount() );
	// Set the Index to the material
	for(int i=0; i<lMesh->GetPolygonCount(); ++i){
		lMaterialElement->GetIndexArray().SetAt(i,0);
	}

	return lNode;
}
*/
KFbxNode* CreateFbxMeshPM(KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem* curse, CPolyMesh* pm, CMeshInfo* mi)
{
	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT( d3ddisp );

	int curseri = curse->serialno;
	CTreeElem2* curte = (*s_lpth)( curseri );
	_ASSERT( curte );

	char meshname[256] = {0};
	sprintf_s( meshname, 256, "%s", curte->engname );
	int facenum = mi->n;

	KFbxMesh* lMesh = KFbxMesh::Create( pScene, meshname );
	lMesh->InitControlPoints( facenum * 3 );
	KFbxVector4* lcp = lMesh->GetControlPoints();

	KFbxGeometryElementNormal* lElementNormal= lMesh->CreateElementNormal();
	lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	lElementNormal->SetReferenceMode(KFbxGeometryElement::eDIRECT);

	KFbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV( "DiffuseUV");
	K_ASSERT( lUVDiffuseElement != NULL);
	lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);

	int vsetno = 0;
	int faceno;
	for( faceno = 0; faceno < facenum; faceno++ ){
		int vno[3];
		vno[0] = *(pm->m_indexbuf2 + faceno * 3);
		vno[2] = *(pm->m_indexbuf2 + faceno * 3 + 1);
		vno[1] = *(pm->m_indexbuf2 + faceno * 3 + 2);

		int vcnt;
		for( vcnt = 0; vcnt < 3; vcnt++ ){
			SKINVERTEX* skinv = d3ddisp->m_skinv + vno[vcnt];

			*( lcp + vsetno ) = KFbxVector4( skinv->pos[0] * s_fbxmult, skinv->pos[1] * s_fbxmult, -skinv->pos[2] * s_fbxmult, 1.0f );

			KFbxVector4 fbxn = KFbxVector4( skinv->normal[0], skinv->normal[1], -skinv->normal[2], 0.0f );
			lElementNormal->GetDirectArray().Add( fbxn );

			KFbxVector2 fbxuv = KFbxVector2( skinv->tex1[0], -skinv->tex1[1] );
			lUVDiffuseElement->GetDirectArray().Add( fbxuv );

			vsetno++;
		}
	}
	lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);

	vsetno = 0;
	for( faceno = 0; faceno < facenum; faceno++ ){
		lMesh->BeginPolygon(-1, -1, -1, false);
		int i;
		for(i = 0; i < 3; i++)
		{
			// Control point index
			lMesh->AddPolygon( vsetno );  
			// update the index array of the UVs that map the texture to the face
			lUVDiffuseElement->GetIndexArray().SetAt( vsetno, vsetno );
			vsetno++;
		}
		lMesh->EndPolygon ();
	}


	// create a KFbxNode
//		KFbxNode* lNode = KFbxNode::Create(pSdkManager,pName);
	KFbxNode* lNode = KFbxNode::Create( pScene, meshname );
	// set the node attribute
	lNode->SetNodeAttribute(lMesh);
	// set the shading mode to view texture
	lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	// rotate the plane
	lNode->LclRotation.Set(KFbxVector4(0, 0, 0));

	// Set material mapping.
	KFbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
	lMaterialElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON);
	lMaterialElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	if( !lMesh->GetElementMaterial(0) ){
		_ASSERT( 0 );
		return NULL;
	}


	// add material to the node. 
	// the material can't in different document with the geometry node or in sub-document
	// we create a simple material here which belong to main document
	static int s_matcnt = 0;
	s_matcnt++;

	char matname[256];
	sprintf_s( matname, 256, "mat_%s", curte->engname );
	//KString lMaterialName = mqomat->name;
	KString lMaterialName = matname;
	KString lShadingName  = "Phong";
	KFbxSurfacePhong* lMaterial = KFbxSurfacePhong::Create( pScene, lMaterialName.Buffer() );

	//KFbxTexture* curtex = CreateTexture(pSdkManager, xmat->pTextureFilename);
	//if (curtex){
	//	lMaterial->Diffuse.ConnectSrcObject(curtex);
	//	lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	//}
	//else{
		lNode->SetShadingMode(KFbxNode::eHARD_SHADING);
	//}

	lMaterial->Diffuse.Set(fbxDouble3(mi->diffuse->x, mi->diffuse->y, mi->diffuse->z));
    lMaterial->Emissive.Set(fbxDouble3(0.0, 0.0, 0.0));
    lMaterial->Ambient.Set(fbxDouble3(mi->ambient->x, mi->ambient->y, mi->ambient->z));
    lMaterial->AmbientFactor.Set(1.0);
    lMaterial->DiffuseFactor.Set(1.0);
    lMaterial->TransparencyFactor.Set(curse->alpha);
    lMaterial->ShadingModel.Set(lShadingName);
    lMaterial->Shininess.Set(0.5);
    lMaterial->Specular.Set(fbxDouble3(mi->specular->x, mi->specular->y, mi->specular->z));
    lMaterial->SpecularFactor.Set(0.3);

	lNode->AddMaterial(lMaterial);
	// We are in eBY_POLYGON, so there's only need for index (a plane has 1 polygon).
	lMaterialElement->GetIndexArray().SetCount( lMesh->GetPolygonCount() );
	// Set the Index to the material
	for(int i=0; i<lMesh->GetPolygonCount(); ++i){
		lMaterialElement->GetIndexArray().SetAt(i,0);
	}

	return lNode;
}

KFbxTexture*  CreateTexture(KFbxSdkManager* pSdkManager, char* texname)
{
	if( !texname ){
		return NULL;
	}
	if( *texname == 0 ){
		return NULL;
	}

    KFbxFileTexture* lTexture = KFbxFileTexture::Create(pSdkManager,"");
    KString lTexPath = texname;

    // Set texture properties.
    lTexture->SetFileName(lTexPath.Buffer());
    //lTexture->SetName("Diffuse Texture");
	lTexture->SetName(texname);
    lTexture->SetTextureUse(KFbxTexture::eSTANDARD);
    lTexture->SetMappingType(KFbxTexture::eUV);
    lTexture->SetMaterialUse(KFbxFileTexture::eMODEL_MATERIAL);
    lTexture->SetSwapUV(false);
    lTexture->SetAlphaSource (KFbxTexture::eNONE);
    lTexture->SetTranslation(0.0, 0.0);
    lTexture->SetScale(1.0, 1.0);
    lTexture->SetRotation(0.0, 0.0);

    return lTexture;
}


BOOL LinkMeshToSkeletonPM2Func(KFbxCluster* lCluster, CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum)
{
	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT(d3ddisp);

	KFbxNode* lSkel;

	if (!s_firsttopbone || !fbxbone){
		return FALSE;
	}

	lSkel = fbxbone->skelnode;
	if (!lSkel){
		//_ASSERT(0);
		return FALSE;
	}

	CShdElem* curbone = fbxbone->selem;
	if (curbone){
		int foundinf = 0;

		//if ((fbxbone != s_fbxbone) && (fbxbone->selem != s_firsttopbone->selem) && ((fbxbone->type == FB_NORMAL) || (fbxbone->type == FB_BUNKI_CHIL))){
		if (fbxbone->selem && (fbxbone->selem != s_firsttopbone->selem)){

			int seri = fbxbone->selem->serialno;
			//CShdElem* curbone = fbxbone->selem;
			_ASSERT(curse);

			int doneflag = 0;
			//if (*(ppsetbone + curbone->serialno) != 0){
			//	doneflag = 1;
			//}


			if (doneflag == 0){

				map<int, int> mapdirty;
				s_linkdirty[curbone] = mapdirty;
				map<CShdElem*, map<int, int>>::iterator itrlinkdirty;
				itrlinkdirty = s_linkdirty.find(curbone);

				CTreeElem2* curte = (*s_lpth)(seri);
				_ASSERT(curte);

				if (curbone->IsJoint()){

					*(ppsetbone + curbone->serialno) = curbone;//!!!!!!!!!

					int curvno = 0;
					int faceno;
					int setfaceno = totalfacenum;
					//for (fno = pmb->startface; fno < pmb->endface; fno++){
					for (faceno = pmb->startface; faceno < pmb->endface; faceno++) {
						int sindex[3] = { 0, 2, 1 };

						int vcnt;
						for (vcnt = 0; vcnt < 3; vcnt++){
							int existflag = 0;
							float curdispinf = 0.0f;
							pm2->GetOptInfByMaterialVNo(matcnt, faceno, sindex[vcnt], curbone->serialno, &existflag, &curdispinf);
							if (existflag == 1){
								if (foundinf == 0){
									foundinf = 1;
									if (lCluster){
										lCluster->SetLinkMode(KFbxCluster::eTOTAL1);
									}
									else{
										return TRUE;
									}
								}

								if (lCluster){
									lCluster->AddControlPointIndex(curvno, curdispinf);
									if (*(ppsetbone + curbone->serialno) == 0){
										*(ppsetbone + curbone->serialno) = curbone;
									}
									(fbxbone->m_boneinfcnt)++;
								}
							}
							curvno++;
						}
					}
				}
			}
		}

		if (foundinf == 0){
			return FALSE;
		}

		if (lCluster){
			KFbxXMatrix lXMatrix;
			lXMatrix.SetIdentity();
			lXMatrix = pMesh->EvaluateGlobalTransform();
			lCluster->SetTransformMatrix(lXMatrix);

			KFbxXMatrix lXMatrix2;
			lXMatrix2 = lSkel->EvaluateGlobalTransform();
			lCluster->SetTransformLinkMatrix(lXMatrix2);

			lSkin->AddCluster(lCluster);

			return TRUE;
		}
	}

	return FALSE;

}

void LinkMeshToSkeletonPM2Req(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum)
{
	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT( d3ddisp );

    KFbxNode* lSkel;

//	int cmp4 = strcmp(pMesh->GetName(), "body_m4");
//	if (cmp4 == 0){
//		_ASSERT(0);
//	}
	//int cmp5 = strcmp(pMesh->GetName(), "body_m5");


	if (!s_firsttopbone || !fbxbone){
		return;
	}

	lSkel = fbxbone->skelnode;
	if (!lSkel){
		//_ASSERT(0);
		return;
	}

	CShdElem* curbone = fbxbone->selem;
	if (curbone){
		BOOL isdirty;
		isdirty = LinkMeshToSkeletonPM2Func(0, fbxbone, lSkin, pScene, pMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);

		if (isdirty){
			KFbxCluster *lCluster = KFbxCluster::Create(pScene, "");
			lCluster->SetLink(lSkel);
			//lCluster->SetLinkMode(KFbxCluster::eTOTAL1);

			BOOL isdirty2;
			isdirty2 = LinkMeshToSkeletonPM2Func(lCluster, fbxbone, lSkin, pScene, pMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);
			_ASSERT(isdirty2 == TRUE);
		}
	}

	if( fbxbone->m_child ){
		LinkMeshToSkeletonPM2Req(fbxbone->m_child, lSkin, pScene, pMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);
	}
	if( fbxbone->m_brother ){
		LinkMeshToSkeletonPM2Req(fbxbone->m_brother, lSkin, pScene, pMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);
	}

}

BOOL LinkToTopBoneFunc(KFbxCluster* lCluster, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* lMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum)
{
	KFbxXMatrix lXMatrix;
	KFbxNode* lSkel;

	if (s_firsttopbone){
		//lSkel = s_fbxbone->skelnode;
		lSkel = s_firsttopbone->skelnode;
		if (!lSkel){
			_ASSERT(0);
			return FALSE;
		}
		CShdElem* curbone = s_firsttopbone->selem;
		if (curbone){
			int foundinf = 0;


			map<CShdElem*, map<int, int>>::iterator itrlinkdirty;
			map<int, int> mapdirty;
			s_linkdirty[curbone] = mapdirty;
			itrlinkdirty = s_linkdirty.find(curbone);


			if (curbone->IsJoint()){

				//*(ppsetbone + curbone->serialno) = curbone;//!!!!!!!!!

				int curvno = 0;
				int faceno;
				int setfaceno = totalfacenum;
				//for (fno = pmb->startface; fno < pmb->endface; fno++){
				for (faceno = pmb->startface; faceno < pmb->endface; faceno++) {
					int sindex[3] = { 0, 2, 1 };

					int vcnt;
					for (vcnt = 0; vcnt < 3; vcnt++){
						int existflag = 0;
						float curdispinf = 0.0f;
						pm2->GetOptInfByMaterialVNo(matcnt, faceno, sindex[vcnt], curbone->serialno, &existflag, &curdispinf);
						if (existflag == 1){
							if (foundinf == 0){
								foundinf = 1;
								if (lCluster){
									lCluster->SetLinkMode(KFbxCluster::eTOTAL1);
								}
								else{
									return TRUE;
								}
							}

							if (lCluster){
								lCluster->AddControlPointIndex(curvno, curdispinf);
								if (*(ppsetbone + curbone->serialno) == 0){
									*(ppsetbone + curbone->serialno) = curbone;
								}
								(s_firsttopbone->m_boneinfcnt)++;
							}
						}
						curvno++;
					}
				}
			}

			if (foundinf == 0){
				//lCluster->SetLinkMode(KFbxCluster::eADDITIVE);
				return FALSE;
			}

			if (lCluster){
				//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				//pos.x = s_firsttopbone->selem->part->jointloc.x * s_fbxmult;
				//pos.y = s_firsttopbone->selem->part->jointloc.y * s_fbxmult;
				//pos.z = -s_firsttopbone->selem->part->jointloc.z * s_fbxmult;

				KFbxScene* lScene = lMesh->GetScene();
				lXMatrix.SetIdentity();
				lXMatrix = lMesh->EvaluateGlobalTransform();
				lCluster->SetTransformMatrix(lXMatrix);
				//lXMatrix.SetIdentity();
				//lXMatrix[3][0] = -pos.x;// *s_fbxmult;
				//lXMatrix[3][1] = -pos.y;// *s_fbxmult;
				//lXMatrix[3][2] = pos.z;// *s_fbxmult;
				//lCluster->SetTransformMatrix(lXMatrix);

				//lXMatrix = lSkel->EvaluateGlobalTransform();
				//lCluster->SetTransformLinkMatrix(lXMatrix);
				lXMatrix.SetIdentity();
				lXMatrix = lSkel->EvaluateGlobalTransform();
				lCluster->SetTransformLinkMatrix(lXMatrix);

				lSkin->AddCluster(lCluster);
				return TRUE;
			}
		}

	}

	return FALSE;
}


void LinkToTopBone(KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* lMesh, CShdElem* curse, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int matcnt, int* psetflag, CShdElem** ppsetbone, int totalfacenum)
{
	KFbxXMatrix lXMatrix;
	KFbxNode* lSkel;

	if (s_firsttopbone){
		//lSkel = s_fbxbone->skelnode;
		lSkel = s_firsttopbone->skelnode;
		if (!lSkel){
			_ASSERT(0);
			return;
		}
		CShdElem* curbone = s_firsttopbone->selem;
		if (curbone){
			BOOL isdirty;
			isdirty = LinkToTopBoneFunc(0, lSkin, pScene, lMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);
			if (isdirty == TRUE){
				KFbxCluster *lCluster = KFbxCluster::Create(pScene, "");
				lCluster->SetLink(lSkel);
				//lCluster->SetLinkMode(KFbxCluster::eTOTAL1);

				BOOL isdirty2;
				isdirty2 = LinkToTopBoneFunc(lCluster, lSkin, pScene, lMesh, curse, pm2, pmb, matcnt, psetflag, ppsetbone, totalfacenum);
				_ASSERT(isdirty2 == TRUE);
			}

		}

	}
}
/*
void LinkToTopBone(KFbxSkin* lSkin, KFbxScene* pScene, CPolyMesh2* pm2, int* psetflag)
{
	KFbxXMatrix lXMatrix;
	KFbxNode* lSkel;

	if (s_firsttopbone){
		//lSkel = s_fbxbone->skelnode;
		lSkel = s_firsttopbone->skelnode;
		if (!lSkel){
			_ASSERT(0);
			return;
		}

		KFbxCluster *lCluster = KFbxCluster::Create(pScene, "");
		lCluster->SetLink(lSkel);
		lCluster->SetLinkMode(KFbxCluster::eTOTAL1);

		int setv;
		for (setv = 0; setv < pm2->optpleng; setv++){
			if (*(psetflag + setv) == 0){
				lCluster->AddControlPointIndex(setv, 1.0);
			}
		}

		D3DXVECTOR3 pos;
		pos.x = s_firsttopbone->selem->part->jointloc.x * s_fbxmult;
		pos.y = s_firsttopbone->selem->part->jointloc.y * s_fbxmult;
		pos.z = -s_firsttopbone->selem->part->jointloc.z * s_fbxmult;

		//KFbxScene* lScene = pMesh->GetScene();
		//lXMatrix = pMesh->EvaluateGlobalTransform();
		//lCluster->SetTransformMatrix(lXMatrix);
		lXMatrix.SetIdentity();
		lXMatrix[3][0] = -pos.x * s_fbxmult;
		lXMatrix[3][1] = -pos.y * s_fbxmult;
		lXMatrix[3][2] = pos.z * s_fbxmult;
		lCluster->SetTransformMatrix(lXMatrix);

		//lXMatrix = lSkel->EvaluateGlobalTransform();
		//lCluster->SetTransformLinkMatrix(lXMatrix);
		lXMatrix.SetIdentity();
		lCluster->SetTransformLinkMatrix(lXMatrix);

		lSkin->AddCluster(lCluster);
	}
}
*/


void LinkMeshToSkeletonPMReq(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, CShdElem* curse, CPolyMesh* pm, CMeshInfo* mi)
{
	CD3DDisp* d3ddisp = curse->d3ddisp;
	_ASSERT( d3ddisp );

	KFbxXMatrix lXMatrix;
    KFbxNode* lSkel;

	if (fbxbone->type == FB_NORMAL){

		int seri = fbxbone->selem->serialno;
		CShdElem* curbone = fbxbone->selem;
		_ASSERT( curse );
		CTreeElem2* curte = (*s_lpth)( seri );
		_ASSERT( curte );

		if( curbone->IsJoint() ){
			lSkel = fbxbone->skelnode;
			if( !lSkel ){
				_ASSERT( 0 );
				return;
			}

			int infdirty = ExistBoneInInfPM( curbone->serialno, pm, mi->n );

			if( infdirty ){
				KFbxCluster *lCluster = KFbxCluster::Create(pScene,"");
				lCluster->SetLink(lSkel);
				lCluster->SetLinkMode(KFbxCluster::eTOTAL1);

				int vsetno = 0;
				int fno;
				for( fno = 0; fno < mi->n; fno++ ){
					int vno[3];
					vno[0] = *(pm->m_indexbuf2 + fno * 3);
					vno[2] = *(pm->m_indexbuf2 + fno * 3 + 1);
					vno[1] = *(pm->m_indexbuf2 + fno * 3 + 2);
				
					int vcnt;
					for( vcnt = 0; vcnt < 3; vcnt++ ){
						CInfElem* curie = pm->m_IE + vno[vcnt];
						int ieno = curie->GetInfElemByBone( curbone->serialno );
						if( ieno >= 0 ){
							lCluster->AddControlPointIndex( vsetno, (double)( (curie->ie + ieno)->dispinf ) );
							//lCluster->AddControlPointIndex(vno[vcnt], (double)((curie->ie + ieno)->dispinf));
							(fbxbone->m_boneinfcnt)++;
						}
						vsetno++;
					}
				}

				CShdElem* parbone = curbone->parent;
				D3DXVECTOR3 pos;
				if (fbxbone->type == FB_NORMAL){
					if (parbone->IsJoint()){
						pos.x = parbone->part->jointloc.x;
						pos.y = parbone->part->jointloc.y;
						pos.z = parbone->part->jointloc.z;
					}
					else{
						pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else if (fbxbone->type == FB_ENDJOINT){
					if (curbone->IsJoint()){
						pos.x = curbone->part->jointloc.x;
						pos.y = curbone->part->jointloc.y;
						pos.z = curbone->part->jointloc.z;
					}
					else{
						pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else{
					_ASSERT(0);
				}

				KFbxXMatrix lXMatrix;
				lXMatrix = pMesh->EvaluateGlobalTransform();
				lCluster->SetTransformMatrix(lXMatrix);

				KFbxXMatrix lXMatrix2;
				lXMatrix2 = lSkel->EvaluateGlobalTransform();
				lCluster->SetTransformLinkMatrix(lXMatrix2);

				lSkin->AddCluster(lCluster);
			}
		}
	}

	if( fbxbone->m_child ){
		LinkMeshToSkeletonPMReq(fbxbone->m_child, lSkin, pScene, pMesh, curse, pm, mi);
	}
	if( fbxbone->m_brother ){
		LinkMeshToSkeletonPMReq(fbxbone->m_brother, lSkin, pScene, pMesh, curse, pm, mi);
	}

}

void AnimateSkeleton(KFbxScene* pScene)
{

	KString lAnimStackName;

	int motnum = s_lpmh->m_kindnum;
	if( motnum < 2 ){
		return;
	}

	CMotionCtrl* topmc = (*s_lpmh)( s_topjoint[0]->serialno );
	_ASSERT( topmc );
	CMotionInfo* topmi = topmc->motinfo;
	_ASSERT( topmi );


	s_ai = (ANIMINFO*)malloc( sizeof( ANIMINFO ) * (motnum - 1) );
	if( !s_ai ){
		_ASSERT( 0 );
		return;
	}
	ZeroMemory( s_ai, sizeof( ANIMINFO ) * (motnum - 1) );
	s_ainum = (motnum - 1);

	int ano;
	for( ano = 0; ano < (motnum - 1); ano++ ){
		(s_ai + ano)->motid = ano;
		(s_ai + ano)->maxframe = *( topmi->motnum + ano ) - 1;
	}
	qsort_s( s_ai, motnum - 1, sizeof( ANIMINFO ), sortfunc_leng, NULL );

	s_firstoutmot = s_ai->motid;

	int outmaxframe = motnum * s_itscale - 1;

	s_writefile->Write2File( "animVersion 1.1;\n" );
	s_writefile->Write2File( "mayaVersion 2013 x64;\n" );
	s_writefile->Write2File( "timeUnit milisec;\n" );
	s_writefile->Write2File( "linearUnit cm;\n" );
	s_writefile->Write2File( "angularUnit deg;\n" );
	s_writefile->Write2File( "startTime 0;\n" );
	s_writefile->Write2File( "endTime %d;\n", outmaxframe );

/*
animVersion 1.1;
mayaVersion 2.0;
timeUnit ntsc;
linearUnit cm;
angularUnit deg;
startTime 1;
endTime 30;
*/


	int motmax;
	int curmotid;
	for( ano = 0; ano < (motnum - 1); ano++ ){
		curmotid = (s_ai + ano)->motid;
		motmax = (s_ai + ano)->maxframe;

		char* motname = *( topmi->motname + curmotid );
		char engname0[256] = {0};
		ConvName2English( curmotid, motname, engname0, 256 );
		char engname[256] = {0};
		sprintf_s( engname, 256, "%s_max%d", engname0, motmax );

		char layername[256];
		//sprintf_s( layername, 256, "Layer_%d", curmotid );
		sprintf_s(layername, 256, "Base Layer");

		lAnimStackName = engname;
	    KFbxAnimStack* lAnimStack = KFbxAnimStack::Create(pScene, lAnimStackName);
        KFbxAnimLayer* lAnimLayer = KFbxAnimLayer::Create(pScene, layername);
		//KFbxAnimLayer* lAnimLayer = KFbxAnimLayer::Create(lAnimStack, layername);
		(s_ai + ano)->animlayer = lAnimLayer;
		strcpy_s( (s_ai + ano)->engmotname, 256, engname );
        lAnimStack->AddMember(lAnimLayer);

		AnimateBoneReq( s_fbxbone, lAnimLayer, curmotid, motmax, 0 );

		//pScene->GetRootNode()->ConvertPivotAnimationRecursive( lAnimStackName, KFbxNode::eDESTINATION_SET, s_timescale, true );
		//pScene->GetRootNode()->ConvertPivotAnimationRecursive( lAnimStackName, KFbxNode::eSOURCE_SET, s_timescale, false );
		pScene->GetRootNode()->ConvertPivotAnimationRecursive( lAnimStackName, KFbxNode::eSOURCE_SET, s_timescale, true );
	}
}

void AnimateBoneReq( CFBXBone* fbxbone, KFbxAnimLayer* lAnimLayer, int curmotid, int motmax, int jointcnt )
{
	int ret;
    KTime lTime;
    int lKeyIndex = 0;
    KFbxNode* lSkel = 0;
	lSkel = fbxbone->skelnode;
	if (!lSkel){
		_ASSERT(0);
		return;
	}
	const char* bonename = lSkel->GetName();


//		ERotationOrder lRotationOrder0 = eEULER_ZXY;
	ERotationOrder lRotationOrder0 = eEULER_XYZ;
	ERotationOrder lRotationOrder1 = eEULER_ZXY;
	lSkel->SetRotationOrder(KFbxNode::eSOURCE_SET , lRotationOrder0 );
	lSkel->SetRotationOrder(KFbxNode::eDESTINATION_SET , lRotationOrder1 );

	CMotionPoint2 calcmp;

	KFbxAnimCurve* lCurveTX;
	KFbxAnimCurve* lCurveTY;
	KFbxAnimCurve* lCurveTZ;

	D3DXVECTOR3 orgtra;
	int frameno;

	if (fbxbone->type == FB_ENDJOINT){
		int seri = fbxbone->selem->serialno;
		CShdElem* curbone = fbxbone->selem;
		_ASSERT(curbone);

		CPart* curpart = curbone->part;
		_ASSERT(curpart);

		CShdElem* parbone = curbone->parent;
		CShdElem* gparbone = 0;
		if (parbone){
			CPart* parpart = parbone->part;
			_ASSERT(parpart);

			if (parpart){
				orgtra = D3DXVECTOR3(curpart->jointloc.x - parpart->jointloc.x, curpart->jointloc.y - parpart->jointloc.y, -curpart->jointloc.z - -parpart->jointloc.z);
			}
			else{
				orgtra = D3DXVECTOR3(curpart->jointloc.x, curpart->jointloc.y, -curpart->jointloc.z);
			}
		}
		else{
			orgtra = D3DXVECTOR3(curpart->jointloc.x, curpart->jointloc.y, -curpart->jointloc.z);
		}

		orgtra *= s_fbxmult;

		lCurveTX = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_X, true);
		lCurveTX->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTX->KeyAdd(lTime);
			lCurveTX->KeySetValue(lKeyIndex, orgtra.x);
			lCurveTX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTX->KeyModifyEnd();

		lCurveTY = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Y, true);
		lCurveTY->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTY->KeyAdd(lTime);
			lCurveTY->KeySetValue(lKeyIndex, orgtra.y);
			lCurveTY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

		}
		lCurveTY->KeyModifyEnd();

		lCurveTZ = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Z, true);
		lCurveTZ->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lKeyIndex = lCurveTZ->KeyAdd(lTime);
			lCurveTZ->KeySetValue(lKeyIndex, orgtra.z);
			lCurveTZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTZ->KeyModifyEnd();

		/////////////////////

		KFbxAnimCurve* lCurveRX;
		lCurveRX = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_X, true);
		lCurveRX->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRX->KeyAdd(lTime);
			lCurveRX->KeySetValue(lKeyIndex, 0.0);
			lCurveRX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveRX->KeyModifyEnd();
		KFbxAnimCurve* lCurveRY;
		lCurveRY = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Y, true);
		lCurveRY->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRY->KeyAdd(lTime);
			lCurveRY->KeySetValue(lKeyIndex, 0.0);
			lCurveRY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveRY->KeyModifyEnd();
		KFbxAnimCurve* lCurveRZ;
		lCurveRZ = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Z, true);
		lCurveRZ->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRZ->KeyAdd(lTime);
			lCurveRZ->KeySetValue(lKeyIndex, 0.0);
			lCurveRZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveRZ->KeyModifyEnd();

		jointcnt++;
	}
	else if ((fbxbone->type == FB_BUNKI_CHIL) || (fbxbone->type == FB_ROOT)){

		lCurveTX = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_X, true);
		lCurveTX->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTX->KeyAdd(lTime);
			lCurveTX->KeySetValue(lKeyIndex, 0.0);
			lCurveTX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTX->KeyModifyEnd();

		lCurveTY = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Y, true);
		lCurveTY->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTY->KeyAdd(lTime);
			lCurveTY->KeySetValue(lKeyIndex, 0.0);
			lCurveTY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

		}
		lCurveTY->KeyModifyEnd();

		lCurveTZ = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Z, true);
		lCurveTZ->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lKeyIndex = lCurveTZ->KeyAdd(lTime);
			lCurveTZ->KeySetValue(lKeyIndex, 0.0);
			lCurveTZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTZ->KeyModifyEnd();

		/////////////////////

		s_writefile->Write2File("anim rotate.rotateX rotateX %s %d 1 0;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRX;
		lCurveRX = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_X, true);
		lCurveRX->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRX->KeyAdd(lTime);
			lCurveRX->KeySetValue(lKeyIndex, 0.0);
			lCurveRX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

			s_writefile->Write2File("%d 0.0000 linear linear 1 1 0;\n", frameno * s_itscale);
		}
		lCurveRX->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");

		////////////////////
		s_writefile->Write2File("anim rotate.rotateY rotateY %s %d 1 1;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRY;
		lCurveRY = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Y, true);
		lCurveRY->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRY->KeyAdd(lTime);
			lCurveRY->KeySetValue(lKeyIndex, 0.0);
			lCurveRY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

			s_writefile->Write2File("%d 0.0000 linear linear 1 1 0;\n", frameno * s_itscale);
		}
		lCurveRY->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");


		s_writefile->Write2File("anim rotate.rotateZ rotateZ %s %d 1 1;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRZ;
		lCurveRZ = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Z, true);
		lCurveRZ->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRZ->KeyAdd(lTime);
			lCurveRZ->KeySetValue(lKeyIndex, 0.0);
			lCurveRZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

			s_writefile->Write2File("%d 0.0000 linear linear 1 1 0;\n", frameno * s_itscale);
		}
		lCurveRZ->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");


		jointcnt++;
	}
	else{
		int seri = fbxbone->selem->serialno;
		CShdElem* curbone = fbxbone->selem;
		_ASSERT(curbone);

		CPart* curpart = curbone->part;
		_ASSERT(curpart);

		CShdElem* parbone = curbone->parent;
		CShdElem* gparbone = 0;
		if (parbone && parbone->IsJoint()){//!!!!
			CPart* parpart = parbone->part;
			_ASSERT(parpart);

			CShdElem* gparbone = parbone->parent;
			CPart* gparpart;
			if (gparbone && gparbone->IsJoint()){
				gparpart = gparbone->part;
			}
			else{
				gparpart = 0;
			}

			if (gparpart){
				orgtra = D3DXVECTOR3(parpart->jointloc.x - gparpart->jointloc.x, parpart->jointloc.y - gparpart->jointloc.y, -parpart->jointloc.z - -gparpart->jointloc.z);
			}
			else{
				//orgtra = D3DXVECTOR3(curpart->jointloc.x - parpart->jointloc.x, curpart->jointloc.y - parpart->jointloc.y, -curpart->jointloc.z - -parpart->jointloc.z);
				orgtra = D3DXVECTOR3(parpart->jointloc.x, parpart->jointloc.y, -parpart->jointloc.z);
			}
		}
		else{
			orgtra = D3DXVECTOR3(curpart->jointloc.x, curpart->jointloc.y, -curpart->jointloc.z);
		}

		orgtra *= s_fbxmult;

		CMotionCtrl* curmc = (*s_lpmh)(seri);
		_ASSERT(curmc);
		CMotionInfo* curmi = curmc->motinfo;
		_ASSERT(curmi);
		CTreeElem2* curte = (*s_lpth)(seri);


		lCurveTX = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_X, true);
		lCurveTX->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTX->KeyAdd(lTime);
			float settra = orgtra.x + calcmp.m_mvx * s_fbxmult;
			lCurveTX->KeySetValue(lKeyIndex, settra);
			lCurveTX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTX->KeyModifyEnd();

		lCurveTY = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Y, true);
		lCurveTY->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTY->KeyAdd(lTime);
			float settra = orgtra.y + calcmp.m_mvy * s_fbxmult;
			lCurveTY->KeySetValue(lKeyIndex, settra);
			lCurveTY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);

		}
		lCurveTY->KeyModifyEnd();

		lCurveTZ = lSkel->LclTranslation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_T_Z, true);
		lCurveTZ->KeyModifyBegin();
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveTZ->KeyAdd(lTime);
			//float settra = orgtra.z + calcmp.m_mvz * s_fbxmult;
			float settra = orgtra.z - calcmp.m_mvz * s_fbxmult;
			lCurveTZ->KeySetValue(lKeyIndex, settra);
			lCurveTZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
		}
		lCurveTZ->KeyModifyEnd();

		/////////////////////

		CMotionPoint2* firstmp = *(curmi->firstmp + curmotid);
		CMotionPoint2* curmp = firstmp;
		D3DXVECTOR3 befeul(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 cureul(0.0f, 0.0f, 0.0f);
		CQuaternion axisq;
		axisq.SetParams(1.0f, 0.0f, 0.0f, 0.0f);
		CQuaternion befq;
		CQuaternion curq;


		s_writefile->Write2File("anim rotate.rotateX rotateX %s %d 1 0;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRX;
		lCurveRX = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_X, true);
		lCurveRX->KeyModifyBegin();
		befeul = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		befq.SetParams(1.0f, 0.0f, 0.0f, 0.0f);
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			//ret = curmi->CalcMotionPointOnFrame( &axisq, curbone, &calcmp, curmotid, frameno, &hasmpflag );
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			curq = calcmp.m_q.CalcFBXEul(befq, befeul, &cureul);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRX->KeyAdd(lTime);
			lCurveRX->KeySetValue(lKeyIndex, cureul.x);
			lCurveRX->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
			befeul = cureul;
			befq = curq;

			s_writefile->Write2File("%d %.4f linear linear 1 1 0;\n", frameno * s_itscale, cureul.x);
		}
		lCurveRX->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");

		////////////////////
		s_writefile->Write2File("anim rotate.rotateY rotateY %s %d 1 1;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRY;
		lCurveRY = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Y, true);
		lCurveRY->KeyModifyBegin();
		befeul = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		befq.SetParams(1.0f, 0.0f, 0.0f, 0.0f);
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			//ret = curmi->CalcMotionPointOnFrame( &axisq, curbone, &calcmp, curmotid, frameno, &hasmpflag );
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			calcmp.m_q.CalcFBXEul(befq, befeul, &cureul);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRY->KeyAdd(lTime);
			lCurveRY->KeySetValue(lKeyIndex, cureul.y);
			lCurveRY->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
			befeul = cureul;
			befq = curq;
			s_writefile->Write2File("%d %.4f linear linear 1 1 0;\n", frameno * s_itscale, cureul.y);
		}
		lCurveRY->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");


		s_writefile->Write2File("anim rotate.rotateZ rotateZ %s %d 1 1;\n", bonename, jointcnt);
		s_writefile->Write2File("animData {\n");
		s_writefile->Write2File(" input time;\n");
		s_writefile->Write2File(" output angular;\n");
		s_writefile->Write2File(" weighted 0;\n");
		s_writefile->Write2File(" preInfinity constant;\n");
		s_writefile->Write2File(" postInfinity constant;\n");
		s_writefile->Write2File(" keys {\n");
		KFbxAnimCurve* lCurveRZ;
		lCurveRZ = lSkel->LclRotation.GetCurve<KFbxAnimCurve>(lAnimLayer, KFCURVENODE_R_Z, true);
		lCurveRZ->KeyModifyBegin();
		befq.SetParams(1.0f, 0.0f, 0.0f, 0.0f);
		befeul = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (frameno = 0; frameno <= motmax; frameno++){
			int hasmpflag = 0;
			//ret = curmi->CalcMotionPointOnFrame( &axisq, curbone, &calcmp, curmotid, frameno, &hasmpflag );
			ret = curmc->CalcMotionPointOnFrame(curbone, &calcmp, curmotid, frameno, &hasmpflag, s_lpsh, s_lpmh);
			_ASSERT(!ret);
			curq = calcmp.m_q.CalcFBXEul(befq, befeul, &cureul);
			lTime.SetSecondDouble((double)frameno / s_timescale);
			lKeyIndex = lCurveRZ->KeyAdd(lTime);
			lCurveRZ->KeySetValue(lKeyIndex, cureul.z);
			lCurveRZ->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
			befeul = cureul;
			befq = curq;
			s_writefile->Write2File("%d %.4f linear linear 1 1 0;\n", frameno * s_itscale, cureul.z);
		}
		lCurveRZ->KeyModifyEnd();
		s_writefile->Write2File(" }\n");
		s_writefile->Write2File("}\n");


		jointcnt++;
	}

		
	if( fbxbone->m_child ){
		AnimateBoneReq( fbxbone->m_child, lAnimLayer, curmotid, motmax, jointcnt );
	}
	if( fbxbone->m_brother ){
		AnimateBoneReq( fbxbone->m_brother, lAnimLayer, curmotid, motmax, jointcnt );
	}
}


int ExistBoneInInfPM2( int boneno, CPolyMesh2* pm2, MATERIALBLOCK* pmb, int totalfacenum )
{
	int dirtyflag = 0;

	CShdElem* curse = (*s_lpsh)( boneno );

	/*
	int vsetno;
	for (vsetno = 0; vsetno < pm2->optpleng; vsetno++){
		CInfElem* curie = pm2->m_IE + vsetno;
		int id = curie->GetInfElemByBone(boneno);
		if (id >= 0){
			dirtyflag = 1;
			break;
		}
	}
	*/

	
	int fno;
	int chkfaceno = totalfacenum;
	for( fno = pmb->startface; fno < pmb->endface; fno++ ){
		int vno[3];
		vno[0] = *(pm2->m_optindexbuf2 + fno * 3);
		vno[2] = *(pm2->m_optindexbuf2 + fno * 3 + 1);
		vno[1] = *(pm2->m_optindexbuf2 + fno * 3 + 2);

		int valid0, valid1, valid2;
		valid0 = ValidatePm2Index(pm2, vno[0]);
		valid1 = ValidatePm2Index(pm2, vno[1]);
		valid2 = ValidatePm2Index(pm2, vno[2]);
		_ASSERT((valid0 == 1) && (valid1 == 1) && (valid2 == 1));

		int vcnt;
		for( vcnt = 0; vcnt < 3; vcnt++ ){
			CInfElem* curie = pm2->m_IE + vno[vcnt];
			int id = curie->GetInfElemByBone( boneno );
			if( id >= 0 ){
				dirtyflag = 1;
				break;
			}
		}

		chkfaceno++;
	}
	
	return dirtyflag;
}
int ExistBoneInInfPM( int boneno, CPolyMesh* pm, int facenum )
{
	int dirtyflag = 0;

	int fno;
	for( fno = 0; fno < facenum; fno++ ){
		int vno[3];
		vno[0] = *(pm->m_indexbuf2 + fno * 3);
		vno[2] = *(pm->m_indexbuf2 + fno * 3 + 1);
		vno[1] = *(pm->m_indexbuf2 + fno * 3 + 2);

		int vcnt;
		for( vcnt = 0; vcnt < 3; vcnt++ ){
			CInfElem* curie = pm->m_IE + vno[vcnt];
			int id = curie->GetInfElemByBone( boneno );
			if( id >= 0 ){
				dirtyflag = 1;
				break;
			}
		}
	}
	return dirtyflag;
}

int ConvName2English( int motid, char* srcptr, char* dstptr, int dstleng )
{
	
	int textleng;
	textleng = (int)strlen( srcptr );
	if( (textleng <= 0) || (textleng > dstleng) ){
		sprintf_s( dstptr, dstleng, "EnglishName%d", motid );
		return 0;//!!!!!!!!!!!!!!!!
	}


	char chkchar;
	int no;
	int finderr = 0;
	for( no = 0; no < textleng; no++ ){
		chkchar = *(srcptr + no);
		if( !((chkchar >= 'a') && (chkchar <= 'z')) && !((chkchar >= 'A') && (chkchar <= 'Z')) && 
			!((chkchar >= '0') && (chkchar <= '9')) && (chkchar != '_' ) ){
			finderr = 1;
			break;
		}
	}

	if( finderr ){
		sprintf_s( dstptr, dstleng, "EnglishName%d", motid );
	}else{
		strcpy_s( dstptr, dstleng, srcptr );
	}

	return 0;
}

int MapShapesOnMeshPM2( KFbxScene* pScene, KFbxNode* pNode, CMorph* morph, MATERIALBLOCK* pmb, int mbno, BLSINDEX* blsindex )
{
	CShdElem* base = morph->m_baseelem;
	CTreeElem2* basete = (*s_lpth)( base->serialno );
	_ASSERT( basete );

	if( morph->m_targetnum <= 0 ){
		return 0;
	}

	char blsname[256] = {0};
	sprintf_s( blsname, 256, "BLS_%s_%d", basete->engname, mbno );
	KFbxBlendShape* lBlendShape = KFbxBlendShape::Create(pScene, blsname);

	(blsindex->channelno) = 0;

	int targetcnt;
	for( targetcnt = 0; targetcnt < morph->m_targetnum; targetcnt++ ){
		CShdElem* targetse = (morph->m_ptarget + targetcnt)->selem;
		CTreeElem2* targette = (*s_lpth)( targetse->serialno );

		char blscname[256] = {0};
		sprintf_s( blscname, 256, "BLSC_%s_%d", targette->engname, mbno );
		KFbxBlendShapeChannel* lBlendShapeChannel = KFbxBlendShapeChannel::Create(pScene,blscname);
		MapTargetShapePM2( lBlendShapeChannel, pScene, morph, targetse, pmb, mbno );
		lBlendShape->AddBlendShapeChannel(lBlendShapeChannel);
		
		BLSINFO blsinfo;
		ZeroMemory( &blsinfo, sizeof( BLSINFO ) );

		blsinfo.blsindex = *blsindex;
		blsinfo.morph = morph;
		blsinfo.target = targetse;
		blsinfo.basenode = pNode;

		s_blsinfo[ blsindex->serialno ] = blsinfo;

		(blsindex->channelno)++;
		(blsindex->serialno)++;

	}

	KFbxGeometry* lGeometry = pNode->GetGeometry();
	lGeometry->AddDeformer(lBlendShape);

	return 0;
}

int MapTargetShapePM2( KFbxBlendShapeChannel* lBlendShapeChannel, KFbxScene* pScene, CMorph* morph, CShdElem* targetse, MATERIALBLOCK* pmb, int mbno )
{
	CTreeElem2* targette = (*s_lpth)( targetse->serialno );

	char shapename[256]={0};
	sprintf_s( shapename, 256, "SHAPE_%s_%d", targette->engname, mbno );
    KFbxShape* lShape = KFbxShape::Create(pScene,shapename);

	CShdElem* base = morph->m_baseelem;
	CPolyMesh2* basepm2 = base->polymesh2;

	CPolyMesh2* targetpm2 = targetse->polymesh2;

	CD3DDisp* tard3ddisp = targetse->d3ddisp;

	int cpnum = ( pmb->endface - pmb->startface ) * 3;
    lShape->InitControlPoints(cpnum);
    KFbxVector4* lVector4 = lShape->GetControlPoints();


//ControllPointに対応させる必要あり　！！！！！

	int facecnt = 0;
	int faceno;

	for( faceno = pmb->startface; faceno < pmb->endface; faceno++ ){
		int vno[3];
		vno[0] = *(targetpm2->m_optindexbuf2 + faceno * 3);
		vno[2] = *(targetpm2->m_optindexbuf2 + faceno * 3 + 1);
		vno[1] = *(targetpm2->m_optindexbuf2 + faceno * 3 + 2);

		_ASSERT( (vno[0] >= 0) && (vno[0] < targetpm2->optpleng) );
		_ASSERT( (vno[1] >= 0) && (vno[1] < targetpm2->optpleng) );
		_ASSERT( (vno[2] >= 0) && (vno[2] < targetpm2->optpleng) );

		int vcnt;
		for( vcnt = 0; vcnt < 3; vcnt++ ){
			SKINVERTEX* skinv = tard3ddisp->m_skinv + vno[vcnt];
			lVector4[ facecnt * 3 + vcnt ].Set( skinv->pos[0] * s_fbxmult, skinv->pos[1] * s_fbxmult, -skinv->pos[2] * s_fbxmult, 1.0 );
		}
		facecnt++;
	}

	lBlendShapeChannel->AddTargetShape(lShape);

	return 0;
}

int AnimateMorph(KFbxScene* pScene)
{
    KTime lTime;
    int lKeyIndex = 0;

	if( s_blsinfo.empty() ){
		return 0;
	}

	if( s_ainum <= 0 ){
		return 0;
	}


	int aino;
	for( aino = 0; aino < s_ainum; aino++ ){
		ANIMINFO* curai = s_ai + aino;
		int curmotid = curai->motid;
		int maxframe = curai->maxframe;
		KFbxAnimLayer* lAnimLayer = curai->animlayer;

		map<int, BLSINFO>::iterator itrblsinfo;
		for( itrblsinfo = s_blsinfo.begin(); itrblsinfo != s_blsinfo.end(); itrblsinfo++ ){
			BLSINFO curinfo = itrblsinfo->second;

			CMotionCtrl* bonemc = curinfo.morph->m_boneelem;
			CMotionInfo* motinfo = bonemc->motinfo;

			KFbxGeometry* lNurbsAttribute = (KFbxGeometry*)(curinfo.basenode)->GetNodeAttribute();
			KFbxAnimCurve* lCurve = lNurbsAttribute->GetShapeChannel(0, curinfo.blsindex.channelno, lAnimLayer, true);
			if (lCurve)
			{
				lCurve->KeyModifyBegin();

				int frameno;
				for( frameno = 0; frameno <= maxframe; frameno++ ){
					double dframe = (double)frameno;

					int existflag = 0;
					float morphval = 0.0f;
					motinfo->GetMMotAnim( &morphval, curmotid, frameno, curinfo.morph->m_baseelem, curinfo.target, &existflag );

					lTime.SetSecondDouble(dframe / s_timescale);
					lKeyIndex = lCurve->KeyAdd(lTime);
					lCurve->KeySetValue(lKeyIndex, morphval * 100.0f);
					lCurve->KeySetInterpolation(lKeyIndex, KFbxAnimCurveDef::eINTERPOLATION_LINEAR);
				}

				lCurve->KeyModifyEnd();
			}
		}
	}

	return 0;
}

int WriteBindPose(KFbxScene* pScene)
{

	KFbxPose* lPose = KFbxPose::Create(pScene,"BindPose0");
	lPose->SetIsBindPose(true);

	if( s_firstoutmot >= 0 ){
		KFbxAnimStack * lCurrentAnimationStack = pScene->FindMember(FBX_TYPE(KFbxAnimStack), s_ai->engmotname);
		if (lCurrentAnimationStack == NULL)
		{
			_ASSERT( 0 );
			return 1;
		}
		KFbxAnimLayer * mCurrentAnimLayer;
		mCurrentAnimLayer = lCurrentAnimationStack->GetMember(FBX_TYPE(KFbxAnimLayer), 0);
		pScene->GetEvaluator()->SetContext(lCurrentAnimationStack);
	}else{
		_ASSERT( 0 );
	}

	//s_lpmh->SetMotionFrameNo(s_lpsh, s_firstoutmot, 0, 1);

	WriteBindPoseReq( s_fbxbone, lPose );

	pScene->AddPose(lPose);

	return 0;
}

void WriteBindPoseReq( CFBXBone* fbxbone, KFbxPose* lPose )
{
	//KTime lTime0;
	//lTime0.SetSecondDouble( 0.0 );
	
	if( fbxbone->type != FB_ROOT ){
		KFbxNode* curskel = fbxbone->skelnode;
		if( curskel ){
			D3DXMATRIX tramat = CalcBindMatrixZA(fbxbone, s_firstoutmot);
			KFbxMatrix lBindMatrix;
			//KFbxMatrix lBindMatrix = curskel->EvaluateGlobalTransform( lTime0 );
			lBindMatrix.SetIdentity();
			lBindMatrix[0][0] = tramat._11;
			lBindMatrix[0][1] = tramat._12;
			lBindMatrix[0][2] = tramat._13;
			lBindMatrix[0][3] = tramat._14;
			lBindMatrix[1][0] = tramat._21;
			lBindMatrix[1][1] = tramat._22;
			lBindMatrix[1][2] = tramat._23;
			lBindMatrix[1][3] = tramat._24;
			lBindMatrix[2][0] = tramat._31;
			lBindMatrix[2][1] = tramat._32;
			lBindMatrix[2][2] = tramat._33;
			lBindMatrix[2][3] = tramat._34;
			lBindMatrix[3][0] = tramat._41;
			lBindMatrix[3][1] = tramat._42;
			lBindMatrix[3][2] = tramat._43;
			lBindMatrix[3][3] = tramat._44;

			lPose->Add(curskel, lBindMatrix);
		}
	}

	if( fbxbone->m_child ){
		WriteBindPoseReq( fbxbone->m_child, lPose );
	}
	if( fbxbone->m_brother ){
		WriteBindPoseReq( fbxbone->m_brother, lPose );
	}
}


int DestroyFBXBoneReq( CFBXBone* fbxbone )
{
	CFBXBone* chilbone = fbxbone->m_child;
	CFBXBone* brobone = fbxbone->m_brother;

	delete fbxbone;

	if( chilbone ){
		DestroyFBXBoneReq( chilbone );
	}
	if( brobone ){
		DestroyFBXBoneReq( brobone );
	}

	return 0;
}

CFBXBone* CreateFBXBone( KFbxScene* pScene )
{
	s_fbxbonenum = 0;
	CFBXBone* retfbxbone = 0;

	KFbxNode* lSkeletonRoot;
	// Create skeleton root. 
    KString lRootName( "RootNode" );
    KFbxSkeleton* lSkeletonRootAttribute = KFbxSkeleton::Create(pScene, lRootName);
    lSkeletonRootAttribute->SetSkeletonType(KFbxSkeleton::eROOT);
    lSkeletonRoot = KFbxNode::Create(pScene,lRootName.Buffer());
    lSkeletonRoot->SetNodeAttribute(lSkeletonRootAttribute);    
    lSkeletonRoot->LclTranslation.Set(KFbxVector4(0.0, 0.0, 0.0));
	retfbxbone = new CFBXBone();
	if( !retfbxbone ){
		_ASSERT( 0 );
		return 0;
	}
	retfbxbone->type = FB_ROOT;
	retfbxbone->selem = 0;
	retfbxbone->skelnode = lSkeletonRoot;
	s_fbxbonenum++;

	int firsttopflag = 1;
	int topno;
	for( topno = 0; topno < s_topnum; topno++ ){
		CShdElem* topj = s_topjoint[topno];
		_ASSERT( topj );
		int topseri = topj->serialno;
		CTreeElem2* topte = (*s_lpth)( topseri );
		_ASSERT( topte );
		CPart* toppart = topj->part;
		_ASSERT( toppart );

		//if (topj->notuse == 0){
			KFbxNode* lSkeletonNode;
			KString lNodeName(topte->engname);
			KFbxSkeleton* lSkeletonNodeAttribute = KFbxSkeleton::Create(pScene, lNodeName);
			lSkeletonNodeAttribute->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonNode = KFbxNode::Create(pScene, lNodeName.Buffer());
			lSkeletonNode->SetNodeAttribute(lSkeletonNodeAttribute);
			lSkeletonNode->LclTranslation.Set(KFbxVector4(toppart->jointloc.x * s_fbxmult, toppart->jointloc.y * s_fbxmult, -toppart->jointloc.z * s_fbxmult));
			//lSkeletonNode->LclTranslation.Set(KFbxVector4(0.0, 0.0, 0.0));

			//_ASSERT(0);
			CFBXBone* fbxbone = new CFBXBone();
			if (!fbxbone){
				_ASSERT(0);
				return 0;
			}
			fbxbone->type = FB_NORMAL;
			fbxbone->selem = topj;
			fbxbone->skelnode = lSkeletonNode;

			if (firsttopflag == 1){
				s_firsttopbone = fbxbone;
				firsttopflag = 0;
			}

			retfbxbone->skelnode->AddChild(lSkeletonNode);
			retfbxbone->AddChild(fbxbone);
			s_fbxbonenum++;

			if (topj->child){
				CreateFBXBoneReq(pScene, topj->child, fbxbone);
			}
		//}
	}

	return retfbxbone;
}

void CreateFBXBoneReq(KFbxScene* pScene, CShdElem* pbone, CFBXBone* parfbxbone)
{


	CTreeElem2* te = (*s_lpth)(pbone->serialno);
	_ASSERT(te);
	CPart* part = pbone->part;
	_ASSERT(part);
	CTreeElem2* parte = (*s_lpth)(pbone->parent->serialno);
	_ASSERT(parte);
	CPart* parpart = pbone->parent->part;
	_ASSERT(parpart);

	char curname[256];
	char parname[256];
	strcpy_s(curname, 256, te->engname);
	strcpy_s(parname, 256, parte->engname);
	char* curjointnameptr;
	char* parjointnameptr;
	curjointnameptr = strstr(curname, "_Joint");
	if (curjointnameptr){
		*curjointnameptr = 0;
	}
	parjointnameptr = strstr(parname, "_Joint");
	if (parjointnameptr){
		*parjointnameptr = 0;
	}


	CShdElem* gparbone = pbone->parent->parent;
	CPart* gparpart = 0;
	if (gparbone && gparbone->IsJoint()){
		gparpart = gparbone->part;
	}
	else{
		gparpart = 0;
	}

	CFBXBone* fbxbone = new CFBXBone();
	if (!fbxbone){
		_ASSERT(0);
		return;
	}

	static int s_bunkicnt = 0;
	//if (pbone->notuse == 0){
		if ((s_createbunkiflag == 1) && parpart && (parpart->bonenum >= 2)){


			CFBXBone* fbxbone2 = new CFBXBone();
			if (!fbxbone2){
				_ASSERT(0);
				return;
			}
			char newname2[256] = { 0 };
			sprintf_s(newname2, 256, "%s_%s_bunki_Joint", parname, curname);

			KString lLimbNodeName2(newname2);
			KFbxSkeleton* lSkeletonLimbNodeAttribute2 = KFbxSkeleton::Create(pScene, lLimbNodeName2);
			lSkeletonLimbNodeAttribute2->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonLimbNodeAttribute2->Size.Set(1.0);
			KFbxNode* lSkeletonLimbNode2 = KFbxNode::Create(pScene, lLimbNodeName2.Buffer());
			lSkeletonLimbNode2->SetNodeAttribute(lSkeletonLimbNodeAttribute2);
			lSkeletonLimbNode2->LclTranslation.Set(KFbxVector4(0.0, 0.0, 0.0));
			parfbxbone->skelnode->AddChild(lSkeletonLimbNode2);

			fbxbone2->selem = pbone->parent;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			fbxbone2->skelnode = lSkeletonLimbNode2;
			parfbxbone->AddChild(fbxbone2);

			fbxbone2->type = FB_BUNKI_CHIL;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			s_fbxbonenum++;

			char newname[256] = { 0 };
			sprintf_s(newname, 256, "%s_%s_Joint", parname, curname);


			KString lLimbNodeName1(newname);
			KFbxSkeleton* lSkeletonLimbNodeAttribute1 = KFbxSkeleton::Create(pScene, lLimbNodeName1);
			lSkeletonLimbNodeAttribute1->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonLimbNodeAttribute1->Size.Set(1.0);
			KFbxNode* lSkeletonLimbNode1 = KFbxNode::Create(pScene, lLimbNodeName1.Buffer());
			lSkeletonLimbNode1->SetNodeAttribute(lSkeletonLimbNodeAttribute1);
			if (gparpart){
				lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult - gparpart->jointloc.x * s_fbxmult,
					parpart->jointloc.y * s_fbxmult - gparpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult - -gparpart->jointloc.z * s_fbxmult));
			}
			else{
				//lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult, part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
				lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult, parpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult));
			}
			fbxbone2->skelnode->AddChild(lSkeletonLimbNode1);

			fbxbone->selem = pbone;
			fbxbone->skelnode = lSkeletonLimbNode1;
			fbxbone2->AddChild(fbxbone);
			fbxbone->type = FB_NORMAL;
			s_fbxbonenum++;


			if (!pbone->child){
				//endjointを作る
				char endname[256];
				sprintf_s(endname, 256, "%s_Joint", curname);

				KString lLimbNodeName3(endname);
				KFbxSkeleton* lSkeletonLimbNodeAttribute3 = KFbxSkeleton::Create(pScene, lLimbNodeName3);
				lSkeletonLimbNodeAttribute3->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
				lSkeletonLimbNodeAttribute3->Size.Set(1.0);
				KFbxNode* lSkeletonLimbNode3 = KFbxNode::Create(pScene, lLimbNodeName3.Buffer());
				lSkeletonLimbNode3->SetNodeAttribute(lSkeletonLimbNodeAttribute3);

				lSkeletonLimbNode3->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult,
					part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
				lSkeletonLimbNode2->AddChild(lSkeletonLimbNode3);

				CFBXBone* fbxbone3 = new CFBXBone();
				if (!fbxbone3){
					_ASSERT(0);
					return;
				}
				fbxbone3->type = FB_ENDJOINT;
				fbxbone->skelnode->AddChild(lSkeletonLimbNode3);
				s_fbxbonenum++;

				fbxbone3->selem = pbone;
				fbxbone3->skelnode = lSkeletonLimbNode3;
				fbxbone->AddChild(fbxbone3);
			}

		}
		else{
			//par中心回転
			char newname[256] = { 0 };
			sprintf_s(newname, 256, "%s_%s_Joint", parname, curname);

			KString lLimbNodeName1(newname);
			KFbxSkeleton* lSkeletonLimbNodeAttribute1 = KFbxSkeleton::Create(pScene, lLimbNodeName1);
			lSkeletonLimbNodeAttribute1->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonLimbNodeAttribute1->Size.Set(1.0);
			KFbxNode* lSkeletonLimbNode1 = KFbxNode::Create(pScene, lLimbNodeName1.Buffer());
			lSkeletonLimbNode1->SetNodeAttribute(lSkeletonLimbNodeAttribute1);
			if (gparpart){
				lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult - gparpart->jointloc.x * s_fbxmult,
					parpart->jointloc.y * s_fbxmult - gparpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult - -gparpart->jointloc.z * s_fbxmult));
			}
			else{
				lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult, parpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult));
				//lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult,
				//	part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
			}
			parfbxbone->skelnode->AddChild(lSkeletonLimbNode1);

			fbxbone->selem = pbone;
			fbxbone->skelnode = lSkeletonLimbNode1;
			parfbxbone->AddChild(fbxbone);
			fbxbone->type = FB_NORMAL;
			s_fbxbonenum++;


			if (!pbone->child){
				//endjointを作る 位置情報
				char endname[256];
				sprintf_s(endname, 256, "%s_Joint", curname);

				KString lLimbNodeName3(endname);
				KFbxSkeleton* lSkeletonLimbNodeAttribute3 = KFbxSkeleton::Create(pScene, lLimbNodeName3);
				lSkeletonLimbNodeAttribute3->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
				lSkeletonLimbNodeAttribute3->Size.Set(1.0);
				KFbxNode* lSkeletonLimbNode3 = KFbxNode::Create(pScene, lLimbNodeName3.Buffer());
				lSkeletonLimbNode3->SetNodeAttribute(lSkeletonLimbNodeAttribute3);

				lSkeletonLimbNode3->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult,
					part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
				lSkeletonLimbNode1->AddChild(lSkeletonLimbNode3);

				CFBXBone* fbxbone3 = new CFBXBone();
				if (!fbxbone3){
					_ASSERT(0);
					return;
				}
				fbxbone3->type = FB_ENDJOINT;
				fbxbone->skelnode->AddChild(lSkeletonLimbNode3);
				s_fbxbonenum++;

				fbxbone3->selem = pbone;
				fbxbone3->skelnode = lSkeletonLimbNode3;
				fbxbone->AddChild(fbxbone3);
			}

		}
	//}
	if (pbone->brother){
		CreateFBXBoneReq(pScene, pbone->brother, parfbxbone);
	}

	//if ((pbone->notuse == 0) && pbone->child){
	if (pbone->child){
		CreateFBXBoneReq(pScene, pbone->child, fbxbone);
	}


}


/*
void CreateFBXBoneReq( KFbxScene* pScene, CShdElem* pbone, CFBXBone* parfbxbone )
{
	CTreeElem2* te = (*s_lpth)( pbone->serialno );
	_ASSERT( te );
	CPart* part = pbone->part;
	_ASSERT( part );
	CTreeElem2* parte = (*s_lpth)( pbone->parent->serialno );
	_ASSERT(parte);
	CPart* parpart = parfbxbone->selem->part;
	_ASSERT( parpart );

	CShdElem* gparbone = parfbxbone->selem->parent;
	CPart* gparpart = 0;
	CShdElem* ggparbone = 0;
	CPart* ggparpart = 0;
	if( gparbone && gparbone->IsJoint() ){
		gparpart = gparbone->part;
		if( gparbone->parent && gparbone->parent->IsJoint() ){
			ggparbone = gparbone->parent;
			ggparpart = ggparbone->part;
		}
	}else{
		gparpart = 0;
	}

	static int s_bunkicnt = 0;
	if (parpart && (parpart->bonenum >= 2)){
		CFBXBone* fbxbone2 = new CFBXBone();
		if (!fbxbone2){
			_ASSERT(0);
			return;
		}
		char newname2[256] = { 0 };
		sprintf_s(newname2, 256, "%s_bunki%d", parte->engname, s_bunkicnt++);

		KString lLimbNodeName2(newname2);
		KFbxSkeleton* lSkeletonLimbNodeAttribute2 = KFbxSkeleton::Create(pScene, lLimbNodeName2);
		lSkeletonLimbNodeAttribute2->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
		lSkeletonLimbNodeAttribute2->Size.Set(1.0);
		KFbxNode* lSkeletonLimbNode2 = KFbxNode::Create(pScene, lLimbNodeName2.Buffer());
		lSkeletonLimbNode2->SetNodeAttribute(lSkeletonLimbNodeAttribute2);
		lSkeletonLimbNode2->LclTranslation.Set(KFbxVector4(0.0, 0.0, 0.0));
		parfbxbone->skelnode->AddChild(lSkeletonLimbNode2);

		fbxbone2->selem = pbone->parent;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		fbxbone2->skelnode = lSkeletonLimbNode2;
		parfbxbone->AddChild(fbxbone2);

		fbxbone2->type = FB_BUNKI_CHIL;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//if (parte){
		//	parfbxbone->type = FB_BUNKI_PAR;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//}

		CFBXBone* fbxbone = new CFBXBone();
		if (!fbxbone){
			_ASSERT(0);
			return;
		}
		char newname[256] = { 0 };
		sprintf_s(newname, 256, "%s", te->engname);


		KString lLimbNodeName1(newname);
		KFbxSkeleton* lSkeletonLimbNodeAttribute1 = KFbxSkeleton::Create(pScene, lLimbNodeName1);
		lSkeletonLimbNodeAttribute1->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
		lSkeletonLimbNodeAttribute1->Size.Set(1.0);
		KFbxNode* lSkeletonLimbNode1 = KFbxNode::Create(pScene, lLimbNodeName1.Buffer());
		lSkeletonLimbNode1->SetNodeAttribute(lSkeletonLimbNodeAttribute1);
		if (gparpart){
			lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult - gparpart->jointloc.x * s_fbxmult,
				parpart->jointloc.y * s_fbxmult - gparpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult - -gparpart->jointloc.z * s_fbxmult));
		}
		else{
			lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult, parpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult));
		}
		fbxbone2->skelnode->AddChild(lSkeletonLimbNode1);

		fbxbone->selem = pbone;
		fbxbone->skelnode = lSkeletonLimbNode1;
		fbxbone2->AddChild(fbxbone);
		fbxbone->type = FB_NORMAL;

		
		if (!pbone->child){
			//endjointを作る
			char endname[256];
			sprintf_s(endname, 256, "%s_end", te->engname);

			KString lLimbNodeName3(endname);
			KFbxSkeleton* lSkeletonLimbNodeAttribute3 = KFbxSkeleton::Create(pScene, lLimbNodeName3);
			lSkeletonLimbNodeAttribute3->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonLimbNodeAttribute3->Size.Set(1.0);
			KFbxNode* lSkeletonLimbNode3 = KFbxNode::Create(pScene, lLimbNodeName3.Buffer());
			lSkeletonLimbNode3->SetNodeAttribute(lSkeletonLimbNodeAttribute3);

			lSkeletonLimbNode3->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult,
				part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
			lSkeletonLimbNode2->AddChild(lSkeletonLimbNode3);

			CFBXBone* fbxbone3 = new CFBXBone();
			if (!fbxbone3){
				_ASSERT(0);
				return;
			}
			fbxbone3->type = FB_ENDJOINT;
			fbxbone->skelnode->AddChild(lSkeletonLimbNode3);

			fbxbone3->selem = pbone;
			fbxbone3->skelnode = lSkeletonLimbNode3;
			fbxbone->AddChild(fbxbone3);
		}
		


		if (pbone->brother){
			CreateFBXBoneReq(pScene, pbone->brother, parfbxbone);
		}

		if (pbone->child){
			CreateFBXBoneReq(pScene, pbone->child, fbxbone);
		}

	}
	else{
		//par中心回転
		CFBXBone* fbxbone = new CFBXBone();
		if (!fbxbone){
			_ASSERT(0);
			return;
		}
		char newname[256] = { 0 };
		sprintf_s(newname, 256, "%s", te->engname);

		KString lLimbNodeName1(newname);
		KFbxSkeleton* lSkeletonLimbNodeAttribute1 = KFbxSkeleton::Create(pScene, lLimbNodeName1);
		lSkeletonLimbNodeAttribute1->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
		lSkeletonLimbNodeAttribute1->Size.Set(1.0);
		KFbxNode* lSkeletonLimbNode1 = KFbxNode::Create(pScene, lLimbNodeName1.Buffer());
		lSkeletonLimbNode1->SetNodeAttribute(lSkeletonLimbNodeAttribute1);
		if (gparpart){
			lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult - gparpart->jointloc.x * s_fbxmult,
				parpart->jointloc.y * s_fbxmult - gparpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult - -gparpart->jointloc.z * s_fbxmult));
		}
		else{
			lSkeletonLimbNode1->LclTranslation.Set(KFbxVector4(parpart->jointloc.x * s_fbxmult, parpart->jointloc.y * s_fbxmult, -parpart->jointloc.z * s_fbxmult));
		}
		parfbxbone->skelnode->AddChild(lSkeletonLimbNode1);

		fbxbone->selem = pbone;
		fbxbone->skelnode = lSkeletonLimbNode1;
		parfbxbone->AddChild(fbxbone);
		fbxbone->type = FB_NORMAL;

		
		if (!pbone->child){
			//endjointを作る 位置情報
			char endname[256];
			sprintf_s(endname, 256, "%s_end", te->engname);

			KString lLimbNodeName3(endname);
			KFbxSkeleton* lSkeletonLimbNodeAttribute3 = KFbxSkeleton::Create(pScene, lLimbNodeName3);
			lSkeletonLimbNodeAttribute3->SetSkeletonType(KFbxSkeleton::eLIMB_NODE);
			lSkeletonLimbNodeAttribute3->Size.Set(1.0);
			KFbxNode* lSkeletonLimbNode3 = KFbxNode::Create(pScene, lLimbNodeName3.Buffer());
			lSkeletonLimbNode3->SetNodeAttribute(lSkeletonLimbNodeAttribute3);

			lSkeletonLimbNode3->LclTranslation.Set(KFbxVector4(part->jointloc.x * s_fbxmult - parpart->jointloc.x * s_fbxmult,
				part->jointloc.y * s_fbxmult - parpart->jointloc.y * s_fbxmult, -part->jointloc.z * s_fbxmult - -parpart->jointloc.z * s_fbxmult));
			lSkeletonLimbNode1->AddChild(lSkeletonLimbNode3);

			CFBXBone* fbxbone3 = new CFBXBone();
			if (!fbxbone3){
				_ASSERT(0);
				return;
			}
			fbxbone3->type = FB_ENDJOINT;
			fbxbone->skelnode->AddChild(lSkeletonLimbNode3);

			fbxbone3->selem = pbone;
			fbxbone3->skelnode = lSkeletonLimbNode3;
			fbxbone->AddChild(fbxbone3);
		}
		

		if (pbone->brother){
			CreateFBXBoneReq(pScene, pbone->brother, parfbxbone);
		}

		if (pbone->child){
			CreateFBXBoneReq(pScene, pbone->child, fbxbone);
		}
	}

}
*/
void CreateDummyInfDataReq( CFBXBone* fbxbone, KFbxSdkManager*& pSdkManager, KFbxScene*& pScene, KFbxNode* lMesh, KFbxSkin* lSkin, int* bonecnt, CShdElem** ppsetbone )
{
	CShdElem* curbone = fbxbone->selem;
	//if( curbone && (fbxbone->m_boneinfcnt == 0) ){
	if (curbone && curbone->IsJoint()){
		if (*(ppsetbone + curbone->serialno) == 0){
			LinkDummyMeshToSkeleton(fbxbone, lSkin, pScene, lMesh, *bonecnt);
			(*bonecnt)++;
		}
	}

	if( fbxbone->m_child ){
		CreateDummyInfDataReq( fbxbone->m_child, pSdkManager, pScene, lMesh, lSkin, bonecnt, ppsetbone );
	}

	if( fbxbone->m_brother ){
		CreateDummyInfDataReq( fbxbone->m_brother, pSdkManager, pScene, lMesh, lSkin, bonecnt, ppsetbone );
	}

}
KFbxNode* CreateDummyFbxMesh(KFbxSdkManager* pSdkManager, KFbxScene* pScene, CShdElem** ppsetbone)
{
	static int s_namecnt = 0;

	char meshname[256] = {0};
	char nodename[256] = { 0 };
	sprintf_s(meshname, 256, "_ND_dtri%d", s_namecnt);
	sprintf_s(nodename, 256, "_ND_dtri%d", s_namecnt);
	s_namecnt++;


	int notdirtynum = 0;
	int boneno;
	for (boneno = 0; boneno < s_lpsh->s2shd_leng; boneno++){
		CShdElem* selem = (*s_lpsh)(boneno);
		if (selem && selem->IsJoint()){
			if (*(ppsetbone + boneno) == 0){
				notdirtynum++;
			}
		}
	}
	if (notdirtynum <= 0){
		return NULL;
	}


	//int facenum = s_fbxbonenum;
	int facenum = notdirtynum;

	KFbxMesh* lMesh = KFbxMesh::Create(pScene, meshname);
	//lMesh->InitControlPoints(facenum * 3);
	lMesh->InitControlPoints(facenum * 3);
	KFbxVector4* lcp = lMesh->GetControlPoints();


	KFbxGeometryElementNormal* lElementNormal = lMesh->CreateElementNormal();
	//lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	lElementNormal->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	lElementNormal->SetReferenceMode(KFbxGeometryElement::eDIRECT);

	KFbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
	K_ASSERT(lUVDiffuseElement != NULL);
	//lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_CONTROL_POINT);
	lUVDiffuseElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON_VERTEX);
	lUVDiffuseElement->SetReferenceMode(KFbxGeometryElement::eDIRECT);

	//int vsetno;
	//for (vsetno = 0; vsetno < facenum * 3; vsetno++){
	//	*(lcp + vsetno) = KFbxVector4(0.0f, 0.0f, 0.0f, 1.0f);

	//	KFbxVector2 fbxuv = KFbxVector2(0.0f, 0.0f);
	//	lUVDiffuseElement->GetDirectArray().Add(fbxuv);

	//	KFbxVector4 fbxn = KFbxVector4(1.0f, 0.0f, 0.0f, 0.0f);
	//	lElementNormal->GetDirectArray().Add(fbxn);
	//}

	////lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);

	int faceno;
	for (faceno = 0; faceno < facenum; faceno++) {
		int srcno[3];
		srcno[0] = faceno * 3;
		srcno[1] = faceno * 3 + 2;
		srcno[2] = faceno * 3 + 1;

		int vcnt;
		for (vcnt = 0; vcnt < 3; vcnt++) {
			//0 2 1のインデックス順で書き出して　読み込み時の　０　１　２の順に直す
			*(lcp + srcno[vcnt]) = KFbxVector4(0.0f, 0.0f, 0.0f, 1.0);
			KFbxVector2 fbxuv = KFbxVector2(0.0f, 0.0f);
			lUVDiffuseElement->GetDirectArray().Add(fbxuv);

			KFbxVector4 fbxn = KFbxVector4(1.0f, 0.0f, 0.0f, 0.0f);
			lElementNormal->GetDirectArray().Add(fbxn);
		}

	}
	lUVDiffuseElement->GetIndexArray().SetCount(facenum * 3);


	for (faceno = 0; faceno < facenum; faceno++){
		int srcno[3];
		srcno[0] = faceno * 3;
		srcno[1] = faceno * 3 + 2;
		srcno[2] = faceno * 3 + 1;

		lMesh->BeginPolygon(-1, -1, -1, false);
		int i;
		for (i = 0; i < 3; i++)
		{
			// Control point index
			lMesh->AddPolygon(srcno[i]);
		}
		lMesh->EndPolygon();
	}


	// create a KFbxNode
	//		KFbxNode* lNode = KFbxNode::Create(pSdkManager,pName);
	//KFbxNode* lNode = KFbxNode::Create(pScene, meshname);
	KFbxNode* lNode = KFbxNode::Create(pScene, nodename);
	// set the node attribute
	lNode->SetNodeAttribute(lMesh);
	// set the shading mode to view texture
	//lNode->SetShadingMode(KFbxNode::eTEXTURE_SHADING);
	// rotate the plane
	lNode->LclRotation.Set(KFbxVector4(0, 0, 0));


	// Set material mapping.
	KFbxGeometryElementMaterial* lMaterialElement = lMesh->CreateElementMaterial();
	lMaterialElement->SetMappingMode(KFbxGeometryElement::eBY_POLYGON);
	lMaterialElement->SetReferenceMode(KFbxGeometryElement::eINDEX_TO_DIRECT);
	if (!lMesh->GetElementMaterial(0)){
		_ASSERT(0);
		return NULL;
	}

	// add material to the node. 
	// the material can't in different document with the geometry node or in sub-document
	// we create a simple material here which belong to main document
	static int s_matcnt = 0;
	s_matcnt++;


	char matname[256];
	sprintf_s(matname, 256, "material_%d", s_matcnt);
	//KString lMaterialName = mqomat->name;
	KString lMaterialName = matname;
	KString lShadingName = "Phong";
	KFbxSurfacePhong* lMaterial = KFbxSurfacePhong::Create(pScene, lMaterialName.Buffer());

	//	lMaterial->Diffuse.Set(fbxDouble3(mqomat->dif4f.r, mqomat->dif4f.g, mqomat->dif4f.b));
	lMaterial->Diffuse.Set(fbxDouble3(1.0f, 1.0f, 1.0f));
	lMaterial->Emissive.Set(fbxDouble3(0.0f, 0.0f, 0.0f));
	lMaterial->Ambient.Set(fbxDouble3(0.0f, 0.0f, 0.0f));
	lMaterial->AmbientFactor.Set(1.0);
	lNode->SetShadingMode(KFbxNode::eHARD_SHADING);
	
	lMaterial->DiffuseFactor.Set(1.0);
	lMaterial->TransparencyFactor.Set(1.0f);
	lMaterial->ShadingModel.Set(lShadingName);
	lMaterial->Shininess.Set(0.5);
	lMaterial->Specular.Set(fbxDouble3(0.0f, 0.0f, 0.0f));
	lMaterial->SpecularFactor.Set(0.3);

	lNode->AddMaterial(lMaterial);
	// We are in eBY_POLYGON, so there's only need for index (a plane has 1 polygon).
	lMaterialElement->GetIndexArray().SetCount(lMesh->GetPolygonCount());
	// Set the Index to the material
	for (int i = 0; i<lMesh->GetPolygonCount(); ++i){
		lMaterialElement->GetIndexArray().SetAt(i, 0);
	}


	return lNode;


}

void LinkDummyMeshToSkeleton(CFBXBone* fbxbone, KFbxSkin* lSkin, KFbxScene* pScene, KFbxNode* pMesh, int bonecnt)
{
	KFbxXMatrix lXMatrix;
    KFbxNode* lSkel;

	//if ((fbxbone->type == FB_NORMAL) || (fbxbone->type == FB_ENDJOINT) || (fbxbone->type == FB_BUNKI_CHIL)){

		int seri = fbxbone->selem->serialno;
		CShdElem* curbone = fbxbone->selem;
		_ASSERT( curbone );
		CTreeElem2* curte = (*s_lpth)( seri );
		_ASSERT( curte );

		if( curbone->IsJoint() ){
			lSkel = fbxbone->skelnode;
			if( !lSkel ){
				_ASSERT( 0 );
				return;
			}

			KFbxCluster *lCluster = KFbxCluster::Create(pScene,"");
			lCluster->SetLink(lSkel);
			lCluster->SetLinkMode(KFbxCluster::eTOTAL1);

			int vsetno = bonecnt * 3;
			int vcnt;
			for( vcnt = 0; vcnt < 3; vcnt++ ){
				lCluster->AddControlPointIndex(vsetno, 1.0);
				vsetno++;
			}

			CShdElem* parbone = curbone->parent;
			D3DXVECTOR3 pos;
			if (fbxbone->type == FB_NORMAL){
				if (parbone->IsJoint()){
					pos.x = parbone->part->jointloc.x;
					pos.y = parbone->part->jointloc.y;
					pos.z = parbone->part->jointloc.z;
				}
				else{
					pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else if ((fbxbone->type == FB_ENDJOINT) || (fbxbone->type == FB_BUNKI_CHIL)){
				pos.x = curbone->part->jointloc.x;
				pos.y = curbone->part->jointloc.y;
				pos.z = curbone->part->jointloc.z;
			}
			else{
				_ASSERT(0);
				pos.x = curbone->part->jointloc.x;
				pos.y = curbone->part->jointloc.y;
				pos.z = curbone->part->jointloc.z;
			}

			KFbxScene* lScene = pMesh->GetScene();
			lXMatrix.SetIdentity();
			lXMatrix = pMesh->EvaluateGlobalTransform();
			lCluster->SetTransformMatrix(lXMatrix);
			//lXMatrix[3][0] = -pos.x * s_fbxmult;
			//lXMatrix[3][1] = -pos.y * s_fbxmult;
			//lXMatrix[3][2] = pos.z * s_fbxmult;
			//lCluster->SetTransformMatrix(lXMatrix);

			lXMatrix.SetIdentity();
			lXMatrix = lSkel->EvaluateGlobalTransform();
			lCluster->SetTransformLinkMatrix(lXMatrix);
			lCluster->SetTransformLinkMatrix(lXMatrix);

			lSkin->AddCluster(lCluster);
		}
	//}

}

D3DXMATRIX CalcBindMatrixZA(CFBXBone* fbxbone, int srcmotid)
{
	D3DXMATRIX axismat;
	CShdElem* curbone = fbxbone->selem;
	D3DXMatrixIdentity(&axismat);

	if (curbone){
		D3DXVECTOR3 curpos;
		CPart* curpart = curbone->part;
		if (curpart){
			curpos.x = curpart->jointloc.x * s_fbxmult;
			curpos.y = curpart->jointloc.y * s_fbxmult;
			curpos.z = -curpart->jointloc.z * s_fbxmult;
		}
		else{
			curpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		D3DXVECTOR3 parpos;
		CFBXBone* parfbxbone = fbxbone->m_parent;
		if (parfbxbone){
			CShdElem* parbone = parfbxbone->selem;
			if (parbone){
				CPart* parpart = parbone->part;
				if (parpart){
					parpos.x = parpart->jointloc.x * s_fbxmult;
					parpos.y = parpart->jointloc.y * s_fbxmult;
					parpos.z = -parpart->jointloc.z * s_fbxmult;
				}
				else{
					parpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else{
				parpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
		else{
			parpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		D3DXVECTOR3 diffvec = curpos - parpos;
		float boneleng = D3DXVec3Length(&diffvec);
		if (boneleng >= 0.00001f){
			int boneno = curbone->serialno;
			int ret;
			int framezero = 0;
			ret = s_lpmh->SetBoneAxisQ(s_lpsh, curbone->serialno, s_lpmh->GetZaType(srcmotid), srcmotid, framezero);
			_ASSERT(!ret);
			CQuaternion axisq;
			ret = curbone->GetBoneAxisQ(&axisq);
			_ASSERT(!ret);

			axismat = axisq.MakeRotMatX();
			axismat._41 = parpos.x;
			axismat._42 = parpos.y;
			axismat._43 = parpos.z;
		}
		else{
			D3DXMatrixIdentity(&axismat);
			axismat._41 = parpos.x;
			axismat._42 = parpos.y;
			axismat._43 = parpos.z;
		}
	}
	else{
		D3DXMatrixIdentity(&axismat);
	}

	return axismat;
}



D3DXMATRIX CalcBindMatrix(CFBXBone* fbxbone)
{
	D3DXVECTOR3 curpos, parpos;

	CShdElem* curbone = fbxbone->selem;
	CPart* curpart = curbone->part;
	curpos.x = curpart->jointloc.x * s_fbxmult;
	curpos.y = curpart->jointloc.y * s_fbxmult;
	curpos.z = -curpart->jointloc.z * s_fbxmult;

	CFBXBone* parfbxbone = fbxbone->m_parent;
	if (parfbxbone){
		CShdElem* parbone = parfbxbone->selem;
		if (parbone){
			CPart* parpart = parbone->part;
			parpos.x = parpart->jointloc.x * s_fbxmult;
			parpos.y = parpart->jointloc.y * s_fbxmult;
			parpos.z = -parpart->jointloc.z * s_fbxmult;
		}
		else{
			parpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	else{
		parpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXMATRIX axismat;
	D3DXVECTOR3 diffvec = curpos - parpos;
	float leng = D3DXVec3Length(&diffvec);
	if (leng >= 0.00001f){
		axismat = CalcAxisMatX_aft(parpos, curpos);
		axismat._41 = parpos.x;
		axismat._42 = parpos.y;
		axismat._43 = parpos.z;
	}
	else{
		D3DXMatrixIdentity(&axismat);
		axismat._41 = parpos.x;
		axismat._42 = parpos.y;
		axismat._43 = parpos.z;
	}
	return axismat;

}

D3DXMATRIX CalcAxisMatX_aft(D3DXVECTOR3 curpos, D3DXVECTOR3 chilpos)
{
	D3DXMATRIX retmat;
	D3DXMatrixIdentity(&retmat);
	if (curpos == chilpos){
		return retmat;
	}

	D3DXVECTOR3 startpos, endpos, upvec;

	D3DXVECTOR3 vecx0, vecy0, vecz0;
	D3DXVECTOR3 vecx1, vecy1, vecz1;

	startpos = curpos;
	endpos = chilpos;

	vecx0.x = 1.0;
	vecx0.y = 0.0;
	vecx0.z = 0.0;

	vecy0.x = 0.0;
	vecy0.y = 1.0;
	vecy0.z = 0.0;

	vecz0.x = 0.0;
	vecz0.y = 0.0;
	vecz0.z = 1.0;

	D3DXVECTOR3 bonevec;
	bonevec = endpos - startpos;
	D3DXVec3Normalize(&bonevec, &bonevec);

	if ((bonevec.x != 0.0f) || (bonevec.y != 0.0f)){
		upvec.x = 0.0f;
		upvec.y = 0.0f;
		upvec.z = 1.0f;
		//m_upkind = UPVEC_Z;//vecy1-->Y, vecz1-->Z
	}
	else{
		upvec.x = 0.0f;
		upvec.y = 1.0f;
		upvec.z = 0.0f;
		//m_upkind = UPVEC_Y;//vecy1-->Z, vecz1-->Y
	}

	vecx1 = bonevec;

	D3DXVec3Cross(&vecy1, &upvec, &vecx1);
	D3DXVec3Normalize(&vecy1, &vecy1);

	D3DXVec3Cross(&vecz1, &vecx1, &vecy1);
	D3DXVec3Normalize(&vecy1, &vecy1);


	retmat._11 = vecx1.x;
	retmat._12 = vecx1.y;
	retmat._13 = vecx1.z;

	retmat._21 = vecy1.x;
	retmat._22 = vecy1.y;
	retmat._23 = vecy1.z;

	retmat._31 = vecz1.x;
	retmat._32 = vecz1.y;
	retmat._33 = vecz1.z;

	return retmat;
}
