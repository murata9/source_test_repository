//�C��N���X 0620���c�V��
class CANNON : public UNIT
{
public:
	enum CANNON_KIND
	{
		E_MAIN_CANNON,//��C
		E_GATLING_CANNON,//�K�g�����O�C
		E_ANTI_AIR_CANNON,//�΋�C��
	};
	static const int	CANNON_MAX = 128;
	static CANNON*		spCannon[CANNON_MAX];//�ÓI�Ȏ���
	static MODEL* cannonModel;//�C�䃂�f��
	static MODEL* cannonBarrelModel;//�C�g���f��
	static MODEL* cannonBaseModel;//�C��x�[�X���f��
	static MODEL* gatlingModel;//�K�g�����O���f��
	static MODEL* antiAirModel;//�΋�C�䃂�f��
	//�R���X�g���N�^
	CANNON();
	//�f�X�g���N�^
	~CANNON();
	CANNON* operator=(const CANNON& obj);
	/**
	 *	@brief	�L���m�����f���ǂݍ��݊֐�
	 *	@author	���c�V��
	 *	@date	2015/06/26
	 */
	static void LoadModel();
	/**
	 *	@brief	�S�L���m���������֐�
	 *	@author	���c�V��
	 *	@date	2015/06/29
	 */
	static void AllInit();
	/**
	 *	@brief	�L���m���N���X�I���֐�
	 *	@author	���c�V��
	 *	@date	2015/06/27
	 */
	static void AllDelete();
	/**
	 *	@brief	�L���m���쐬�֐�
	 *	@param	kind		���
	 *	@param	pos			���W	MY_POINT�^��D3DXVECTOR3�^
	 *	@return �쐬�����L���m���̃|�C���^	���s������NULL
	 *	@author	���c�V��
	 *	@date	2015/06/20
	 */
	static CANNON* CreateCannon(CANNON_KIND kind, D3DXVECTOR3* pos);
	static CANNON* CreateCannon(CANNON_KIND kind, MY_POINT* point);
	/**
	 *	@brief	�S�L���m���X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/06/26
	 */
	static void AllUpdate();
	/**
	 *	@brief	�S�L���m���`��֐�
	 *	@author	���c�V��
	 *	@date	2015/06/20
	 */
	static void AllDraw();
	//��C���쒆�t���O
	static void ChangeMainCannonControll(){ s_main_cannon_controll = (s_main_cannon_controll + 1) % 2; }
	static int GetMainCannonControll(){ return s_main_cannon_controll; }
	MY_POINT* GetChipPoint(){ return &m_chip_point; }
	float GetSearchLength(){ return m_search_length; }
	int GetUpgreadCost(){ return m_upgread_cost; }
	int GetSell(){ return m_sell; }
	int GetAllCost(){ return m_all_cost; }
	CANNON_KIND GetKind(){ return m_kind; }
	//�X�e�[�^�X�\���p
	int GetLevel(){ return m_level; }
	int GetPower(){ return m_power; }
	//�A�b�v�O���[�h
	bool Upgread();
	//���p
	void Sell();
private://����J�����o�֐�
	/**
	 *	@brief	�L���m���������֐�
	 *	@param	kind		���
	 *	@param	pos			3D���W
	 *	@param	point		�}�b�v�`�b�v���W
	 *	@author	���c�V��
	 *	@date	2015/06/20
	 */
	void Init(CANNON_KIND kind, D3DXVECTOR3* pos, MY_POINT* point);
	/**
	 *	@brief	�L���m���X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/06/26
	 */
	virtual void Update() = 0;
	/**
	 *	@brief	�L���m���`��֐�
	 *	@author	���c�V��
	 *	@date	2015/06/20
	 */
	virtual void Draw();
	/**
	 *	@brief	�z�u�\�z�L���m���`��֐�
	 *	@author	���c�V��
	 *	@date	2015/07/11
	 */
	static void DrawCreate();
protected://protected�����o�ϐ�
	static int	s_main_cannon_controll;//false:�����낵 true:��C���_
	int			m_fire_cnt;//���ˊԊu�J�E���g
	int			m_fire_cnt_max;//���ˊԊu �Ⴂ�قǘA�ˑ��x������
	int			m_power;//�U����
	int			m_level;//�A�b�v�O���[�h���x��
	int			m_upgread_cost;//�A�b�v�O���[�h�R�X�g
	int			m_all_cost;//�R�X�g���v	���l����p
	int			m_sell;//���l
	float		m_search_length;//�˒�
	CANNON_KIND m_kind;//�C��̎��
	float		m_rot_x;//������]
	float		m_rot_y;//����]
	MY_POINT	m_chip_point;//�}�b�v�`�b�v��̈ʒu
};

//��C�N���X 0627 ���c�V��
class MAIN_CANNON : public CANNON
{
public:
	static const float HitSplashLength;//��C�͈̔̓_���[�W�̓����蔻��T�C�Y
	static MAIN_CANNON* pMainCannon;//��C
	//�R���X�g���N�^
	MAIN_CANNON();
	//�f�X�g���N�^
	~MAIN_CANNON();
	MAIN_CANNON* operator=(const MAIN_CANNON& obj);
	/**
	 *	@brief	�����[�h�Q�[�W�`��֐�
	 *	@author	���c�V��
	 *	@date	2015/07/22
	 */
	void DrawReload();
	/**
	 *	@brief	���e�\�z�ʒu�`��֐�
	 *	@author	���c�V��
	 *	@date	2015/07/27
	 */
	void DrawTargetPos();
private:
	/**
	 *	@brief	��C�X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/06/27
	 */
	void Update();
	/**
	 *	@brief	��C�`��֐�
	 *	@author	���c�V��
	 *	@date	2015/06/27
	 */
	//void Draw();
	D3DXVECTOR3 m_target_pos;//���e�\���n�_
};


//�K�g�����O�C�N���X 0627 ���c�V��
class GATLING_CANNON : public CANNON
{
public:
	//�R���X�g���N�^
	GATLING_CANNON();
	//�f�X�g���N�^
	~GATLING_CANNON();
	GATLING_CANNON* operator=(const MAIN_CANNON& obj);
private:
	/**
	 *	@brief	�K�g�����O�X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/06/27
	 */
	void Update();
};

//�΋�C��N���X 0816 ���c�V��
class ANTI_AIR_CANNON : public CANNON
{
public:
	//�R���X�g���N�^
	ANTI_AIR_CANNON();
	//�f�X�g���N�^
	~ANTI_AIR_CANNON();
private:
	/**
	 *	@brief	�΋�C��X�V�֐�
	 *	@author	���c�V��
	 *	@date	2015/08/16
	 */
	void Update();
};