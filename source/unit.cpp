///@file ユニットクラス 0626村田昂平

#include "include.h"

//コンストラクタ
UNIT::UNIT()
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity(&m_matWorld);
	m_pModel = NULL;
	m_used = false;
}

//デストラクタ
UNIT::~UNIT()
{
}

/**
 *	@brief	ユニット描画関数
 *	@author	村田昂平
 *	@date	2015/06/26
 *	@note	オーバーライド用
 */
void UNIT::Draw()
{
	SetTransForm(&m_matWorld, &m_pos);
	m_pModel->RenderModel();
}

/**
 *	@brief	ユニット更新関数
 *	@author	村田昂平
 *	@date	2015/06/26
 *	@note	オーバーライド用
 */
void UNIT::Update()
{

}
