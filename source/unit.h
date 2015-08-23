//���j�b�g�N���X 0626���c�V��

class UNIT
{
public:
	//�R���X�g���N�^
	UNIT();
	//�f�X�g���N�^
	virtual ~UNIT() = 0;//�������z�f�X�g���N�^ ���̉����֎~����
	UNIT* operator=(const UNIT& obj);
	//�ʒu��ݒ�
	void SetPos(D3DXVECTOR3* pos) { m_pos = *pos; }
	//�ʒu���擾
	D3DXVECTOR3* GetPos() { return &m_pos; }
	//used�擾
	bool GetUsed() { return m_used; }
	//used�ݒ�
	void SetUsed(bool b) { m_used = b; }
protected://protected�����o�֐�
	/**
	 *	@brief	�X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/06/26
	 */
	virtual void Update();
	/**
	 *	@brief	�`��֐�
	 *	@author	���c�V��
	 *	@date	2015/06/26
	 */
	virtual void Draw();
protected://protected�����o�ϐ�
	D3DXVECTOR3 m_pos;//�ʒu
	D3DXMATRIX	m_matWorld;//���[���h�ϊ��s��
	MODEL*		m_pModel;//���f��
	bool		m_used;//�g�p����Ă��邩
};
