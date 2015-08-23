///@file 砲台クラス 0620 村田昂平

#include "include.h"
MAIN_CANNON* MAIN_CANNON::pMainCannon;//主砲
const float MAIN_CANNON::HitSplashLength = 3.0f;//主砲の範囲ダメージの当たり判定サイズ
CANNON* CANNON::spCannon[CANNON_MAX];//静的な実体
int CANNON::s_main_cannon_controll;//主砲操作中フラグ
MODEL* CANNON::cannonModel;//砲台モデル
MODEL* CANNON::cannonBarrelModel;//砲身モデル
MODEL* CANNON::cannonBaseModel;//砲台ベースモデル
MODEL* CANNON::gatlingModel;//ガトリングモデル
MODEL* CANNON::antiAirModel;//対空砲台モデル

/**
 *	@brief	キャノンモデル読み込み関数
 *	@author	村田昂平
 *	@date	2015/06/26
 */
void CANNON::LoadModel()
{
	//初期化
	//読み込み
	cannonModel = MODEL::ReturnModel("DATA/cannon/", "cannon2.x");
	cannonBarrelModel = MODEL::ReturnModel("DATA/cannon/", "cannon_barrel.x");
	cannonBaseModel = MODEL::ReturnModel("DATA/cannon/", "cannon_base.x");
	gatlingModel = MODEL::ReturnModel("DATA/cannon/", "Gatling.x");
	antiAirModel = MODEL::ReturnModel("DATA/cannon/", "cannon_base.x");
}


/**
 *	@brief	キャノンクラス終了関数
 *	@author	村田昂平
 *	@date	2015/06/27
 */
void CANNON::AllDelete()
{
	for (int i = 0; i < CANNON_MAX; i++)
	{
		SAFE_DELETE(spCannon[i]);
	}
}


/**
 *	@brief	全キャノン初期化関数
 *	@author	村田昂平
 *	@date	2015/06/29
 */
void CANNON::AllInit()
{
	AllDelete();
	MAIN_CANNON::pMainCannon = NULL;
}

//コンストラクタ
CANNON::CANNON()
{
	m_fire_cnt = 0;
	m_power = 0;
	m_search_length = 0;
	m_rot_x = 0;
	m_rot_y = 0;
	m_kind = E_MAIN_CANNON;
}

//コンストラクタ
MAIN_CANNON::MAIN_CANNON()
{
	m_target_pos = D3DXVECTOR3(0, 0, 0);
}

//コンストラクタ
GATLING_CANNON::GATLING_CANNON()
{

}

//コンストラクタ
ANTI_AIR_CANNON::ANTI_AIR_CANNON()
{

}

//デストラクタ
CANNON::~CANNON()
{

}

//デストラクタ
MAIN_CANNON::~MAIN_CANNON()
{

}

//デストラクタ
GATLING_CANNON::~GATLING_CANNON()
{

}

//デストラクタ
ANTI_AIR_CANNON::~ANTI_AIR_CANNON()
{

}
/**
 *	@brief	キャノン初期化関数
 *	@param	kind		種類
 *	@param	pos			座標
 *	@param	point		マップチップ座標
 *	@author	村田昂平
 *	@date	2015/06/20
 */
void CANNON::Init(CANNON_KIND kind, D3DXVECTOR3* pos, MY_POINT* point)
{
	MODEL* pModel;
	m_level = 1;
	m_fire_cnt = 0;
	switch (kind)
	{
	case CANNON::E_MAIN_CANNON:
		pModel = cannonBaseModel;
		m_power = 100;
		m_search_length = 1000.0f;//射程なし
		m_sell = -1;//売却不可
		m_upgread_cost = 200;
		m_all_cost = 0;
		m_fire_cnt_max = 120;
		break;
	case CANNON::E_GATLING_CANNON:
		pModel = gatlingModel;
		m_power = 10;
		m_search_length = 5.0f;
		m_sell = 20;
		m_upgread_cost = 75;
		m_all_cost = 100;
		m_fire_cnt_max = 30;
		break;
	case CANNON::E_ANTI_AIR_CANNON:
		pModel = antiAirModel;
		//未設定
		m_power = 10;
		m_search_length = 5.0f;
		m_sell = 20;
		m_upgread_cost = 75;
		m_all_cost = 100;
		m_fire_cnt_max = 30;
		break;
	}
	m_pModel = pModel;
	SetPos(pos);
	m_chip_point = *point;
	m_kind = kind;
	m_used = true;
}
/**
 *	@brief	キャノン作成関数
 *	@param	kind		種類
 *	@param	pos			座標	D3DXVECTOR3型
 *	@return 作成したキャノンのポインタ	失敗したらNULL
 *	@author	村田昂平
 *	@date	2015/07/11
 *	@note	座標を変換してオーバーロード元に渡す
 */
