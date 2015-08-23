///@file ���j�b�g�N���X 0626���c�V��

#include "include.h"

//�R���X�g���N�^
UNIT::UNIT()
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixIdentity(&m_matWorld);
	m_pModel = NULL;
	m_used = false;
}

//�f�X�g���N�^
UNIT::~UNIT()
{
}

/**
 *	@brief	���j�b�g�`��֐�
 *	@author	���c�V��
 *	@date	2015/06/26
 *	@note	�I�[�o�[���C�h�p
 */
void UNIT::Draw()
{
	SetTransForm(&m_matWorld, &m_pos);
	m_pModel->RenderModel();
}

/**
 *	@brief	���j�b�g�X�V�֐�
 *	@author	���c�V��
 *	@date	2015/06/26
 *	@note	�I�[�o�[���C�h�p
 */
void UNIT::Update()
{

}
