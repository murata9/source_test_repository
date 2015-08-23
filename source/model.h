//モデルクラス 0612村田昂平
//構造体の前方宣言
typedef struct tagTEX* PTEX;//TEXのポインタ

class MODEL
{
private:
	static const int MAX_MODEL = 64;
	static MODEL sModel[MAX_MODEL];//モデル
private:
	LPD3DXMESH			pmesh;//メッシュ
	D3DMATERIAL9*		pmaterials;//マテリアルの配列
	PTEX*				pPTEX;//テクスチャの配列
	DWORD				nummaterials;//マテリアルの数
	BOOL				used;//データが入っているか示すフラグ
public:
	MODEL()//コンストラクタ
	{
		used = 0;
	}
	~MODEL();//デストラクタ
	//Xファイルを読み込む関数
	static MODEL* ReturnModel(LPCTSTR foldepath, LPCTSTR filename);
	//メッシュとテクスチャの表示関数
	void RenderModel();
	//メッシュを返す
	const LPD3DXMESH GetMesh()
	{
		return pmesh;
	}
};