CANNON* CANNON::CreateCannon(CANNON_KIND kind, D3DXVECTOR3* pos)
{
	//3次元座標をマップチップ単位に変換
	int ix;
	int iy;
	map.FloatToIntPos(pos->x, pos->z, &ix, &iy);
	MY_POINT point = MY_POINT(ix, iy);
	return CreateCannon(kind, &point);
}

/**
 *	@brief	キャノン作成関数
 *	@param	kind		種類
 *	@param	pos			座標	MY_POINT型
 *	@return 作成したキャノンのポインタ	失敗したらNULL
 *	@author	村田昂平
 *	@date	2015/06/20
 */
CANNON* CANNON::CreateCannon(CANNON_KIND kind, MY_POINT* point)
{
	//空きを探す
	for (int i = 0; i < CANNON_MAX; i++)
	{
		//空いていたら初期化
		if (CANNON::spCannon[i] == NULL)
		{
			//座標をマップチップ単位に変換
			D3DXVECTOR3 create_pos = D3DXVECTOR3(0, 0, 0);
			map.IntToFloatPos(point->x, point->y, &create_pos.x, &create_pos.z);
			if (kind != E_MAIN_CANNON)
			{
				if (map.GetChip(point)->GetState() == 1)
				{
					return NULL;
				}
				if (point->y >= map.GetHeight() - 2)
				{
					return NULL;//壁付近
				}
				//コストの支払い
				if (game_state.PayMoney(100) == false)return NULL;
				//通行不能化処理
				//map.ChangeChipLoop(point->x,point->y, 0, 1, 3);
				map.GetChip(point)->SetId(0);
				map.GetChip(point)->SetState(1);
				//ゴールを塞がないかチェック
				if (goal.CheckGoal() == false)
				{
					//キャンセル
					map.GetChip(point)->SetId(-1);
					map.GetChip(point)->SetState(0);
					game_state.AddMoney(100);
					return NULL;
				}
				map.SetChangedFlag(1);
			}
			//種類に応じて作成
			switch (kind)
			{
			case CANNON::E_MAIN_CANNON:
				MAIN_CANNON::pMainCannon = new MAIN_CANNON;//主砲は専用のポインタを用意
				spCannon[i] = MAIN_CANNON::pMainCannon;
				break;
			case CANNON::E_GATLING_CANNON:
				spCannon[i] = new GATLING_CANNON;
				PlayWav(ST_SE, SSK_CREATE_CANNON, 0);
				break;
			case CANNON::E_ANTI_AIR_CANNON:
				spCannon[i] = new ANTI_AIR_CANNON;
				PlayWav(ST_SE, SSK_CREATE_CANNON, 0);
				break;
			}
			CANNON::spCannon[i]->Init(kind, &create_pos, point);
			return CANNON::spCannon[i];
		}
	}
	return NULL;
}

/**
 *	@brief	キャノン描画関数
 *	@author	村田昂平
 *	@date	2015/06/20
 */
void CANNON::Draw()
{
	D3DXVECTOR3 draw_pos = m_pos;
	draw_pos.y += 0.5f;//位置補正
	//行列の回転
	D3DXMATRIX matRotX;
	D3DXMatrixRotationX(&matRotX, m_rot_x);
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, m_rot_y);
	SetTransForm(&m_matWorld, &draw_pos);
	//スケーリング
	//D3DXMATRIX matScl;
	//float s = 2.5f;
	//D3DXMatrixScaling(&matScl, s, s, s);
	//m_matWorld = matScl * m_matWorld;
	//ベース描画
	m_matWorld = matRotY * m_matWorld;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pModel->RenderModel();
	if (m_kind == E_MAIN_CANNON)
	{
		//砲身描画
		m_matWorld = matRotY * matRotX * m_matWorld;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		cannonBarrelModel->RenderModel();
	}
}

