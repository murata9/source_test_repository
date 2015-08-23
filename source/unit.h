//ユニットクラス 0626村田昂平

class UNIT
{
public:
	//コンストラクタ
	UNIT();
	//デストラクタ
	virtual ~UNIT() = 0;//純粋仮想デストラクタ 実体化を禁止する
	UNIT* operator=(const UNIT& obj);
	//位置を設定
	void SetPos(D3DXVECTOR3* pos) { m_pos = *pos; }
	//位置を取得
	D3DXVECTOR3* GetPos() { return &m_pos; }
	//used取得
	bool GetUsed() { return m_used; }
	//used設定
	void SetUsed(bool b) { m_used = b; }
protected://protectedメンバ関数
	/**
	 *	@brief	更新関数
	 *	@author	村田昂平
	 *	@date	2015/06/26
	 */
	virtual void Update();
	/**
	 *	@brief	描画関数
	 *	@author	村田昂平
	 *	@date	2015/06/26
	 */
	virtual void Draw();
protected://protectedメンバ変数
	D3DXVECTOR3 m_pos;//位置
	D3DXMATRIX	m_matWorld;//ワールド変換行列
	MODEL*		m_pModel;//モデル
	bool		m_used;//使用されているか
};
