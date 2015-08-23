//���f���N���X 0612���c�V��
//�\���̂̑O���錾
typedef struct tagTEX* PTEX;//TEX�̃|�C���^

class MODEL
{
private:
	static const int MAX_MODEL = 64;
	static MODEL sModel[MAX_MODEL];//���f��
private:
	LPD3DXMESH			pmesh;//���b�V��
	D3DMATERIAL9*		pmaterials;//�}�e���A���̔z��
	PTEX*				pPTEX;//�e�N�X�`���̔z��
	DWORD				nummaterials;//�}�e���A���̐�
	BOOL				used;//�f�[�^�������Ă��邩�����t���O
public:
	MODEL()//�R���X�g���N�^
	{
		used = 0;
	}
	~MODEL();//�f�X�g���N�^
	//X�t�@�C����ǂݍ��ފ֐�
	static MODEL* ReturnModel(LPCTSTR foldepath, LPCTSTR filename);
	//���b�V���ƃe�N�X�`���̕\���֐�
	void RenderModel();
	//���b�V����Ԃ�
	const LPD3DXMESH GetMesh()
	{
		return pmesh;
	}
};