/**
 *	@brief	配置予想キャノン描画関数
 *	@author	村田昂平
 *	@date	2015/07/11
 */
void CANNON::DrawCreate()
{
	if (mouse->GetChipPointFlag() == false)return;
	if (CANNON_MENU_UI::GetSelectCannon() != NULL)return;
	MY_POINT point = *mouse->GetChipPoint();
	D3DXVECTOR3 draw_pos = D3DXVECTOR3(0, 0, 0);
	map.IntToFloatPos(point.x, point.y, &draw_pos.x, &draw_pos.z);
	draw_pos.y += 0.5f;//位置補正
	//行列の回転
	//ベース描画
	D3DXMATRIX matWorld;
	SetTransForm(&matWorld, &draw_pos);
	gatlingModel->RenderModel();
}

/**
 *	@brief	全キャノン描画関数
 *	@author	村田昂平
 *	@date	2015/06/20
 */
void CANNON::AllDraw()
{
	for (int i = 0; i < CANNON_MAX; i++)
	{
		if (CANNON::spCannon[i] == NULL)continue;
		if (CANNON::spCannon[i]->GetUsed() == false)continue;
		CANNON::spCannon[i]->Draw();
	}
	if (GetMainCannonControll() == 0)
	{
		//配置予想表示
		DrawCreate();
	}
	MAIN_CANNON::pMainCannon->DrawReload();
}

/**
 *	@brief	砲台アップグレード関数
 *	@return	成功ならtrue お金が足りなければfalse
 *	@author	村田昂平
 *	@date	2015/07/18
 */
bool CANNON::Upgread()
{
	if (game_state.PayMoney(m_upgread_cost) == true)
	{
		m_level++;
		m_power += m_upgread_cost * 2 / 10;
		m_all_cost += m_upgread_cost;
		m_sell = m_all_cost * 2 / 10;//20%
		m_upgread_cost *= 2;
		PlayWav(ST_SE, SSK_UPGRADE_CANNON, 0);
		return true;
	}
	return false;
}

/**
 *	@brief	砲台売却関数
 *	@author	村田昂平
 *	@date	2015/07/18
 */
void CANNON::Sell()
{
	game_state.AddMoney(m_sell);
	map.SetChangedFlag(1);
	map.GetChip(&m_chip_point)->SetState(0);
	//debug表示
	map.GetChip(&m_chip_point)->SetId(-1);
	m_used = false;
	CANNON_MENU_UI::SetSelectCannon(NULL);
	PlayWav(ST_SE, SSK_SELL_CANNON, 0);
}

/**
 *	@brief	主砲更新関数
 *	@author	村田昂平
 *	@date	2015/06/27
 */
void MAIN_CANNON::Update()
{
	if (m_fire_cnt > 0)
	{
		m_fire_cnt--;
	}
	//主砲
	if (GetMainCannonControll() == 1)
	{
		m_rot_x = ((float)mouse->GetScreenPosY() / SCREEN_HEIGHT);
		m_rot_x *= D3DXToRadian(-90);//(D3DX_PI / 2) * 0.5f;//90度 * 0.5
		m_rot_x -= D3DXToRadian(10);//D3DX_PI * 3 / 8;

		m_rot_y = ((float)mouse->GetScreenPosX() / SCREEN_WIDTH) - 0.5f;//-0.5～0.5
		m_rot_y *= D3DX_PI * 0.3f;//180度 * 0.3
		//主砲発射
		if (mouse->GetChipPointFlag() == true)
		{
			m_target_pos = *mouse->GetPos();
			m_target_pos.y = m_pos.y;
			if (mouse->GetClick() == 1 && m_fire_cnt == 0)
			{
				D3DXVECTOR3 speed = m_target_pos - m_pos;
				float dF = D3DXVec3Length(&speed);
				speed /= 60.0f;
				speed.y = 30.0f * BULLET::BULLET_GRAVITY;
				BULLET::CreateBullet(BULLET::E_MAIN_CANNON_BULLET, &m_pos, &speed, m_power, m_search_length);
				m_fire_cnt = m_fire_cnt_max;
				//SE
				PlayWav(ST_SE, SSK_MAIN_CANNON, 0);
			}
		}
	}
}

