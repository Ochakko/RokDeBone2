おちゃっこLABの2002から2016年のアートソース公開！！

これらのソースは１０年間継ぎ足しカレーのように
新たに作り直すことなく、こつこつ煮ながら作ったものです。

ですのできれいなソースとはお世辞にも言えない状態です。

こういうソースでちゃんと動いていたことが
ある意味アートなのです！！

ライセンスはきちんとします。
LGPLのライセンスの元に公開します。


RokDeBone2は
メタセコイアデータにアニメーションを付けるツールです。
その他のゲームデータ作成機能もあります。
FBXにアニメーションを書き出すことも出来ます。
DirectX9を使用しています。

oggvorbis、zlib、FBX SDK(2012.2)、DirectX SDK(June2010)、colladaを使用しています。
libogg は1.2.2、libvorbisは1.3.2を使用しました。
（この圧縮ファイル中には入っていません。ビルドするときは別途DLしてください。）



////////
2020/11/08　おちゃっこLAB　おちゃっこ
	FBX出力修正
	Cluster出力を必要最小限にした
	頂点に影響を与えないジョイントの処理のためにダミーメッシュ復活

	AppleのRealityConverterでusdzに変換できることを確認
	
2020/10/31その１　おちゃっこLAB　おちゃっこ
	FBX出力修正
	Maya2018で読み込めるように再修正

	今回の修正により
	Appleのコンバーターでusdzに変換できることを確認

	MameBake3Dは明日？着手予定

2020/10/29その２　おちゃっこLAB　おちゃっこ
	2020/10/29その１の更新で
	新しく作ったデータではうまくいったが
	古いデータをコンバートすると不具合が出た
	すぐには直らないので影響が少ない方に戻すことにした
	つまり
	2020/10/20のバージョンに戻すことにした
	

2020/10/29その１　おちゃっこLAB　おちゃっこ
	FBX出力のデバッグ
		2020/10/20に無駄な頂点出力をやめたがその頂点へのダミー影響ボーンが残っていた
		無くなった頂点へのダミー影響ボーン処理をコメントアウト


2020/10/20　おちゃっこLAB　おちゃっこ
	FBXの受け渡し順として
	RokDeBone2-->Maya2018-->MameBake3D-->MameBake3D
	という経路と
	RokDeBone2-->Maya2018-->MameBake3D-->Maya2018
	という経路において
	FBXが正しく受け渡せるようにデバッグした
	
	RokDeBone2とMameBake3Dと両方の修正が必要だった
	RokDeBone2では無駄な頂点バッファが出力されていて悪影響を与えることがあったのを修正
	MameBake3DではインデックスとUVの読み込み時と書き出し時の処理を見直した
	MameBake3Dでは影響度出力の際の参照と書き出しのインデックスがorgなのかoptなのかをはっきりさせた
	
	
	上記の２つの経路で正常にFBXの受け渡しが出来ることを確認
	ただし
	RokDeBone2-->Maya2018-->MameBake3D-->Maya2018
	の経路においては
	Maya上で法線の平均メニューを実行するなどしてスムージングをすることが必要だった
	

2016/06/22 その１
	exeファイルをgitからrmしました。
	コンパイル環境がない方で実行ファイルを試したい方は
	info@ochakkolab.moo.jp までメールにてお問合せください。

2016/04/13 おちゃっこLAB　おちゃっこ
	　　FBXExportの更新。
	　　無効のボーンが一番親にあった場合に書き出されていた不具合を修正。

2016/04/12 おちゃっこLAB　おちゃっこ
	　　BVH読み込みを改良しました。
	　　今まではBVHのZが反転して読み込まれていたのでモデルデータを後ろ向きにする必要がありましたが
	　　今回から前向きのままで設定、再生出来るようになりました。
	　　manual/rdb2_LoadBVH.htmに追記。

	　　23:00頃
	　　FBX出力をバージョンアップしました。
	　　一番親のボーンの位置が直りました。

2016/04/06 おちゃっこLAB　おちゃっこ
	　　FBX出力を改良しました。
	　　スムージングを考慮した法線で出力します。Mayaで読み込み確認しました。
	　　RokDeBone2でいうところのtoon1モードで書き出します。
	　　toon1用のマテリアルに自動で分割して出力します。
	　　出力後はtoon1モードになります（切り替え可能）。

	　　サンプルはRokDeBone2/RokDeBone2DX/Sampleの中にあります。
	　　bvhに適したサンプルはこれから準備します。

2016/04/02 おちゃっこLAB おちゃっこ
	　　GitHubには改行を\r\nから\nへと自動変換してpushしていました。
	　　そのためmqoの読み込みが空回りして起動時に固まっていました。
	　　今回から\r\nのままpushするので解決したと思います。

	　　今回、--forceでpushします。
	　　昔のpushにはsdfファイルが含まれてしまっていたのですが
	　　その昔のsdfのサイズで怒られるようになってしまったので--forceします。
	　　まだ自分の変更しかpushされていなかったので。

2014/08/05 おちゃっこLAB おちゃっこ
　　　　　　音を読み込んでいるとMOAのダイアログでPlayを押しても
　　　　　　テスト出来ない不具合を修正しました。
　　　　　　(MOAのダイアログとはツールボタンの「モ」を押したときに出るダイアログです。)

2014/07/24 おちゃっこLAB おちゃっこ
　　　　　　インポートした後にポーズのコピーペーストをすると
　　　　　　PasteUndoBuffer : AddMotionPointer errorとダイアログが出て
　　　　　　落ちることがあるバグを修正しました。

2014/07/23 おちゃっこLAB おちゃっこ
　　　　　　名前ではなくヒエラルキー(階層構造)を元に
　　　　　　コピーペーストする機能を追加しました。
　　　　　　ヒエラルキーコピーはCtrl + H
　　　　　　ヒエラルキーペーストはCtrl + Shift + H。

2014/07/10 おちゃっこLAB　おちゃっこ　
　　　　　　いつの間にか回転モード（ツールボタンのR）に出来なくなっていたバグを修正。
　　　　　　FBX書き出しを有効に。
　　　　　　レジストキーの入力を不要に。

2014/06/10 おちゃっこLAB　おちゃっこ(化け猫おちゃっこ)
　　　　　　RokDeBone2をオープンソースにして公開。
　　　

info@ochakkolab.moo.jp



