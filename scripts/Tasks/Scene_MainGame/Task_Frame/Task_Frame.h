#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include "../../../Engine/myGame/myGame.h"

namespace  Frame
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("分割画面用の枠");	//グループ名
	const  string  defName("NoName");	//タスク名
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// ゲームデータ
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class Data : public BData
	{
	public:
		using SP = shared_ptr<Data>;

		json LoadDataJson(bool useBinary_)						override;	//テキストデータ読み込み
		Data();			//コンストラクタ（特に処理なし）
		~Data();		//デストラクタ  （特に処理なし）
		static Data::SP UseData();	//ゲームエンジンからデータを受け取る
		static int myNum;			//ゲームエンジンにあるデータ配列の中で、自身の要素を示す番号
		//（★★使用する場合、myPG.cppの読み込み関数にこのクラスを追加するのも忘れずに！★★） 
		//-----------------------------
		//データ変数は以下に追加する
		string imageFilePath;
		ML::Size2	imageSize;


		//-----------------------------
	};

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// リソース
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		Data::SP		data;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		ML::Image::SP img;
	};

	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// オブジェクト
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render3D_L0()		override;//「3D描画」１フレーム毎に行う処理、レイヤー0
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理、3D描画後に行われる
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
	};
}