/**
 *	@brief	リロードゲージ描画関数
 *	@author	村田昂平
 *	@date	2015/07/22
 */
void MAIN_CANNON::DrawReload()
{
	D3DXVECTOR3 temp;
	D3DXVECTOR3 pos = m_pos;
	//pos.y += 1.0f;
	Calc3DPosTo2DPos(&pos, &temp);
	float draw_x = temp.x;
	float draw_y = temp.y;
	float w = 15.0f / temp.z;//距離による拡縮
	float width2 = 40 * w;
	float height2 = 5 * w;
	//枠
	//DrawTexture(draw_x - width2, draw_y - height2, draw_x + width2, draw_y + height2, UI_tex[UI_HP_RED]->pTex, 0);
	//表示範囲制限
	SetDrawUI_Area((int)(draw_x - width2), (int)(width2 * 2 + 1), m_fire_cnt, m_fire_cnt_max);
	//リロード
	DrawTexture(draw_x - width2, draw_y - height2, draw_x + width2, draw_y + height2, UI_tex[UI_CANNON_RELOAD]->pTex, 0);
	//リセット
	ResetDrawArea();
}


/**
 *	@brief	着弾予想位置描画関数
 *	@author	村田昂平
 *	@date	2015/07/27
 */
void MAIN_CANNON::DrawTargetPos()
{
	if (s_main_cannon_controll != false)
	{
		if (mouse->GetChipPointFlag() != false)
		{
			D3DXVECTOR3 temp_pos = m_target_pos;
			temp_pos.y += 0.1f;
			DrawCircle(&temp_pos, 4.0f * MAIN_CANNON::HitSplashLength);
		}
	}
}

/**
 *	@brief	ガトリング砲更新関数
 *	@author	村田昂平
 *	@date	2015/06/27
 */
void GATLING_CANNON::Update()
{
	if (m_fire_cnt > 0)
	{
		m_fire_cnt--;
	}
	if (m_fire_cnt == 0)
	{
		//敵を探す
		ENEMY* pEnemy = ENEMY::SearchNearEnemy(GetPos(), m_search_length);
		if (pEnemy == NULL)return;
		//角度差計算
		D3DXVECTOR3 dVec = m_pos - *pEnemy->GetPos();
		m_rot_y = atan2(dVec.x, dVec.z) + D3DX_PI;
		D3DXVECTOR3 speed = D3DXVECTOR3(0, 0, 1);
		//行列を使って速度を回転
		D3DXMATRIX matRot;
		D3DXMatrixRotationY(&matRot, m_rot_y);
		D3DXVec3TransformCoord(&speed, &speed, &matRot);
		speed *= 0.5f;
		//敵の移動を予測射撃
		D3DXVECTOR3* enemy_speed = pEnemy->GetSpeedVec();
		speed += *enemy_speed;
		for (int i = 0; i < 2; i++)
		{
			D3DXVECTOR3 pos = m_pos;
			D3DXVECTOR3 add_pos = D3DXVECTOR3(-0.5f + (1 * i), 0.5f, 0);
			//行列を使って回転後の砲身位置を計算
			D3DXVec3TransformCoord(&add_pos, &add_pos, &matRot);
			pos += add_pos;
			pos += speed;
			BULLET::CreateBullet(BULLET::E_GATLING_BULLET, &pos, &speed, m_power, m_search_length);
		}
		m_fire_cnt = m_fire_cnt_max;
		for (int i = 0; i < GATLING_SE_MAX; i++)
		{
			if (CheckPlayWav(ST_SE, SSK_GATLING_CANNON + i) == 0)
			{
				PlayWav(ST_SE, SSK_GATLING_CANNON + i, 0);
				break;
			}
		}
	}
}

/**
 *	@brief	対空砲台砲更新関数
 *	@author	村田昂平
 *	@date	2015/08/16
 */
void ANTI_AIR_CANNON::Update()
{

}

/**
 *	@brief	全キャノン更新関数
 *	@author	村田昂平
 *	@date	2015/06/26
 */
void CANNON::AllUpdate()
{
	for (int i = 0; i < CANNON_MAX; i++)
	{
		if (CANNON::spCannon[i] == NULL)continue;
		if (CANNON::spCannon[i]->GetUsed() == false)continue;
		CANNON::spCannon[i]->Update();
	}
}
