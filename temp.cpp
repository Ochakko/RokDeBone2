	int serino;
	COPYELEM* curce;
	int pastenum = 0;
	for( serino = 1; serino < shdnum; serino++ ){
		curce = m_CEBuff + serino;
		if( curce->mp.m_frameno >= 0 ){
			int cpsymflag = 0;

			//対称ペーストの場合、対称のボーンをペーストする。
			int symmno = 0;
			int symmtype = SYMMTYPE_NONE;
			ret = SOGetSymmNo( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
			if( ret ){
				DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo error !!!\n" );
				_ASSERT( 0 );
				return 1;
			}
			if( symmno <= 0 ){
				ret = SOGetSymmNo2( m_thandler, m_shandler, serino, &symmno, &symmtype );//SymmOpe.cpp
				if( ret ){
					DbgOut( "MotParamDlg : GetPasteMP : GetSymmNo2 error !!!\n" );
					_ASSERT( 0 );
					return 1;
				}
			}
			if( symmno > 0 ){
				*(pastelist + symmno) = 1;
				pastenum++;
				cpsymflag = 1;
			}

			if( cpsymflag == 0 ){
				*(pastelist + serino) = 1;	
				pastenum++;
			}
		}
	}

