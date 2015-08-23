///@file ���f���N���X 0612���c�V��
#include "my3dlib.h"
#include "basic3dtex.h"

MODEL MODEL::sModel[MAX_MODEL];//���f��

MODEL::~MODEL()
{
	//���f���̉��
	if (this->used == TRUE)
	{
		if (this->pmaterials != NULL)
		{
			delete[] this->pmaterials;
		}
		if (this->pPTEX != NULL)
		{
			for (DWORD j = 0; j < this->nummaterials; j++)
			{
				if (this->pPTEX[j] != NULL)//�e�N�X�`�����Ȃ��Ƃ��悤�ɒǉ�
				{
					FreeTex(this->pPTEX[j]);
				}
			}
		}
		delete[] this->pPTEX;
		if (this->pmesh != NULL)
		{
			this->pmesh->Release();
		}
	}
}

/**
 *	@brief	X�t�@�C����ǂݍ��ފ֐�
 *	@param	folderPath	�t�H���_�p�X	��:"model/"
 *	@param	filename	�t�@�C����		��:"jiki.x"
 *	@return	-1:���s	����ȊO:���f���z��̃C���f�b�N�X
 *	@author	���c�V��
 *	@date	��������
 */
MODEL* MODEL::ReturnModel(LPCTSTR folderPath, LPCTSTR filename)
{
	MODEL* pModel = NULL;
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (sModel[i].used == 0)
		{
			pModel = &sModel[i];
			break;
		}
	}
	if (pModel == NULL) return NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer;	//�ꎞ�L���p�o�b�t�@
	char loadXfilename[256];
	strcpy_s(loadXfilename, folderPath);
	strcat_s(loadXfilename, filename);
	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(loadXfilename, D3DXMESH_SYSTEMMEM,
		g_pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &pModel->nummaterials,
		&pModel->pmesh)))
	{
		char buf[256];
		sprintf_s(buf, "X�t�@�C����������܂��� �t�@�C����:%s", loadXfilename);
		MessageBox(NULL, buf, _T("3D Lib"), MB_OK);
		return NULL;
	}

	//�}�e���A���ƃe�N�X�`���L�^�p�z��̊m��
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	int num = pModel->nummaterials;
	pModel->pmaterials = new D3DMATERIAL9[num];
	if (pModel->pmaterials == NULL) return NULL;
	pModel->pPTEX = new TEX*[num];
	if (pModel->pPTEX == NULL) return NULL;

	for (int i = 0; i < num; i++)
	{
		//�}�e���A���̃R�s�[
		pModel->pmaterials[i] = d3dxMaterials[i].MatD3D;
		//�A���r�G���g�F�̐ݒ�
		pModel->pmaterials[i].Ambient = pModel->pmaterials[i].Diffuse;
		//�e�N�X�`���̓ǂݍ���
		pModel->pPTEX[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			//TEX�^�ɕύX
			pModel->pPTEX[i] = LoadTex(folderPath, d3dxMaterials[i].pTextureFilename);
			if (pModel->pPTEX[i] == NULL || pModel->pPTEX[i]->pTex == NULL)
			{
				MessageBox(NULL, _T("�e�N�X�`����������܂���"), _T("3D Lib"), MB_OK);
				return NULL;
			}
		}
	}
	pD3DXMtrlBuffer->Release();
	pModel->used = TRUE;

	return pModel;
}

/**
 *	@brief	�A�j���[�V�����̂Ȃ����f���`��֐�
 *	@param	model	�`�悷�郂�f���|�C���^
 *	@author	���c�V��
 *	@date	2014/12/16
 *	@note	���f���^�|�C���^�ŕ`��ł���悤�ɉ��� (�I�[�o�[���[�h)
 */
void MODEL::RenderModel()
{
	if (this->used == FALSE)
	{
		MessageBox(NULL, _T("���f�������_�[�G���[ ���f��������܂���"), _T("3dlib.cpp"), MB_OK);
		return;
	}
	for (DWORD i = 0; i < this->nummaterials; i++)
	{
		g_pD3DDevice->SetMaterial(&this->pmaterials[i]);
		if (this->pPTEX[i] != NULL)
		{
			g_pD3DDevice->SetTexture(0, this->pPTEX[i]->pTex);
		}
		else
		{
			g_pD3DDevice->SetTexture(0, NULL);
		}
		this->pmesh->DrawSubset(i);
	}
}
