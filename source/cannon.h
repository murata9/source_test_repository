//砲台クラス 0620村田昂平
class CANNON : public UNIT
{
public:
	enum CANNON_KIND
	{
		E_MAIN_CANNON,//主砲
		E_GATLING_CANNON,//ガトリング砲
		E_ANTI_AIR_CANNON,//対空砲台
	};
	static const int	CANNON_MAX = 128;
	static CANNON*		spCannon[CANNON_MAX];//静的な実体
	static MODEL* cannonModel;//砲台モデル
	static MODEL* cannonBarrelModel;//砲身モデル
	static MODEL* cannonBaseModel;//砲台ベースモデル
	static MODEL* gatlingModel;//ガトリングモデル
	static MODEL* antiAirModel;//対空砲台モデル
	//コンストラクタ
	CANNON();
	//デストラクタ
	~CANNON();
	CANNON* operator=(const CANNON& obj);
	/**
	 *	@brief	キャノンモデル読み込み関数
	 *	@author	村田昂平
	 *	@date	2015/06/26
	 */
	static void LoadModel();
	/**
	 *	@brief	全キャノン初期化関数
	 *	@author	村田昂平
	 *	@date	2015/06/29
	 */
	static void AllInit();
	/**
	 *	@brief	キャノンクラス終了関数
	 *	@author	村田昂平
	 *	@date	2015/06/27
	 */
	static void AllDelete();
	/**
	 *	@brief	キャノン作成関数
	 *	@param	kind		種類
	 *	@param	pos			座標	MY_POINT型かD3DXVECTOR3型
	 *	@return 作成したキャノンのポインタ	失敗したらNULL
	 *	@author	村田昂平
	 *	@date	2015/06/20
	 */
	static CANNON* CreateCannon(CANNON_KIND kind, D3DXVECTOR3* pos);
	static CANNON* CreateCannon(CANNON_KIND kind, MY_POINT* point);
	/**
	 *	@brief	全キャノン更新関数
	 *	@author	村田昂平
	 *	@date	2015/06/26
	 */
	static void AllUpdate();
	/**
	 *	@brief	全キャノン描画関数
	 *	@author	村田昂平
	 *	@date	2015/06/20
	 */
	static void AllDraw();
	//主砲操作中フラグ
	static void ChangeMainCannonControll(){ s_main_cannon_controll = (s_main_cannon_controll + 1) % 2; }
	static int GetMainCannonControll(){ return s_main_cannon_controll; }
	MY_POINT* GetChipPoint(){ return &m_chip_point; }
	float GetSearchLength(){ return m_search_length; }
	int GetUpgreadCost(){ return m_upgread_cost; }
	int GetSell(){ return m_sell; }
	int GetAllCost(){ return m_all_cost; }
	CANNON_KIND GetKind(){ return m_kind; }
	//ステータス表示用
	int GetLevel(){ return m_level; }
	int GetPower(){ return m_power; }
	//アップグレード
	bool Upgread();
	//売却
	void Sell();
private://非公開メンバ関数
	/**
	 *	@brief	キャノン初期化関数
	 *	@param	kind		種類
	 *	@param	pos			3D座標
	 *	@param	point		マップチップ座標
	 *	@author	村田昂平
	 *	@date	2015/06/20
	 */
	void Init(CANNON_KIND kind, D3DXVECTOR3* pos, MY_POINT* point);
	/**
	 *	@brief	キャノン更新関数
	 *	@author	村田昂平
	 *	@date	2015/06/26
	 */
	virtual void Update() = 0;
	/**
	 *	@brief	キャノン描画関数
	 *	@author	村田昂平
	 *	@date	2015/06/20
	 */
	virtual void Draw();
	/**
	 *	@brief	配置予想キャノン描画関数
	 *	@author	村田昂平
	 *	@date	2015/07/11
	 */
	static void DrawCreate();
protected://protectedメンバ変数
	static int	s_main_cannon_controll;//false:見下ろし true:主砲視点
	int			m_fire_cnt;//発射間隔カウント
	int			m_fire_cnt_max;//発射間隔 低いほど連射速度が速い
	int			m_power;//攻撃力
	int			m_level;//アップグレードレベル
	int			m_upgread_cost;//アップグレードコスト
	int			m_all_cost;//コスト合計	売値決定用
	int			m_sell;//売値
	float		m_search_length;//射程
	CANNON_KIND m_kind;//砲台の種類
	float		m_rot_x;//高さ回転
	float		m_rot_y;//横回転
	MY_POINT	m_chip_point;//マップチップ上の位置
};

//主砲クラス 0627 村田昂平
class MAIN_CANNON : public CANNON
{
public:
	static const float HitSplashLength;//主砲の範囲ダメージの当たり判定サイズ
	static MAIN_CANNON* pMainCannon;//主砲
	//コンストラクタ
	MAIN_CANNON();
	//デストラクタ
	~MAIN_CANNON();
	MAIN_CANNON* operator=(const MAIN_CANNON& obj);
	/**
	 *	@brief	リロードゲージ描画関数
	 *	@author	村田昂平
	 *	@date	2015/07/22
	 */
	void DrawReload();
	/**
	 *	@brief	着弾予想位置描画関数
	 *	@author	村田昂平
	 *	@date	2015/07/27
	 */
	void DrawTargetPos();
private:
	/**
	 *	@brief	主砲更新関数
	 *	@author	村田昂平
	 *	@date	2015/06/27
	 */
	void Update();
	/**
	 *	@brief	主砲描画関数
	 *	@author	村田昂平
	 *	@date	2015/06/27
	 */
	//void Draw();
	D3DXVECTOR3 m_target_pos;//着弾予測地点
};


//ガトリング砲クラス 0627 村田昂平
class GATLING_CANNON : public CANNON
{
public:
	//コンストラクタ
	GATLING_CANNON();
	//デストラクタ
	~GATLING_CANNON();
	GATLING_CANNON* operator=(const MAIN_CANNON& obj);
private:
	/**
	 *	@brief	ガトリング更新関数
	 *	@author	村田昂平
	 *	@date	2015/06/27
	 */
	void Update();
};

//対空砲台クラス 0816 村田昂平
class ANTI_AIR_CANNON : public CANNON
{
public:
	//コンストラクタ
	ANTI_AIR_CANNON();
	//デストラクタ
	~ANTI_AIR_CANNON();
private:
	/**
	 *	@brief	対空砲台更新関数
	 *	@author	村田昂平
	 *	@date	2015/08/16
	 */
	void Update();
};