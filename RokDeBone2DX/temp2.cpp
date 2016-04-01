int CMotParamDlg::SetQOpTarget( int mvboneno, CQuaternion multq, CQuaternion totalq, int skipupdate )
{
	int ret;

	if( !m_thandler || !m_shandler || !m_mhandler )
		return 0;
	if( m_selectedno == 0 )
		return 0;
	if( m_preview_flag == 1 )
		return 0;

	if( !m_optardlg ){
		return 0;
	}
	if( m_optardlg->m_ope == OPTAR_CUR ){
		return 0;
	}

	m_optardlg->CalcStartEnd( m_current, m_sl_mp_rangemax );

	CShdElem* curselem;
	curselem = (*m_shandler)( mvboneno );
	_ASSERT( curselem );
	CShdElem* parselem;
	parselem = m_shandler->FindUpperJoint( curselem );

	CShdElem* savepar = parselem;

	CMotionCtrl* mcptr;
	if( m_mhandler->m_standard == 1 ){
		if( savepar ){
			mcptr = (*m_mhandler)( savepar->serialno );
		}else{
			mcptr = 0;
		}
	}else{
		mcptr = (*m_mhandler)( mvboneno );
	}

	if( !mcptr ){
		return 0;
	}

	int frameno;
	CMotionPoint2* calcmp;
	calcmp = mcptr->GetFirstMotionPoint( m_mot_cookie );

	while( calcmp ){
		frameno = calcmp->m_frameno;
		if( (frameno >= m_optardlg->m_startframe) && (frameno <= m_optardlg->m_endframe) ){

			if( frameno != m_current ){
			// worldmat
				CQuaternion worldq;
				D3DXMATRIX worldm;
				worldm = m_d3dapp->m_matWorld;
				worldm._41 = 0.0f;
				worldm._42 = 0.0f;
				worldm._43 = 0.0f;
				D3DXQUATERNION wqx;
				D3DXQuaternionRotationMatrix( &wqx, &worldm );
				worldq.x = wqx.x;
				worldq.y = wqx.y;
				worldq.z = wqx.z;
				worldq.w = wqx.w;

				CQuaternion parq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion curq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion invcurq( 1.0f, 0.0f, 0.0f, 0.0f );
				CQuaternion multq2;

				//m_q = invparq * multq;
				//m_q = multq * curmp->m_q;//Œã‚ÉŠ|‚¯‚é
				//m_q = curmp->m_q * multq;//Å‰‚ÉŠ|‚¯‚é		
				if( m_mhandler->m_standard == 1 ){
					if( parselem ){
						parselem = m_shandler->FindUpperJoint( parselem );
					}
				}

				CQuaternion setq;
				D3DXVECTOR3 neweul;
				if( m_soutai == OP_ZETTAI ){
					setq = totalq;
					CShdElem* tmpse = (*m_shandler)( mcptr->serialno );
					if( curmp->prev ){
						ret = qToEuler( tmpse, &curmp->m_q, &neweul );
						_ASSERT( !ret );
						ret = modifyEuler( &neweul, &curmp->prev->m_eul );
						_ASSERT( !ret );
					}else{
						ret = qToEuler( tmpse, &curmp->m_q, &neweul );
						_ASSERT( !ret );
						D3DXVECTOR3 befeul( 0.0f, 0.0f, 0.0f );
						ret = modifyEuler( &neweul, &befeul );
						_ASSERT( !ret );
					}
				}else{
					while( parselem ){
						CMotionCtrl* parmc;
						parmc = (*m_mhandler)( parselem->serialno );
			
						//parmp = parmc->IsExistMotionPoint( m_mot_cookie, m_current );
						//_ASSERT( parmp );
						CMotionPoint2 parmp;
						int hasmpflag = 0;
						ret = parmc->CalcMotionPointOnFrame( parselem, &parmp, m_mot_cookie, frameno, &hasmpflag, m_shandler, m_mhandler );
						if( ret ){
							DbgOut( "motparamdlg : SetMotionPointIK : mc CalcMotionPointOnFrame error !!!\n" );
							_ASSERT( 0 );
							return 1;
						}

						parq = parmp.m_q * parq;
						//parq = parq * parmp->m_q;//!!!!!!
						parq.normalize();
						parselem = m_shandler->FindUpperJoint( parselem );
					}

					curq = worldq * parq * curmp->m_q;

					curq.normalize();
					curq.inv( &invcurq );
					invcurq.normalize();

					D3DXQUATERNION xmultq;
					multq.CQuaternion2X( &xmultq );

					D3DXVECTOR3 axis, multaxis;
					float rad;
					D3DXQuaternionToAxisAngle( &xmultq, &axis, &rad );
					DXVec3Normalize( &axis, &axis );
					invcurq.Rotate( &multaxis, axis );
					DXVec3Normalize( &multaxis, &multaxis );

					multq2.SetAxisAndRot( multaxis, rad );

					CQuaternion aftq;
					aftq = curmp->m_q * multq2;

					int startout = 0;
					ret = g_bonepropdlg->GetLimitedQ( mcptr->serialno, &curmp->m_q, &curmp->m_eul, &aftq, &setq, &startout, &neweul, 0, 0 );
					if( ret ){
						DbgOut( "motparamdlg : SetMotionPointIK : GetLimitedQ error !!!\n" );
						_ASSERT( 0 );
						return 1;
					}
				}

				m_mvx = curmp->m_mvx;
				m_mvy = curmp->m_mvy;
				m_mvz = curmp->m_mvz;
				m_interp = curmp->interp;

				m_scalex = curmp->m_scalex;
				m_scaley = curmp->m_scaley;
				m_scalez = curmp->m_scalez;
				m_userint1 = curmp->m_userint1;
				ret = curmp->SetParams( frameno, setq, m_mvx, m_mvy, m_mvz, m_dispswitch, m_interp, m_scalex, m_scaley, m_scalez, m_userint1 );
				_ASSERT( !ret );
				curmp->m_eul = neweul;//!!!!!!!!!!!!!!!!!	
			}
		}
		calcmp = calcmp->next;
	}

	if( mcptr && (skipupdate == 0 ) ){
		UpdateMatrixSelected( mcptr->serialno, 1 );
	}

	return 0;
}
