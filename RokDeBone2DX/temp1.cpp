int CD3DDisp::RenderTriToon1( int withalpha, LPDIRECT3DDEVICE9 pd3dDevice, CShdHandler* lpsh, float* toonparams, float* fogparams, CToon1Params* ptoon1, int lightflag,
	DWORD switchno, DWORD swstate, int srcenabletex, CMotionCtrl* srcfirstjoint, int srcframeno, int srcmotid, CHKALPHA* chkalpha )
{
	HRESULT hres;
	int ret = 0;
	static int dbgcnt = 0;


	if( (withalpha == 0) && (chkalpha->notalphanum == 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	if( (withalpha != 0) && (chkalpha->alphanum == 0) ){
		return 0;//!!!!!!!!!!!!!!!!!!!!!!!!!
	}


//DbgOut( "d3ddisp : RenderTriPhong\n" );
	hres = g_pEffect->SetValue( "mFogParams", fogparams, sizeof( float ) * 2 );
	if( hres != D3D_OK ){
		DbgOut( "d3ddisp : RenderTriToon1 : fogparams set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}

	hres = g_pEffect->SetMatrix( "mView", &m_matView );
	if( hres != D3D_OK ){
		DbgOut( "d3ddisp : RenderTriToon1 : view set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}
	hres = g_pEffect->SetMatrix( "mProj", &m_matProj );
	if( hres != D3D_OK ){
		DbgOut( "d3ddisp : RenderTriToon1 : proj set error !!!\n" );
		_ASSERT( 0 );
		return 1;
	}


//float3 mLightParams[5][7];
//0: x=validflag, y=type, z=divRange
//1: x=cosPhi, y=divCosPhi
//2: xyz=diffuse
//3: xyz=specular
//4: xyz=ambient
//5: xyz=position
//6: xyz=direction

	float tmplparam[5][7][4];
	ZeroMemory( tmplparam, sizeof( float ) * 5 * 7 * 4 );

	CLightData* curl;
	curl = g_lighthead;
	int setlno = 0;
	int lno;
	for( lno = 0; lno < 5; lno++ ){
		if( curl ){
			tmplparam[lno][0][0] = 1.0f;

			tmplparam[lno][2][0] = curl->Diffuse.x;
			tmplparam[lno][2][1] = curl->Diffuse.y;
			tmplparam[lno][2][2] = curl->Diffuse.z;

			tmplparam[lno][3][0] = curl->Specular.x;
			tmplparam[lno][3][1] = curl->Specular.y;
			tmplparam[lno][3][2] = curl->Specular.z;

			tmplparam[lno][4][0] = curl->Ambient.x;
			tmplparam[lno][4][1] = curl->Ambient.y;
			tmplparam[lno][4][2] = curl->Ambient.z;

			switch( curl->Type ){
			case D3DLIGHT_DIRECTIONAL:
				tmplparam[lno][0][1] = 0.0f;

				tmplparam[lno][6][0] = curl->orgDirection.x;
				tmplparam[lno][6][1] = curl->orgDirection.y;
				tmplparam[lno][6][2] = curl->orgDirection.z;

				break;
			case D3DLIGHT_POINT:
				tmplparam[lno][0][1] = 1.0f;

				tmplparam[lno][0][2] = curl->divRange;


				tmplparam[lno][5][0] = curl->orgPosition.x;
				tmplparam[lno][5][1] = curl->orgPosition.y;
				tmplparam[lno][5][2] = curl->orgPosition.z;

				break;
			case D3DLIGHT_SPOT:
				tmplparam[lno][0][1] = 2.0f;

				tmplparam[lno][0][2] = curl->divRange;
				tmplparam[lno][1][0] = curl->cosPhi;
				tmplparam[lno][1][1] = curl->divcosPhi;

				tmplparam[lno][5][0] = curl->orgPosition.x;
				tmplparam[lno][5][1] = curl->orgPosition.y;
				tmplparam[lno][5][2] = curl->orgPosition.z;

				tmplparam[lno][6][0] = curl->orgDirection.x;
				tmplparam[lno][6][1] = curl->orgDirection.y;
				tmplparam[lno][6][2] = curl->orgDirection.z;

				break;
			}
				
			curl = curl->nextlight;
			setlno++;

		}else{
			tmplparam[lno][0][0] = 0.0f;
			tmplparam[lno][0][1] = -1.0f;
		}
	}

	if( (g_chkVS >= 20) || (g_chkVS == 0) ){
		hres = g_pEffect->SetValue( "mLightParams", &(tmplparam[0][0][0]), sizeof( float ) * 5 * 7 * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : mLightParam SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		hres = g_pEffect->SetValue( "mLightParams", &(tmplparam[0][0][0]), sizeof( float ) * 1 * 7 * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : mLightParam SetValue error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	float fsetlno;
	if( (g_renderstate[D3DRS_LIGHTING] == FALSE) || (lightflag == 0) ){
		fsetlno = -1.0f;//<--- lighting –³Œø‚ÌƒTƒCƒ“
		hres = g_pEffect->SetFloat( "mLightNum", fsetlno );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : mLightNum SetFloat error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}else{
		fsetlno = (float)setlno;
		hres = g_pEffect->SetFloat( "mLightNum", fsetlno );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : mLightNum SetFloat error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

	hres = g_pEffect->SetValue( "mEyePos", (void*)(&m_EyePt), sizeof( float ) * 3 );
	if( hres != D3D_OK ){
		DbgOut( "d3ddisp : RenderTriToon1 : LightSpecular set error !!!\n" );
		_ASSERT( 0 );
		return 1;	
	}

	if( (g_usePS == 1) && (toonparams) ){
		hres = g_pEffect->SetValue( "mToonParams", toonparams, sizeof( float ) * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : toonparams set error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}
	}

///////////////
	UINT cPasses, iPass;
	UINT cPasses2, iPass2;

	int renderno;
	int befblno = -1;
	for( renderno = 0; renderno < mrenderblocknum; renderno++ ){
		RENDERBLOCK* currb;
		currb = mrenderblock + renderno;

		int blno;
		blno = currb->skinno;

		D3DXMATRIX* curskinmat = *(mskinmat + blno);
		int curskinnum = *(mskinmatnum + blno);

		char* curtexname = 0;
		int newrapstate;

//DbgOut( "d3ddisp render : RENDERBLOCK : no %d, materialno %d, skinno %d, startface %d, endface %d\r\n",
//	renderno, currb->materialno, currb->skinno, currb->startface, currb->endface );

		LPDIRECT3DTEXTURE9 curtex0;
		curtex0 = (ptoon1 + currb->materialno)->tex;
		pd3dDevice->SetTexture( 0, curtex0 );//!!!!!!!!!


		CMQOMaterial* curmqomat;
		curmqomat = currb->mqomat;

		if( (withalpha == 0) && ( (curmqomat->col.a != 1.0f) || (curmqomat->transparent != 0) ) ){
			continue;//!!!!!!!!!!!!!!!!!!!!
		}
		if( (withalpha != 0) && (curmqomat->col.a == 1.0f) && (curmqomat->transparent == 0) ){
			continue;//!!!!!!!!!!!!!!!!!!!!
		}

		ret = curmqomat->SetCurTexname( switchno, swstate, srcenabletex, srcfirstjoint, srcframeno, srcmotid );
		_ASSERT( !ret );

		curtexname = curmqomat->curtexname;
		if( curmqomat->curtexname == 0 )
			curtexname = NULL;

		
		switch( curmqomat->texrule ){
		case TEXRULE_X:
		case TEXRULE_Y:
		case TEXRULE_Z:
			newrapstate = 0;
			break;
		case TEXRULE_CYLINDER:
			newrapstate = D3DWRAP_U;
			break;
		case TEXRULE_SPHERE:
			//newrapstate = D3DWRAP_U | D3DWRAP_V;
			newrapstate = D3DWRAP_U;
			break;
		case TEXRULE_MQ:
		default:
			newrapstate = 0;
			break;
		}
		SetRenderStateIfNotSame( pd3dDevice, D3DRS_WRAP0, newrapstate );


		LPDIRECT3DTEXTURE9 curtex1 = NULL;		
		curtex1 = g_texbnk->GetTexData( curtexname, curmqomat->transparent );
		if( curtex1 ){
			pd3dDevice->SetTexture( 1, curtex1 );
		}else{
			pd3dDevice->SetTexture( 1, NULL );
		}


		if( curmqomat->blendmode == 0 ){
			SetRenderStateIfNotSame( pd3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_ALPHATESTENABLE, FALSE );						
		}else if( curmqomat->blendmode == 1 ){
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_SRCBLEND, D3DBLEND_ONE );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_DESTBLEND, D3DBLEND_ONE );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_ALPHATESTENABLE, TRUE );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_ALPHAREF, 0x08 );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		}else if( curmqomat->blendmode == 2 ){
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_DESTBLEND, D3DBLEND_ONE );
			SetRenderStateIfNotSame( pd3dDevice,  D3DRS_ALPHATESTENABLE, FALSE );
		}

		hres = g_pEffect->SetFloat( "mAlpha", curmqomat->col.a );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : mAlpha SetFloat error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}


		if( curskinnum == 1 ){
			hres = g_pEffect->SetTechnique( "RenderSceneBone0Toon1" );
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else if( curskinnum == 2 ){
			hres = g_pEffect->SetTechnique( "RenderSceneBone1Toon1" );
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}else{
			hres = g_pEffect->SetTechnique( "RenderSceneBone4Toon1" );
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}

		if( blno != befblno ){
			hres = g_pEffect->SetMatrixArray( "mWorldMatrixArray", curskinmat, curskinnum );
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : worldmatrixarray set error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			befblno = blno;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		}

		float tmpfl[4];
		tmpfl[0] = 0.0f;
		tmpfl[1] = 0.0f;
		tmpfl[2] = (ptoon1 + currb->materialno)->darknl;
		tmpfl[3] = (ptoon1 + currb->materialno)->brightnl;
		hres = g_pEffect->SetValue( "mToonParams", tmpfl, sizeof( float ) * 4 );
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : toon1 params set error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

			/////////
		hres = g_pEffect->Begin(&cPasses, 0);
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : effect Begin error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}

		for (iPass = 0; iPass < cPasses; iPass++){
			hres = g_pEffect->BeginPass(iPass);
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : effect BeginPass error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			// The effect interface queues up the changes and performs them 
			// with the CommitChanges call. You do not need to call CommitChanges if 
			// you are not setting any parameters between the BeginPass and EndPass.
			// V( g_pEffect->CommitChanges() );

			int curnumprim;
			curnumprim = currb->endface - currb->startface;

			if( curnumprim > 0 ){
				pd3dDevice->SetVertexDeclaration( m_pskindecl );
				pd3dDevice->SetStreamSource( 0, *(m_ArrayVB + blno), 0, sizeof(SKINVERTEX) );
				pd3dDevice->SetIndices( m_IB );

				hres = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
													0,
													0,
													m_numTLV,
													currb->startface * 3,
													curnumprim
													);
				_ASSERT( hres == D3D_OK );
			}

			if( m_revArrayVB && (curnumprim > 0) ){
				pd3dDevice->SetVertexDeclaration( m_pskindecl );
				pd3dDevice->SetStreamSource( 0, *(m_revArrayVB + blno), 0, sizeof(SKINVERTEX) );		
				pd3dDevice->SetIndices( m_revIB );
				hres = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
												0,
												0,
												m_numTLV,
												currb->startface * 3,
												curnumprim
												);
				_ASSERT( hres == D3D_OK );
			}

			hres = g_pEffect->EndPass();
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : effect EndPass error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

		}
		hres = g_pEffect->End();
		if( hres != D3D_OK ){
			DbgOut( "d3ddisp : RenderTriToon1 : effect End error !!!\n" );
			_ASSERT( 0 );
			return 1;
		}				
	}	




//////////// edge
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1 , D3DTSS_COLOROP , D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1 , D3DTSS_ALPHAOP , D3DTOP_DISABLE );
	pd3dDevice->SetTexture( 0, NULL );
	pd3dDevice->SetTexture( 1, NULL );
		

	int renderno2;
	int befblno2 = -1;
	for( renderno2 = 0; renderno2 < mrenderblocknum; renderno2++ ){
		RENDERBLOCK* currb2;
		currb2 = mrenderblock + renderno2;

		int blno2;
		blno2 = currb2->skinno;

		D3DXMATRIX* curskinmat = *(mskinmat + blno2);
		int curskinnum = *(mskinmatnum + blno2);

//DbgOut( "d3ddisp render : RENDERBLOCK : no %d, materialno %d, skinno %d, startface %d, endface %d\r\n",
//	renderno, currb->materialno, currb->skinno, currb->startface, currb->endface );


		////////////

		if( (ptoon1 + currb2->materialno)->edgevalid0 != 0 ){

			if( curskinnum == 1 ){
				hres = g_pEffect->SetTechnique( "RenderSceneBone0Edge0" );
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else if( curskinnum == 2 ){
				hres = g_pEffect->SetTechnique( "RenderSceneBone1Edge0" );
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}else{
				hres = g_pEffect->SetTechnique( "RenderSceneBone4Edge0" );
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : SetTechnique error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}

			if( blno2 != befblno2 ){
				hres = g_pEffect->SetMatrixArray( "mWorldMatrixArray", curskinmat, curskinnum );
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : edge worldmatrixarray set error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
				befblno2 = blno2;
			}


			float edge0[4];
			edge0[0] = (ptoon1 + currb2->materialno)->edgecol0.r;
			edge0[1] = (ptoon1 + currb2->materialno)->edgecol0.g;
			edge0[2] = (ptoon1 + currb2->materialno)->edgecol0.b;
			edge0[3] = (ptoon1 + currb2->materialno)->edgecol0.a;//<--- “à—e‚Íwidth

			hres = g_pEffect->SetValue( "mEdgeCol0", &(edge0[0]), sizeof( float ) * 4 );
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : mEdgeCol0 SetValue error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}


			/////////
			hres = g_pEffect->Begin(&cPasses2, 0);
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : effect Begin error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}

			for (iPass2 = 0; iPass2 < cPasses2; iPass2++){
				hres = g_pEffect->BeginPass(iPass2);
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : effect BeginPass error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}


				// The effect interface queues up the changes and performs them 
				// with the CommitChanges call. You do not need to call CommitChanges if 
				// you are not setting any parameters between the BeginPass and EndPass.
				// V( g_pEffect->CommitChanges() );

				int curnumprim;
				curnumprim = currb2->endface - currb2->startface;

				if( curnumprim > 0 ){

					if( (ptoon1 + currb2->materialno)->edgeinv0 && m_revArrayVB ){
						//— –Ê
						pd3dDevice->SetVertexDeclaration( m_pskindecl );
						pd3dDevice->SetStreamSource( 0, *(m_revArrayVB + blno2), 0, sizeof(SKINVERTEX) );		
						//pd3dDevice->SetIndices( m_revIB );
						pd3dDevice->SetIndices( m_IB );
						hres = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
													0,
													0,
													m_numTLV,
													currb2->startface * 3,
													curnumprim
													);
					}else{

						pd3dDevice->SetVertexDeclaration( m_pskindecl );
						pd3dDevice->SetStreamSource( 0, *(m_ArrayVB + blno2), 0, sizeof(SKINVERTEX) );
						//pd3dDevice->SetIndices( m_IB );
						pd3dDevice->SetIndices( m_revIB );

						hres = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
														0,
														0,
														m_numTLV,
														currb2->startface * 3,
														curnumprim
														);
						}
				}

				/***
				if( m_revArrayVB && (curnumprim > 0) ){
					pd3dDevice->SetVertexDeclaration( m_pskindecl );
					pd3dDevice->SetStreamSource( 0, *(m_revArrayVB + blno2), 0, sizeof(SKINVERTEX) );		
					pd3dDevice->SetIndices( m_revIB );
					hres = pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
												0,
												0,
												m_numTLV,
												currb2->startface * 3,
												curnumprim
												);

				}
				***/
				hres = g_pEffect->EndPass();
				if( hres != D3D_OK ){
					DbgOut( "d3ddisp : RenderTriToon1 : effect EndPass error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}

			}
			hres = g_pEffect->End();
			if( hres != D3D_OK ){
				DbgOut( "d3ddisp : RenderTriToon1 : effect End error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
		}
	}


	return ret;
}
